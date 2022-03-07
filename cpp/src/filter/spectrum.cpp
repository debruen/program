
#include "spectrum.h"

Spectrum::Spectrum() {

  m_data = coat_data();

  m_data.push_back(data::data_data("color range", m_color_min, m_color_max));

  m_spectral_rgb = rgb_spectrum();
  m_data.push_back(data::init_2d_uchar_data("rgb", m_spectral_rgb));

  std::vector<std::string> shape_options{"sine", "saw", "square", "triangle"};
  m_data.push_back(data::init_str("shape", shape_options, m_shape));

  m_data.push_back(data::data_data("audio range", m_audio_min, m_audio_max));

  m_data.push_back(data::data_value("frequency gamma", m_frq_gamma));

  m_data.push_back(data::data_float("frequency", 0, 1, m_frequency));

  m_data.push_back(data::data_float("amplitude", 0, 1, m_amplitude));

  m_data.push_back(data::data_float("phase", 0, 1, m_phase));

  m_data.push_back(data::data_float("tilt", 0, 1, m_tilt));

}

void Spectrum::spectral_rgb(double& r, double& g, double& b, double l) {
    double t;  r=0.0; g=0.0; b=0.0;
         if ((l>=400.0)&&(l<410.0)) { t=(l-400.0)/(410.0-400.0); r=    +(0.33*t)-(0.20*t*t); }
    else if ((l>=410.0)&&(l<475.0)) { t=(l-410.0)/(475.0-410.0); r=0.14         -(0.13*t*t); }
    else if ((l>=545.0)&&(l<595.0)) { t=(l-545.0)/(595.0-545.0); r=    +(1.98*t)-(     t*t); }
    else if ((l>=595.0)&&(l<650.0)) { t=(l-595.0)/(650.0-595.0); r=0.98+(0.06*t)-(0.40*t*t); }
    else if ((l>=650.0)&&(l<700.0)) { t=(l-650.0)/(700.0-650.0); r=0.65-(0.84*t)+(0.20*t*t); }
         if ((l>=415.0)&&(l<475.0)) { t=(l-415.0)/(475.0-415.0); g=             +(0.80*t*t); }
    else if ((l>=475.0)&&(l<590.0)) { t=(l-475.0)/(590.0-475.0); g=0.8 +(0.76*t)-(0.80*t*t); }
    else if ((l>=585.0)&&(l<639.0)) { t=(l-585.0)/(639.0-585.0); g=0.84-(0.84*t)           ; }
         if ((l>=400.0)&&(l<475.0)) { t=(l-400.0)/(475.0-400.0); b=    +(2.20*t)-(1.50*t*t); }
    else if ((l>=475.0)&&(l<560.0)) { t=(l-475.0)/(560.0-475.0); b=0.7 -(     t)+(0.30*t*t); }
}

std::vector< std::vector<unsigned char> > Spectrum::rgb_spectrum() {
  std::vector< std::vector<unsigned char> > rgb;
  double r, g, b;

  for (std::size_t l = 400; l <= 700; l++) {
    std::vector<unsigned char> value;
    spectral_rgb(r, g, b, l);
    value.push_back(round(b * 255));
    value.push_back(round(g * 255));
    value.push_back(round(r * 255));
    rgb.push_back(value);
  }

  return rgb;
}

nlohmann::json Spectrum::data() {

  return m_data;
}

nlohmann::json Spectrum::update(nlohmann::json data) {

  m_shape     = data::get_string(data, "shape");
  m_frequency = data::get_float(data, "frequency");
  m_amplitude = data::get_float(data, "amplitude");
  m_phase     = data::get_float(data, "phase");

  m_data = data;

  return m_data;
}

void Spectrum::set_audio_frequency(const std::size_t& height, double& frequency) {
  frequency = pow(m_frequency, m_frq_gamma);
  frequency = math::project(m_audio_min, m_audio_max, frequency);
  frequency = frequency * height / 44100.0;
}

cv::Mat Spectrum::image_frame(cv::Mat& image, std::size_t frame_index) {

  cv::Size size(image.cols, image.rows);

  cv::Mat film = cv::Mat(size, CV_8UC3);

  std::size_t color_index = round((1 - m_frequency) * (m_spectral_rgb.size() - 1));

  unsigned char r, g, b;

  r = m_spectral_rgb[color_index][0];
  g = m_spectral_rgb[color_index][1];
  b = m_spectral_rgb[color_index][2];

  film = cv::Scalar(r, g, b);

  return film;
}

cv::Mat Spectrum::audio_frame(cv::Mat& audio, std::size_t frame_index) {

  const std::size_t& width(audio.cols),& height(audio.rows);

  cv::Mat film = cv::Mat(cv::Size(width, height), CV_64F);

  double frequency, amplitude;

  set_audio_frequency(height, frequency);

  amplitude = m_amplitude;

  AudioSine sine(width, height, frame_index, frequency, m_phase, m_tilt, m_shape);

  cv::parallel_for_(cv::Range(0, height), [&](const cv::Range &range) {
    for (std::size_t y = range.start; y < range.end; y++) {
      for (std::size_t x = 0; x < width; x++) {
        film.ptr<double>(y)[x] = sine.point(y, x) * m_amplitude;
        // film.ptr<double>(y)[x] = 0;
      }
    }
  });

  return film;
}
