
#include "gradient.h"

Gradient::Gradient() {

  m_data = get_data();

  std::vector<std::string> shape_values{"sine", "saw", "square", "triangle"};
  m_data.push_back(data::init_str("shape", shape_values, m_shape));
  m_data.push_back(data::init_1d_float_data("frequency range", {m_freq_min, m_freq_max}));

  m_data.push_back(data::init_value("frequency gamma", m_frq_gamma));

  m_data.push_back(data::init_float("frequency", 0, 1, m_frequency));

  m_data.push_back(data::init_float("amplitude", 0, 1, m_amplitude));

  m_data.push_back(data::init_float("phase", m_phase_min, m_phase_max, m_phase));

  m_data.push_back(data::init_float("tilt", 0, 1, m_tilt));

  std::vector<std::string> filter_options{"none", "frequency", "phase", "amplitude", "tilt"};
  m_data.push_back(data::init_str("filter", filter_options, m_filter));
}

nlohmann::json Gradient::init() {

  return m_data;
}

nlohmann::json Gradient::update(nlohmann::json data) {

  m_shape     = data::get_str(data, "shape");
  m_frequency = data::get_float(data, "frequency");
  m_amplitude = data::get_float(data, "amplitude");
  m_phase     = data::get_float(data, "phase");

  m_tilt      = data::get_float(data, "tilt");
  m_filter    = data::get_str(data, "filter");

  m_data = data;

  return m_data;
}

void Gradient::set_area_frequency(double& frequency) {
  frequency = pow(m_frequency, m_frq_gamma);
  frequency = frequency * (m_freq_max - m_freq_min) + m_freq_min;
  frequency = frequency * 2;
}

cv::Mat Gradient::frame(cv::Mat& mask, std::size_t frame_index, std::string type) {

  double frequency;

  set_area_frequency(frequency);
  if (type == "image") {
    frequency = frequency * 2;
  }

  Sine sine(mask.cols, mask.rows, frame_index, m_shape, frequency, m_phase, m_tilt, type);

  cv::parallel_for_(cv::Range(0, mask.rows), [&](const cv::Range &range) {
    for (int y = range.start; y < range.end; y++) {
      for (int x = 0; x < mask.cols; x++) {
        mask.ptr<double>(y)[x] = (sine.point(y, x) * m_amplitude + 1) / 2;
      }
    }
  });

  return mask;
}

void Gradient::process(cv::Mat& mask, std::size_t frame_index, std::string type) {

  unsigned char filter_select = 0;
  if (m_filter == "frequency") filter_select = 1;
  if (m_filter == "phase") filter_select = 2;
  if (m_filter == "amplitude") filter_select = 3;
  if (m_filter == "tilt") filter_select = 4;

  double frequency, amplitude;

  amplitude = m_amplitude;

  set_area_frequency(frequency);

  double phase, pm;

  // for (int i = 0; i < page; i++) {
  //   if(m_tilt <= 0.25) {
  //     pm = 1 - m_tilt * 4;
  //   } else if (m_tilt <= 0.5) {
  //     pm = (m_tilt - 0.25) * (-4);
  //   } else if (m_tilt <= 0.75) {
  //     pm = 1 - (m_tilt - 0.5) * 4;
  //   } else {
  //     pm = (m_tilt - 0.75) * (-4);
  //   }
  //   m_temp_phase += frequency * pm;
  // }

  Sine sine(mask.cols, mask.rows, frame_index, m_shape, frequency, m_phase, m_tilt, type);

  // cv::parallel_for_(cv::Range(0, mask.rows), [&](const cv::Range &range) {
    double* ptr;
    for (int y = 0; y < mask.rows; y++) {
      ptr = mask.ptr<double>(y);
      for (int x = 0; x < mask.cols; x++) {
        if (filter_select == 1) sine.set_frequency(ptr[x]);
        if (filter_select == 2) sine.set_phase(ptr[x]);
        if (filter_select == 3) amplitude = ptr[x];
        if (filter_select == 4) sine.set_tilt(ptr[x]);
        ptr[x] = (sine.point(y, x) * amplitude + 1) / 2;
        // ptr[x] = math::normalize(m_norm_low, m_norm_high, sine.point(y, x)) * amplitude;
      }
    }
  // });

}
