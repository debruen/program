
#include "sine.h"

Sine::Sine(int& width, int& height, std::size_t& index, const std::string& shape, double frequency, double phase, double tilt, std::string type)
    : m_width{width}, m_height{height}, m_index{index}, m_shape{shape}, m_proto_frequency{frequency}, m_proto_phase(phase), m_tilt(1-tilt), m_type(type) {

  adjust_phase();
  adjust_member();
}

void Sine::adjust_phase() {

  std::cout << "SINE freq: " << m_proto_frequency << '\n';

  m_frequency = m_proto_frequency * 360.0;

  double multiplier{1}, phase = m_proto_phase, tilt = 1 - m_tilt;

  if(tilt <= 0.25) {
    std::cout << "A" << '\n';
    multiplier = 1 - tilt * 4;
  } else if (tilt <= 0.5) {
    std::cout << "B" << '\n';
    multiplier = (tilt - 0.25) * (-4);
  } else if (tilt <= 0.75) {
    std::cout << "C" << '\n';
    multiplier = 1 - (tilt - 0.5) * 4;
  } else {
    std::cout << "D" << '\n';
    multiplier = (tilt - 0.75) * (-4);
  }

  for (std::size_t i = 0; i < m_index; i++) {
    phase += m_frequency * multiplier;
  }

  std::cout.precision(17);
  std::cout << "sine index: " << m_index << '\n';
  std::cout << "sine tilt: " << tilt << '\n';
  std::cout << "sine frequency AP B: " << m_frequency << '\n';
  std::cout << "sine multiplier: " << multiplier << '\n';
  std::cout << "sine proto phase: " << m_proto_phase << '\n';
  std::cout << "sine phase: " << phase << '\n';

  if (m_type == "audio") {
    m_phase = phase * -1;
  } else {
    m_phase = phase;
  }

}

void Sine::adjust_member() {

  std::cout << "sine frequency AM A: " << m_frequency << '\n';

  m_sine_width = static_cast<double>(m_width - 1);
  m_sine_height = static_cast<double>(m_height - 1);

  double adjusted_tilt;

  if(m_tilt <= 0.25) {
    m_y_multi = 1 - m_tilt * 4;
    m_x_multi = m_tilt * 4;
    m_tilt_add_angle = 0;
  } else if (m_tilt <= 0.5) {
    adjusted_tilt = m_tilt - 0.25;
    m_y_multi = adjusted_tilt * 4;
    m_x_multi = 1 - adjusted_tilt * 4;
    m_tilt_add_angle = 0;
  } else if (m_tilt <= 0.75) {
    adjusted_tilt = m_tilt - 0.5;
    m_y_multi = 1 - adjusted_tilt * 4;
    m_x_multi = adjusted_tilt * 4;
    m_tilt_add_angle = 180;
  } else {
    adjusted_tilt = m_tilt - 0.75;
    m_y_multi = adjusted_tilt * 4;
    m_x_multi = 1 - adjusted_tilt * 4;
    m_tilt_add_angle = 180;
  }

}

void Sine::set_frequency(double frequency) {
  m_proto_frequency = frequency;
  adjust_phase();
}

void Sine::set_phase(double phase) {
  m_proto_phase = phase;
  adjust_phase();
}

void Sine::set_tilt(double tilt) {
  m_tilt = 1 - tilt;
  adjust_phase();
  adjust_member();
}

double Sine::point(int& y, int& x) {

  double y_frame, x_frame;

  y_frame = y;
  x_frame = x;

  double adjusted_y, yv, xv, degrees, sinus;

  if(m_tilt <= 0.25) {
    adjusted_y = y_frame;          // √
  } else if (m_tilt <= 0.5) {
    adjusted_y = m_sine_height - y_frame;
  } else if (m_tilt <= 0.75) {
    adjusted_y = y_frame;
  } else {
    adjusted_y = m_sine_height - y_frame;
  }

  yv = (adjusted_y / m_sine_height * m_y_multi);
  xv = (x_frame / m_sine_width * m_x_multi);

  degrees = ((xv + yv) * m_frequency) + m_phase + m_tilt_add_angle;

  // std::cout << "degrees" << degrees << '\n';

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

    if(sinus <= m_shape_note) {
      sin_a = sinus / 2 - 0.5;
      sin_b = sinus * (-1) / 2 + 0.5;
    } else {
      sin_a = sinus * (-1) / 2 + 0.5;
      sin_b = sinus / 2 - 0.5;
    }

    if (m_tilt == 0 && x == m_width - 1) {
      m_shape_note = sinus;
      sinus = sin_a;
    } else if (m_tilt == 1 && x == m_width - 1) {
      m_shape_note = sinus;
      sinus = sin_a;
    } else if (m_tilt == 0.5 && x == m_width - 1) {
      m_shape_note = sinus;
      sinus = sin_b;
    } else if (m_tilt > 0 && m_tilt < 0.5) {
      m_shape_note = sinus;
      sinus = sin_b;
    } else if (m_tilt > 0.5 && m_tilt < 1) {
      m_shape_note = sinus;
      sinus = sin_a;
    } else {
      sinus = sin_a;
    }

  }

  return sinus;
}
