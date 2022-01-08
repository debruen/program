
#include "hsl.h"

Hsl::Hsl() {

  // 0
  hsl_data = get_data();

  // 1

  nlohmann::json narrowing = data::data_float("narrowing", 0, 1, 0);
  hsl_data.push_back(narrowing);

  // 2
  std::vector<std::string> keys_options {"97", "88"};
  nlohmann::json keys = data::data_string("keys", true, keys_options, keys_options[0]);

  hsl_data.push_back(keys);

  std::vector<std::string> channel_options {"H", "s", "l"};
  /// 3:

  nlohmann::json a = data::data_string("a", true, channel_options, channel_options[0]);
  hsl_data.push_back(a);

  /// 4:
  nlohmann::json b = data::data_string("b", true, channel_options, channel_options[1]);
  hsl_data.push_back(b);

  /// 5:
  nlohmann::json c = data::data_string("c", true, channel_options, channel_options[2]);
  hsl_data.push_back(c);

  nlohmann::json save = get_save();
  hsl_data.push_back(save);

} // constructor END

double Hsl::normDouble(double min, double max, double value) {
    return (value - min) / (max - min);
}

std::vector<double> Hsl::minMax(std::vector<double>& v) {
    std::vector<double> temp;

    temp.push_back(*std::min_element(v.begin(), v.end()));
    temp.push_back(*std::max_element(v.begin(), v.end()));

    return temp;
}

std::vector<double> Hsl::fft(std::vector<double> input) {
    fftw_complex *fftin, *fftout;
    fftw_plan fftplan;

    fftin = (fftw_complex*) fftw_malloc(sizeof(fftw_complex) * input.size()); // allocating memory
    fftout = (fftw_complex*) fftw_malloc(sizeof(fftw_complex) * input.size());
    fftplan = fftw_plan_dft_1d(input.size(), fftin, fftout, FFTW_FORWARD, FFTW_ESTIMATE); // Here we set which kind of transformation we want to perform

    for (int i = 0; i < static_cast<int>(input.size()); ++i) {
        fftin[i][0] = input[i];
        fftin[i][1] = 0.0;
    }

    fftw_execute(fftplan);

    std::vector<double> band;

    for (int i = 0; i < static_cast<int>(input.size()) / 2; ++i) {

        band.push_back(sqrt(fftout[i][0] * fftout[i][0] + fftout[i][1] * fftout[i][1]));
    }

    fftw_destroy_plan(fftplan);
    fftw_cleanup();

    return band;
}

void Hsl::norm3dDouble(std::vector< std::vector< std::vector<double> > >& vector) {

    std::vector<double> check, range;

    for (unsigned int i = 0; i < vector.size(); ++i) {
        for (unsigned int j = 0; j < vector[0].size(); ++j) {
            for (unsigned int k = 0; k < vector[0][0].size(); ++k) {
                check.push_back(vector[i][j][k]);
            }
        }
    }
    range = minMax(check);

    for (unsigned int i = 0; i < vector.size(); ++i) {
        for (unsigned int j = 0; j < vector[0].size(); ++j) {
            for (unsigned int k = 0; k < vector[0][0].size(); ++k) {
                vector[i][j][k] = normDouble(range[0], range[1], vector[i][j][k]);
            }
        }
    }
}

nlohmann::json Hsl::data() {

  return hsl_data;
} // data() END

nlohmann::json Hsl::data(nlohmann::json data) {

  /// set output type value
  set_value(data::get_string(data, "type"));

  /// set hsl data
  hsl_data = data;

  return hsl_data;
} // data(data) END

