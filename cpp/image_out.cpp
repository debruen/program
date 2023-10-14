#include "image_out.h"

ImageOut::ImageOut() {

  m_default["type"] = "hsl_to_signal";
  m_default["narrowing"] = 0.0;
  m_default["channel_a"] = 0;
  m_default["channel_b"] = 1;
  m_default["channel_c"] = 2;
  m_default["keyboard"] = 97;

}

nlohmann::json ImageOut::get_default() {
  return m_default;
}

void ImageOut::set_data(nlohmann::json data) {

  m_channel_a = data["channel_a"];
  m_channel_b = data["channel_b"];
  m_channel_c = data["channel_c"];

  m_keys = data["keyboard"];

  m_narrowing = data["narrowing"];

}

void ImageOut::setSine(unsigned int bands) {
    std::vector<stk::SineWave> sineA(bands);
    std::vector<stk::SineWave> sineB(bands);

    sineL = sineA;
    sineR = sineB;

    std::mt19937 generator(8);
    std::uniform_real_distribution<double> distribution(-1.0,1.0);

    std::vector<double> phase;
    for (unsigned int i = 0; i < bands; ++i) {
        phase.push_back(distribution(generator));
    }
    for (unsigned int i = 0; i < bands; ++i) {
      sineL[i].addPhase(phase[i]);
      sineR[i].addPhase(phase[(bands - 1) - i]);
    }
}

void ImageOut::hsl_to_signal(cv::Mat& image, stk::StkFrames& audio, nlohmann::json data, unsigned int page) { // ------------------ Image -> Audio
    std::cout << "HsltoSignal: resize" << std::endl;

    set_data(data);

    unsigned int length, bands, nlength, narrow, index;

    // get dimensions
    length = audio.frames();
    bands = image.cols / 2;

    double table_H[181], table_sl[256], H, s, l, /* table_top[181][256][256][2], */ table_lr[bands][2], freq, base, left, right, xcorrection, basefreq, tonefreq, discreteL, discreteR, chanA, volume, chanC, shiftKeys;
    cv::Mat imgImage;

    if(m_keys == 88) {
      freq = 27.5000;
      shiftKeys = 76.0;
    } else {
      freq = 16.3516;
      shiftKeys = 85.0;
    }
    base = pow(2, 1/12.0);

    for (int i = 0; i < 256; ++i)
      table_sl[i] = static_cast<double>(i) / 255;

    for (int i = 0; i < 181; ++i)
      table_H[i] = static_cast<double>(i) / 180;

    // for (int i = 0; i < 181; ++i) {
    //   for (int j = 0; j < 256; ++j) {
    //     for (int k = 0; k < 256; ++k) {
    //
    //       if(m_channel_a == 0) chanA = table_H[i];
    //       if(m_channel_a == 1) chanA = table_sl[j];
    //       if(m_channel_a == 2) chanA = table_sl[k];
    //
    //       if(m_channel_b == 0) volume = table_H[i];
    //       if(m_channel_b == 1) volume = table_sl[j];
    //       if(m_channel_b == 2) volume = table_sl[k];
    //
    //       if(m_channel_c == 0) chanC = table_H[i];
    //       if(m_channel_c == 1) chanC = table_sl[j];
    //       if(m_channel_c == 2) chanC = table_sl[k];
    //
    //       basefreq = pow(base, chanA * 12.0) * freq;
    //       tonefreq = pow(base, chanC * shiftKeys) * basefreq;
    //
    //       table_top[i][j][k][0] = tonefreq;
    //       table_top[i][j][k][1] = volume;
    //
    //     }
    //   }
    // }

    // stereo distribution
    nlength = round(m_narrowing * bands / 2);
    xcorrection = 8.0 / (static_cast<double>(bands));
    for (unsigned int i = 0; i < bands; ++i) {
      if(nlength == 0) {
        narrow = i;
      } else if(i < nlength) {
        narrow = 0;
      } else if (i > bands - nlength) {
        narrow = bands - 1;
      } else {
        narrow = (static_cast<double>(i) - nlength) / m_narrowing;
      }

      table_lr[i][0] = static_cast<double>(narrow) / (bands - 1) * xcorrection;
      table_lr[i][1] = (1 - static_cast<double>(narrow) / (bands - 1)) * xcorrection;

    }

    // image conversion
    cv::cvtColor(image, imgImage, cv::COLOR_BGR2HLS);
    cv::flip(imgImage, imgImage, -1);
    if(static_cast<unsigned int>(imgImage.cols) != bands && static_cast<unsigned int>(imgImage.rows) != length) {
        cv::Size size(bands, length);
        cv::resize(imgImage, imgImage, size, 0, 0, cv::INTER_CUBIC);
    }

    // set sine
    if(page == 0) {
      setSine(bands);
    }

    // init frames
    stk::StkFrames framesL(length, 1), framesR(length, 1);

    std::cout << "HsltoSignal: read" << std::endl;

    uchar* ptr;
    for (unsigned int y = 0; y < length; ++y) {

      left = 0;
      right = 0;

      ptr = imgImage.ptr<uchar>(y);
      for (unsigned int x = 0; x < bands; ++x) {
        index = x + x + x;

        H = table_H[ptr[index]];
        s = table_sl[ptr[index + 2]];
        l = table_sl[ptr[index + 1]];

        if(m_channel_a == 0) chanA = H;
        if(m_channel_a == 1) chanA = s;
        if(m_channel_a == 2) chanA = l;

        if(m_channel_b == 0) volume = H;
        if(m_channel_b == 1) volume = s;
        if(m_channel_b == 2) volume = l;

        if(m_channel_c == 0) chanC = H;
        if(m_channel_c == 1) chanC = s;
        if(m_channel_c == 2) chanC = l;

        basefreq = pow(base, chanA * 12.0) * freq;
        tonefreq = pow(base, chanC * shiftKeys) * basefreq;

        sineL[x].setFrequency(tonefreq);
        sineR[x].setFrequency(tonefreq);

        discreteL = sineL[x].tick() * volume;
        discreteR = sineR[x].tick() * volume;

        // sineL[x].setFrequency(table_top[ptr[index]][ptr[index + 2]][ptr[index + 1]][0]);
        // sineR[x].setFrequency(table_top[ptr[index]][ptr[index + 2]][ptr[index + 1]][0]);
        //
        // discreteL = sineL[x].tick() * table_top[ptr[index]][ptr[index + 2]][ptr[index + 1]][1];
        // discreteR = sineR[x].tick() * table_top[ptr[index]][ptr[index + 2]][ptr[index + 1]][1];

        left = left + discreteL * table_lr[x][0];
        right = right + discreteR * table_lr[x][1];
      }

      framesL[y] = left;
      framesR[y] = right;
    }
    std::cout << "HsltoSignal: done" << std::endl;
    audio.setChannel(0, framesL, 0);
    audio.setChannel(1, framesR, 0);

}

