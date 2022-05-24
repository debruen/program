
#include "hsl.h"

Hsl::Hsl() {

  _data = get_data();

  // 1
  nlohmann::json narrowing = data::init_float("narrowing", 0, 1, 0);
  _data.push_back(narrowing);

  // 2
  std::vector<std::string> keys_options {"97", "88"};
  nlohmann::json keys = data::init_str("keys", keys_options, keys_options[0]);

  _data.push_back(keys);

  std::vector<std::string> channel_options {"H", "s", "l"};
  /// 3:

  nlohmann::json a = data::init_str("a", channel_options, channel_options[0]);
  _data.push_back(a);

  /// 4:
  nlohmann::json b = data::init_str("b", channel_options, channel_options[1]);
  _data.push_back(b);

  /// 5:
  nlohmann::json c = data::init_str("c", channel_options, channel_options[2]);
  _data.push_back(c);

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

    for (std::size_t i = 0; i < input.size(); i++) {
        fftin[i][0] = input[i];
        fftin[i][1] = 0.0;
    }

    fftw_execute(fftplan);

    std::vector<double> band;

    for (std::size_t i = 0; i < input.size() / 2; i++) {
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

  return _data;
} // data() END

nlohmann::json Hsl::update(nlohmann::json data) {

  /// set output type value
  set_value(data::get_str(data, "type"));

  /// set hsl data
  _data = data;

  return _data;
} // data(data) END

void Hsl::set_audio_frequency(int& height, double& frequency) {
  // frequency = pow(frequency, 6);
  frequency = frequency * (21000 - 16) + 16;
  // frequency = math::project(m_audio_min, m_audio_max, frequency);
  frequency = frequency * ( static_cast<double>(height) / 44100.0);
}

void Hsl::image_frame(cv::Mat& image, cv::Mat& audio, std::size_t frame_index) {

  std::string chan_a, chan_b, chan_c, keyboard;

  chan_a = data::get_str(_data, "a");
  chan_b = data::get_str(_data, "b");
  chan_c = data::get_str(_data, "c");

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

  int audio_frames, sine_bands, n_length, narrow;

  audio_frames = audio.rows;

  sine_bands = image.cols / 8;

  double freq, keys, base, narrowing, table_hsl[256], table_lr[sine_bands][2], x_correction;

  narrowing = data::get_float(_data, "narrowing");
  keyboard = data::get_str(_data, "keys");

  if(keyboard == "88") {
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
  x_correction = 1 / (static_cast<double>(sine_bands));
  for (int i = 0; i < sine_bands; ++i) {
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
  unsigned int index{0};

  cv::Mat audio_image;

  cv::cvtColor(image, audio_image, cv::COLOR_BGR2HLS_FULL);

  cv::flip(audio_image, audio_image, 0);

  cv::Size size(sine_bands, audio_frames);

  std::cout << "sine_bands: " << sine_bands << '\n';
  std::cout << "audio_frames: " << audio_frames << '\n';

  try {
    cv::resize(audio_image, audio_image, size, 0, 0, cv::INTER_CUBIC);
  } catch( cv::Exception& e ) {
    const char* err_msg = e.what();
  }

  int c{0}, wa{2}, ha = audio_frames;
  std::string shape = "sine";

  Sine sine(wa, ha, frame_index, shape, 0, 0, 0, "audio");

  uchar* ptr;
  double* dptr;

  for (int y = 0; y < audio_frames; y++) {
    left = 0;
    right = 0;

    ptr = audio_image.ptr<uchar>(y);
    for (int x = 0; x < sine_bands; x++) {
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

      set_audio_frequency(audio_frames, tonefreq);
      sine.set_frequency(tonefreq);

      discrete_l = sine.point(y, c) * vol;

      left = left + discrete_l * table_lr[x][0];
      right = right + discrete_l * table_lr[x][1];
    }

    dptr = audio.ptr<double>(y);
    dptr[0] = left;
    dptr[1] = right;
  }

}

void Hsl::audio_frame(cv::Mat& image, cv::Mat& audio, std::size_t frame_index) {

  std::size_t audio_frames = audio.rows;

  stk::StkFrames framesL(audio_frames, 1), framesR(audio_frames, 1), audio_comp(audio_frames, 2);

  // settings
  const std::size_t max_bands{2048};

  std::size_t width, height, area, bands{0}, cut, steps, index{0};

  width  = image.cols;
  height = image.rows;
  area = width * height;

  if(audio_frames < max_bands) {
    bands = audio_frames / 2;
  } else {
    bands = max_bands;
  }

  cut = audio_frames - bands;
  steps = area / bands * 2;

  std::vector< std::vector<double> > fftL, fftR;

  std::vector< std::vector< std::vector<double> > > fftN;


  for(std::size_t i = 0; i < steps; i++) {

    index = round(static_cast<double>(cut) / steps * i);

    std::vector<double> bandInL, bandInR, bandOutL, bandOutR;

    double* dptr;
    for (std::size_t f = 0; f < bands; f++) {
      dptr = audio.ptr<double>(index + f);

      bandInL.push_back(dptr[0]);
      bandInR.push_back(dptr[1]);
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

  std::size_t size_diff, y, yP{0}, x, z;
  double yN, volume, tone, position;
  bool even;
  uchar valL, valR;

  int w = (width - 1) / 2, xL, xR;

  // size_diff = round(static_cast<double>(height) / fftL.size());
  //
  // std::mt19937 generator(8);
  // std::uniform_int_distribution<int> distribution(0, size_diff);

  cv::Mat_<cv::Vec3b> _I = image;

  std::vector<double> hsl;
  std::vector<unsigned char> rgb;

  for (std::size_t i = 0; i < fftL.size(); ++i) {
    even = (i % 1);
    for (std::size_t j = 0; j < fftL[0].size(); ++j) {

      // y position
      yN = static_cast<double>(i) / fftL.size();
      // if(size_diff > 1) yP = distribution(generator);
      y = round(yN * (height - 1));
      if(y > height - 1) y = height - 1;

      xL = round(fftL[i][j] * w) + w;
      xR = round(w - (fftR[i][j] * w));

      if(xL > width - 1) xL = width - 1;
      if(xR < 0) xR = 0;

      //
      // volume = fftL[i][j] + fftR[i][j];
      // tone = static_cast<double>(j) / fftL[0].size();
      // position = fftL[i][j];
      // z = 0;
      // if (position < fftR[i][j]) {
      //   position = fftR[i][j];
      //   z = 1;
      // }
      // position = position / volume;
      //
      // // x position
      // if(z) {
      //   x = round(position * (width - 1));
      // } else {
      //   x = width - round(position * (width - 1));
      // }

      valL = round(pow(fftL[i][j], 0.3) * 255);
      valR = round(pow(fftR[i][j], 0.3) * 255);

      _I(y,xL)[2] = valL + _I(y,xL)[2] / 2; // R
      _I(y,xL)[1] = valL + _I(y,xL)[1] / 2; // G
      _I(y,xL)[0] = valL + _I(y,xL)[0] / 2; // B

      _I(y,xR)[2] = valR + _I(y,xR)[2] / 2; // R
      _I(y,xR)[1] = valR + _I(y,xR)[1] / 2; // G
      _I(y,xR)[0] = valR + _I(y,xR)[0] / 2; // B
    }

  }

  image = _I;

}
