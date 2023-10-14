#include "audio_out.h"

AudioOut::AudioOut() {

}

void AudioOut::spectogramm(stk::StkFrames& audio, cv::Mat& image) { // ------------------ Image -> Audio
    std::cout << "spectogramm" << std::endl;

    unsigned int width, height;

    width = static_cast<unsigned int>(image.cols);
    height = static_cast<unsigned int>(image.rows);

    unsigned int length = audio.frames();
    stk::StkFrames framesL(length, 1), framesR(length, 1);
    unsigned int n, f{0}, c{0};
    // double freq;

    framesL.setChannel(0, audio, 0);
    framesR.setChannel(0, audio, 1);

    n = round(sqrt(audio.frames() * sqrt(2)));
    f = round(sqrt(audio.frames() * (1 / sqrt(2))));

    cv::Size sizeA(n / 2, f);
    cv::Mat imgAudio(sizeA, CV_8UC3);
    imgAudio = 0;

    unsigned int i, j, w, h;
    uchar* ptr;

    h = f;
    w = n / 2.0;

    for (unsigned int y = 0; y < h; ++y) {

        std::vector<double> bandInL, bandInR, bandOutL, bandOutR;
        for (unsigned int i = 0; i < n; ++i) {

            bandInL.push_back(framesL[c]);
            bandInR.push_back(framesR[c]);

            ++c;
        }

        bandOutL = wtc::v1d::norm1d(wtc::v1d::fft(bandInL));
        bandOutR = wtc::v1d::norm1d(wtc::v1d::fft(bandInR));

        ptr = imgAudio.ptr<uchar>(y);
        for (unsigned int x = 0; x < w; ++x) {
          i = x * 3;
          j = ((w - 1) - x) * 3;

          double left, right;
          left  = (1 - bandOutL[x]) * 255.0;
          right = (1 - bandOutR[x]) * 255.0;

            ptr[i + 2] = round( (left + ptr[i + 2]) / 2);
            ptr[i + 1] = round( (left + ptr[i + 1]) / 2);
            ptr[i] = round( (left + ptr[i]) / 2);

            ptr[j + 2] = round( (right + ptr[j + 2]) / 2);
            ptr[j + 1] = round( (right + ptr[j + 1]) / 2);
            ptr[j] = round( (right + ptr[j]) / 2);
        }
    }

    cv::Size size(width, height);
    cv::resize(imgAudio, imgAudio, size, 0, 0, cv::INTER_CUBIC);

    cv::flip(imgAudio, imgAudio, -1);

    image = imgAudio;
}

void AudioOut::stereo_spectogramm(stk::StkFrames& audio, cv::Mat& image) { // ------------------ Image -> Audio
    std::cout << "stereo spectogramm" << std::endl;

    // settings
    const unsigned int max_bands{2048};

    unsigned int width, height, area, frames, bands{0}, cut, steps, index{0};

    width  = static_cast<unsigned int>(image.cols);
    height = static_cast<unsigned int>(image.rows);
    area = width * height;

    frames = audio.frames();

    stk::StkFrames framesL(frames, 1), framesR(frames, 1);

    framesL.setChannel(0, audio, 0);
    framesR.setChannel(0, audio, 1);

    if(frames < max_bands) {
      bands = frames / 2;
    } else {
      bands = max_bands;
    }

    cut = frames - bands;
    steps = area / bands * 2;

    std::vector< std::vector<double> > fftL, fftR;

    std::vector< std::vector< std::vector<double> > > fft;

    for(unsigned int i = 0; i < steps; i++) {

      index = round(static_cast<double>(cut) / steps * i);

      std::vector<double> bandInL, bandInR, bandOutL, bandOutR;
      for (unsigned int f = 0; f < bands; f++) {
        bandInL.push_back(framesL[index + f]);
        bandInR.push_back(framesR[index + f]);
      }

      bandOutL = wtc::v1d::fft(bandInL);
      bandOutR = wtc::v1d::fft(bandInR);

      fftL.push_back(bandOutL);
      fftR.push_back(bandOutR);

    }

    fft.push_back(fftL);
    fft.push_back(fftR);

    wtc::v3d::norm3dDouble(fft);

    fftL = fft[0];
    fftR = fft[1];

    unsigned int size_diff, y, yP{0}, x1, x2;
    double yN, xN;
    bool even;
    uchar valL, valR;

    size_diff = round(static_cast<double>(height) / fftL.size());

    std::cout << fftL.size() << " " << height << " " << size_diff << std::endl;

    std::mt19937 generator(8);
    std::uniform_int_distribution<int> distribution(0, size_diff);

    cv::Mat_<cv::Vec3b> _I = image;

    std::vector<double> hsl;
    std::vector<unsigned char> rgb;

    for (unsigned int i = 0; i < fftL.size(); ++i) {
      even = (i % 1);
      for (unsigned int j = 0; j < fftL[0].size(); ++j) {

        // y position
        yN = static_cast<double>(i) / static_cast<double>(fftL.size());
        // if(size_diff > 1) yP = distribution(generator);
        y = height - round(yN * static_cast<double>(height - 1)) + yP;
        if(y > height - 1) y = height - 1;

        // x position
        xN = static_cast<double>(j) / fftL[0].size() / 2;
        x1 = round(pow(xN, 0.6) * (width - 1));
        x2 = width - x1 - 1;

        valL = round(pow(fftL[i][j], 0.3) * 255);
        valR = round(pow(fftR[i][j], 0.3) * 255);

        _I(y,x1)[2] = valL + _I(y,x1)[2] / 2; // R
        _I(y,x1)[1] = valL + _I(y,x1)[1] / 2; // G
        _I(y,x1)[0] = valL + _I(y,x1)[0] / 2; // B

        _I(y,x2)[2] = valR + _I(y,x2)[2] / 2; // R
        _I(y,x2)[1] = valR + _I(y,x2)[1] / 2; // G
        _I(y,x2)[0] = valR + _I(y,x2)[0] / 2; // B

      }

    }

    image = _I;

    std::cout << "stereo spectogramm END" << std::endl;
}