void ImageOut::rgb_to_chord(cv::Mat& image, stk::StkFrames& audio, nlohmann::json data, unsigned int page) { // ------------------ Image -> Audio
    std::cout << "rgb_to_chord: resize" << std::endl;

    set_data(data);

    // set sine
    if(page == 0) {
      setSine(3);
    }

    unsigned int length, bands, nlength, narrow, index;

    // get dimensions
    length = audio.frames();
    bands = image.cols / 2;

    double freqA{261.626}, freqB{329.628}, freqC{391.995}, table_color[256], table_lr[bands][2], leftA, leftB, leftC, rightA, rightB, rightC, xcorrection;

    sineL[0].setFrequency(freqA);
    sineL[1].setFrequency(freqB);
    sineL[2].setFrequency(freqC);

    sineR[0].setFrequency(freqA);
    sineR[1].setFrequency(freqB);
    sineR[2].setFrequency(freqC);

    cv::Mat imgImage;

    image.copyTo(imgImage);

    cv::flip(imgImage, imgImage, -1);
    if(static_cast<unsigned int>(imgImage.cols) != bands && static_cast<unsigned int>(imgImage.rows) != length) {
        cv::Size size(bands, length);
        cv::resize(imgImage, imgImage, size, 0, 0, cv::INTER_CUBIC);
    }

    // init frames
    stk::StkFrames framesL(length, 1), framesR(length, 1);

    // stereo distribution
    nlength = round(m_narrowing * bands / 2);
    xcorrection = 1.0 / (static_cast<double>(bands) * 1.5);
    for (unsigned int i = 0; i < bands; ++i) {
      if(nlength == 0) {
        narrow = i;
      } else if(i < nlength) {
        narrow = 0;
      } else if (i > bands - nlength) {
        narrow = bands - 1;
      } else {
        narrow = (static_cast<double>(i) - nlength) / m_narrowing;
      }

      table_lr[i][0] = static_cast<double>(narrow) / (bands - 1) * xcorrection;
      table_lr[i][1] = (1 - static_cast<double>(narrow) / (bands - 1)) * xcorrection;

    }

    for (int i = 0; i < 256; ++i)
      table_color[i] = static_cast<double>(i) / 255;

    std::cout << "rgb_to_chord: read" << std::endl;

    uchar* ptr;
    for (unsigned int y = 0; y < length; ++y) {

      leftA = 0;
      leftB = 0;
      leftC = 0;

      rightA = 0;
      rightB = 0;
      rightC = 0;

      ptr = imgImage.ptr<uchar>(y);
      for (unsigned int x = 0; x < bands; ++x) {
        index = x + x + x;

        leftA = leftA + table_color[ptr[index]] * table_lr[x][0];
        leftB = leftB + table_color[ptr[index + 2]] * table_lr[x][0];
        leftC = leftC + table_color[ptr[index + 1]] * table_lr[x][0];

        rightA = rightA + table_color[ptr[index]] * table_lr[x][1];
        rightB = rightB + table_color[ptr[index + 2]] * table_lr[x][1];
        rightC = rightC + table_color[ptr[index + 1]] * table_lr[x][1];

      }

      framesL[y] = (sineL[0].tick() * leftA + sineL[1].tick() * leftB + sineL[2].tick() * leftC);
      framesR[y] = (sineR[0].tick() * rightA + sineR[1].tick() * rightB + sineR[2].tick() * rightC);
    }

    std::cout << "rgb_to_chord: done" << std::endl;

    audio.setChannel(0, framesL, 0);
    audio.setChannel(1, framesR, 0);

}
