
#include "gradient.h"

Gradient::Gradient() {

  m_data = get_data();

  std::vector<std::string> shape_values{"sine", "saw", "square", "triangle"};
  m_data.push_back(data::init_str("shape", shape_values, m_shape));
  m_data.push_back(data::init_1d_float_data("frequency range", {m_freq_min, m_freq_max}));

  m_data.push_back(data::data_value("frequency gamma", m_frq_gamma));

  m_data.push_back(data::data_float("frequency", 0, 1, m_frequency));

  m_data.push_back(data::data_float("amplitude", 0, 1, m_amplitude));

  m_data.push_back(data::data_float("phase", m_phase_min, m_phase_max, m_phase));

  m_data.push_back(data::data_float("tilt", 0, 1, m_tilt));

  std::vector<std::string> filter_options{"none", "frequency", "amplitude", "phase", "tilt"};
  m_data.push_back(data::init_str("filter", filter_options, m_filter));

}

double Gradient::frame_phase(std::size_t index) {

  double phase = m_phase, multiplier;

  for (std::size_t i = 0; i < index; i++) {
    if(m_tilt <= 0.25) {
      multiplier = 1 - m_tilt * 4;
    } else if (m_tilt <= 0.5) {
      multiplier = (m_tilt - 0.25) * (-4);
    } else if (m_tilt <= 0.75) {
      multiplier = 1 - (m_tilt - 0.5) * 4;
    } else {
      multiplier = (m_tilt - 0.75) * (-4);
    }
    phase += m_frequency * multiplier;
  }

//  phase = math::circle(m_phase_min, m_phase_max, phase);

  return phase;
}

double Gradient::discrete(std::size_t& y, std::size_t& x, double& frequency, double& phase, double& tilt) {

  static double prev_shape;

  double f, p, t, w, h, rp, ra, ya, xa, ry, yv, xv, degrees, sinus;

  f = frequency * 360;
  p = phase * 360;
  t = 1 - tilt;

  w = static_cast<double>(m_width - 1);
  h = static_cast<double>(m_height - 1);

  if(t <= 0.25) {
    ya = 1 - t * 4;
    xa = t * 4;
    rp = 0;
  } else if (t <= 0.5) {
    ra = t - 0.25;
    ya = ra * 4;
    xa = 1 - ra * 4;
    rp = 0;
  } else if (t <= 0.75) {
    ra = t - 0.5;
    ya = 1 - ra * 4;
    xa = ra * 4;
    rp = 180;
  } else {
    ra = t - 0.75;
    ya = ra * 4;
    xa = 1 - ra * 4;
    rp = 180;
  }

  if(t <= 0.25) {
    ry = y;          // √
  } else if (t <= 0.5) {
    ry = h - y;
  } else if (t <= 0.75) {
    ry = y;
  } else {
    ry = h - y;
  }

  yv = (ry / h * ya);
  xv = (x / w * xa);

  // pattern here
  // to part of the function

  degrees = ((xv + yv) * f) + p + rp;

  sinus = sin(math::radian(degrees));

  if (m_shape == "square") {
    if (sinus >= 0)
      sinus = 1;
    else
      sinus = -1;
  }

  if (m_shape == "triangle")
    sinus = acos(sinus) / M_PI_2 - 1;

  if (m_shape == "saw") {

    double sin_a, sin_b;

    sinus = acos(sinus) / M_PI_2 - 1;

    if(sinus <= prev_shape) {
      sin_a = sinus / 2 - 0.5;
      sin_b = sinus * (-1) / 2 + 0.5;
    } else {
      sin_a = sinus * (-1) / 2 + 0.5;
      sin_b = sinus / 2 - 0.5;
    }

    if (m_tilt == 0 && x == m_width - 1) {
      prev_shape = sinus;
      sinus = sin_a;
    } else if (m_tilt == 1 && x == m_width - 1) {
      prev_shape = sinus;
      sinus = sin_a;
    } else if (m_tilt == 0.5 && x == m_width - 1) {
      prev_shape = sinus;
      sinus = sin_b;
    } else if (m_tilt > 0 && m_tilt < 0.5) {
      prev_shape = sinus;
      sinus = sin_b;
    } else if (m_tilt > 0.5 && m_tilt < 1) {
      prev_shape = sinus;
      sinus = sin_a;
    } else {
      sinus = sin_a;
    }

  }
  return math::normalize(m_sine_min, m_sine_max, sinus);
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

cv::Mat Gradient::frame(cv::Mat& mask, std::size_t index) {

  m_width = mask.cols;
  m_height = mask.rows;

  double frequency, amplitude, phase, tilt;

  frequency = pow(m_frequency, m_frq_gamma);
  frequency = math::project(m_freq_min, m_freq_max, frequency);

  amplitude = m_amplitude;
  phase     = frame_phase(index);
  tilt      = m_tilt;

  std::cout << "frequency: " << frequency << '\n';
  double* ptr;

  for (std::size_t y = 0; y < m_height; y++) {

    ptr = mask.ptr<double>(y);
    for (std::size_t x = 0; x < m_width; x++) {

      if (frequency == 0) {
        ptr[x] = 1 * amplitude;
      } else {
        ptr[x] = discrete(y, x, frequency, phase, tilt) * amplitude;
      }

    }
  }

  return mask;
}

void Gradient::process(cv::Mat& mask, std::size_t index) {

  m_width = mask.cols;
  m_height = mask.rows;

  unsigned char filter_select = 0;

  if (m_filter == "frequency") filter_select = 1;
  if (m_filter == "amplitude") filter_select = 2;
  if (m_filter == "phase") filter_select = 3;
  if (m_filter == "tilt") filter_select = 4;

  double frequency, amplitude, phase, tilt;

  frequency = pow(m_frequency, m_frq_gamma);
  frequency = math::project(m_freq_min, m_freq_max, frequency);

  amplitude = m_amplitude;
  phase     = frame_phase(index);
  tilt      = m_tilt;

  double* ptr;

  for (std::size_t y = 0; y < m_height; y++) {

    ptr = mask.ptr<double>(y);
    for (std::size_t x = 0; x < m_width; x++) {

      if (filter_select == 1) frequency = math::project(m_freq_min, m_freq_max, ptr[x]);
      if (filter_select == 2) amplitude = ptr[x];
      if (filter_select == 3) phase     = math::project(m_phase_min, m_phase_max, ptr[x]);
      if (filter_select == 4) tilt      = ptr[x];

      if (frequency == 0) {
        ptr[x] = 1 * amplitude;
      } else {
        ptr[x] = discrete(y, x, frequency, phase, tilt) * amplitude;
      }

    }
  }

}