void AudioOut::stereo_shape(stk::StkFrames& audio, cv::Mat& image) {
    std::cout << "stereo shape" << std::endl;

    // settings
    const unsigned int max_bands{2048};

    unsigned int width, height, area, frames, bands{0}, cut, steps, index{0};

    width  = static_cast<unsigned int>(image.cols);
    height = static_cast<unsigned int>(image.rows);
    area = width * height;

    frames = audio.frames();

    stk::StkFrames framesL(frames, 1), framesR(frames, 1);

    framesL.setChannel(0, audio, 0);
    framesR.setChannel(0, audio, 1);

    if(frames < max_bands) {
      bands = frames / 2;
    } else {
      bands = max_bands;
    }

    cut = frames - bands;
    steps = area / bands * 2;

    std::vector< std::vector<double> > fftL, fftR;

    std::vector< std::vector< std::vector<double> > > fft;

    for(unsigned int i = 0; i < steps; i++) {

      index = round(static_cast<double>(cut) / steps * i);

      std::vector<double> bandInL, bandInR, bandOutL, bandOutR;
      for (unsigned int f = 0; f < bands; f++) {
        bandInL.push_back(framesL[index + f]);
        bandInR.push_back(framesR[index + f]);
      }

      bandOutL = wtc::v1d::fft(bandInL);
      bandOutR = wtc::v1d::fft(bandInR);

      fftL.push_back(bandOutL);
      fftR.push_back(bandOutR);

    }

    fft.push_back(fftL);
    fft.push_back(fftR);

    wtc::v3d::norm3dDouble(fft);

    fftL = fft[0];
    fftR = fft[1];

    unsigned int size_diff, y, yP{0}, x, z;
    double yN, volume, tone, position;
    bool even;
    uchar valL, valR;

    size_diff = round(static_cast<double>(height) / fftL.size());

    std::cout << fftL.size() << " " << height << " " << size_diff << std::endl;

    std::mt19937 generator(8);
    std::uniform_int_distribution<int> distribution(0, size_diff);

    cv::Mat_<cv::Vec3b> _I = image;

    std::vector<double> hsl;
    std::vector<unsigned char> rgb;

    for (unsigned int i = 0; i < fftL.size(); ++i) {
      even = (i % 1);
      for (unsigned int j = 0; j < fftL[0].size(); ++j) {

        // y position
        yN = static_cast<double>(i) / fftL.size();
        if(size_diff > 1) yP = distribution(generator);
        y = height - round(yN * (height - 1)) + yP;
        if(y > height - 1) y = height - 1;

        volume = fftL[i][j] + fftR[i][j];
        tone = static_cast<double>(j) / fftL[0].size();
        position = fftL[i][j];
        z = 0;
        if (position < fftR[i][j]) {
          position = fftR[i][j];
          z = 1;
        }
        position = position / volume;

        // x position
        if(z) {
          x = round(position * (width - 1));
        } else {
          x = width - round(position * (width - 1));
        }

        valL = round(pow(fftL[i][j], 0.3) * 255);
        valR = round(pow(fftR[i][j], 0.3) * 255);

        _I(y,x)[2] = valL + _I(y,x)[2] / 2; // R
        _I(y,x)[1] = valL + _I(y,x)[1] / 2; // G
        _I(y,x)[0] = valL + _I(y,x)[0] / 2; // B

      }

    }

    image = _I;

    std::cout << "stereo shape END" << std::endl;
}