void Hsl::process(std::vector<cv::Mat>& images, stk::StkFrames& audio) {

  std::string chan_a, chan_b, chan_c;

  chan_a = data::get_string(hsl_data, "a");
  chan_b = data::get_string(hsl_data, "b");
  chan_c = data::get_string(hsl_data, "c");

  uchar ca, cb, cc;
  if(chan_a == "H") ca = 0;
  if(chan_a == "s") ca = 1;
  if(chan_a == "l") ca = 2;

  if(chan_b == "H") cb = 0;
  if(chan_b == "s") cb = 1;
  if(chan_b == "l") cb = 2;

  if(chan_c == "H") cc = 0;
  if(chan_c == "s") cc = 1;
  if(chan_c == "l") cc = 2;

  unsigned int image_frames, audio_frames, audio_frame_part, sine_bands, n_length, narrow;

  image_frames = images.size();
  audio_frames = audio.frames();

  audio_frame_part = audio_frames / image_frames;
  sine_bands = images[0].cols / 2;

  set_sine(sine_bands);

  double freq, keys, base, narrowing, table_hsl[256], table_lr[sine_bands][2], x_correction;

  narrowing = hsl_data[1]["value"];

  if(hsl_data[2]["value"] == 88) {
    freq = 27.5000;
    keys = 76.0;
  } else {
    freq = 16.3516;
    keys = 85.0;
  }
  base = pow(2, 1/12.0);

  // creating normalised LUT for Hsl
  for (int i = 0; i < 256; ++i)
    table_hsl[i] = static_cast<double>(i) / 255;

  n_length = round(narrowing * sine_bands / 2);
  x_correction = 8.0 / (static_cast<double>(sine_bands));
  for (unsigned int i = 0; i < sine_bands; ++i) {
    if(n_length == 0) {
      narrow = i;
    } else if(i < n_length) {
      narrow = 0;
    } else if (i > sine_bands - n_length) {
      narrow = sine_bands - 1;
    } else {
      narrow = (static_cast<double>(i) - n_length) / narrowing;
    }

    table_lr[i][0] = (1 - static_cast<double>(narrow) / (sine_bands - 1)) * x_correction;
    table_lr[i][1] = static_cast<double>(narrow) / (sine_bands - 1) * x_correction;

  }

  double left, right, h, s, l, b_freq, vol, s_freq, basefreq, tonefreq, discrete_l, discrete_r;
  unsigned int index;

  stk::StkFrames frames_l(audio_frames, 1), frames_r(audio_frames, 1);

  // image frame itteration
  for (unsigned int i = 0; i < image_frames; i++) {

    cv::Mat audio_image;
    stk::StkFrames frames_part_l(audio_frame_part, 1), frames_part_r(audio_frame_part, 1);

    cv::cvtColor(images[i], audio_image, cv::COLOR_BGR2HLS_FULL);
    cv::flip(audio_image, audio_image, 0);
    cv::Size size(sine_bands, audio_frame_part);
    cv::resize(audio_image, audio_image, size, 0, 0, cv::INTER_CUBIC);

    uchar* ptr;
    for (unsigned int y = 0; y < audio_frame_part; y++) {

      left = 0;
      right = 0;

      ptr = audio_image.ptr<uchar>(y);

      for (unsigned int x = 0; x < sine_bands; x++) {
        index = x + x + x;

        h = table_hsl[ptr[index]];
        s = table_hsl[ptr[index + 2]];
        l = table_hsl[ptr[index + 1]];

        if(ca == 0) b_freq = h;
        if(ca == 1) b_freq = s;
        if(ca == 2) b_freq = l;

        if(cb == 0) vol = h;
        if(cb == 1) vol = s;
        if(cb == 2) vol = l;

        if(cc == 0) s_freq = h;
        if(cc == 1) s_freq = s;
        if(cc == 2) s_freq = l;

        basefreq = pow(base, b_freq * 12.0) * freq;
        tonefreq = pow(base, s_freq * keys) * basefreq;

        m_sine_l[x].setFrequency(tonefreq);
        m_sine_r[x].setFrequency(tonefreq);

        discrete_l = m_sine_l[x].tick() * vol;
        discrete_r = m_sine_r[x].tick() * vol;

        left = left + discrete_l * table_lr[x][0];
        right = right + discrete_r * table_lr[x][1];
      }

      frames_part_l[y] = left;
      frames_part_r[y] = right;
    }

    unsigned int start, stop, f;
    start = i * audio_frame_part;
    stop = (i + 1) * audio_frame_part;
    f = 0;
    for (unsigned int a = start; a < stop; a++) {
      frames_l[a] = frames_part_l[f];
      frames_r[a] = frames_part_r[f];
      f++;
    }

  } // image frame itteration

  audio.setChannel(0, frames_l, 0);
  audio.setChannel(1, frames_r, 0);

} // process(images, audio) END

void Hsl::process(stk::StkFrames& audio, std::vector<cv::Mat>& images) {

  unsigned int image_frames, audio_frames, audio_frame_part;

  image_frames = images.size();
  audio_frames = audio.frames();

  audio_frame_part = audio_frames / image_frames;

  stk::StkFrames framesL(audio_frame_part, 1), framesR(audio_frame_part, 1), audio_comp(audio_frame_part, 2);


  stk::StkFrames preL(audio_frames, 1), preR(audio_frames, 1);

  preL.setChannel(0, audio, 0);
  preR.setChannel(0, audio, 1);

  for (unsigned int i = 0; i < images.size(); i++) {

    unsigned int start, stop, f;
    start = round(i * static_cast<double>(audio_frame_part));
    stop = round((i + 1) * static_cast<double>(audio_frame_part));
    f = 0;
    for (unsigned int a = start; a < stop; a++) {
      framesL[f] = preL[a];
      framesR[f] = preR[a];
      f++;
    }
    audio_comp.setChannel(0, framesL, 0);
    audio_comp.setChannel(1, framesR, 0);

    // settings
    const unsigned int max_bands{2048};

    unsigned int width, height, area, bands{0}, cut, steps, index{0};

    width  = static_cast<unsigned int>(images[i].cols);
    height = static_cast<unsigned int>(images[i].rows);
    area = width * height;

    framesL.setChannel(0, audio_comp, 0);
    framesR.setChannel(0, audio_comp, 1);

    if(audio_frame_part < max_bands) {
      bands = audio_frame_part / 2;
    } else {
      bands = max_bands;
    }

    cut = audio_frame_part - bands;
    steps = area / bands * 2;

    std::vector< std::vector<double> > fftL, fftR;

    std::vector< std::vector< std::vector<double> > > fftN;

    for(unsigned int i = 0; i < steps; i++) {

      index = round(static_cast<double>(cut) / steps * i);

      std::vector<double> bandInL, bandInR, bandOutL, bandOutR;
      for (unsigned int f = 0; f < bands; f++) {
        bandInL.push_back(framesL[index + f]);
        bandInR.push_back(framesR[index + f]);
      }

      bandOutL = fft(bandInL);
      bandOutR = fft(bandInR);

      fftL.push_back(bandOutL);
      fftR.push_back(bandOutR);

    }

    fftN.push_back(fftL);
    fftN.push_back(fftR);

    norm3dDouble(fftN);

    fftL = fftN[0];
    fftR = fftN[1];

    unsigned int size_diff, y, yP{0}, x, z;
    double yN, volume, tone, position;
    bool even;
    uchar valL, valR;

    size_diff = round(static_cast<double>(height) / fftL.size());

    std::mt19937 generator(8);
    std::uniform_int_distribution<int> distribution(0, size_diff);

    cv::Mat_<cv::Vec3b> _I = images[i];

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

    images[i] = _I;
  }
} // process(audio, images) END
