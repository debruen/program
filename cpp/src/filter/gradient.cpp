
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

  std::vector<std::string> filter_options{"none", "frequency", "amplitude", "phase", "tilt"};
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
  frequency = math::project(m_freq_min, m_freq_max, frequency);
}

cv::Mat Gradient::frame(cv::Mat& mask, std::size_t frame_index) {

  const int& width(mask.cols),& height(mask.rows);

  AreaSine sine(width, height, frame_index, m_shape);

  double frequency, amplitude, phase, tilt;

  set_area_frequency(frequency);

  amplitude = m_amplitude;
  phase     = m_phase;
  tilt      = m_tilt;

  double* ptr;
  for (int y = 0; y < height; y++) {

    ptr = mask.ptr<double>(y);
    for (int x = 0; x < width; x++) {

      if (frequency == 0) {
        ptr[x] = 1 * amplitude;
      } else {
        ptr[x] = math::normalize(-1, 1, sine.point(y, x, frequency, phase, tilt) * amplitude);
      }

    }
  }

  return mask;
}

void Gradient::process(cv::Mat& mask, std::size_t frame_index) {

  const std::size_t& width(mask.cols),& height(mask.rows);

  AreaSine sine(width, height, frame_index, m_shape);

  unsigned char filter_select = 0;

  if (m_filter == "frequency") filter_select = 1;
  if (m_filter == "amplitude") filter_select = 2;
  if (m_filter == "phase") filter_select = 3;
  if (m_filter == "tilt") filter_select = 4;

  double frequency, amplitude, phase, tilt;

  set_area_frequency(frequency);

  amplitude = m_amplitude;
  phase     = m_phase;
  tilt      = m_tilt;

  double* ptr;

  for (std::size_t y = 0; y < height; y++) {

    ptr = mask.ptr<double>(y);
    for (std::size_t x = 0; x < width; x++) {

      if (filter_select == 1) frequency = math::project(m_freq_min, m_freq_max, ptr[x]);
      if (filter_select == 2) amplitude = ptr[x];
      if (filter_select == 3) phase     = math::project(m_phase_min, m_phase_max, ptr[x]);
      if (filter_select == 4) tilt      = ptr[x];

      if (frequency == 0) {
        ptr[x] = 1 * amplitude;
      } else {
        ptr[x] =  math::normalize(-1, 1, sine.point(y, x, frequency, phase, tilt) * amplitude);
      }

    }
  }

}
