
#include "audiosine.h"

AudioSine::AudioSine(int& width, int& height, std::size_t& index, const double& frequency, const double& tilt, const std::string& shape, double phase)
  : m_width{width}, m_height{height}, m_index{index}, m_frequency(frequency * 360.0), m_tilt(tilt), m_point_tilt(1-tilt), m_shape(shape), m_phase(phase) {

  frame_phase();
}

void AudioSine::frame_phase() {

  double multiplier{1};

  for (std::size_t i = 0; i < m_index; i++) {
    if(m_tilt <= 0.25) {
      multiplier = 1 - m_tilt * 4;
    } else if (m_tilt <= 0.5) {
      multiplier = (m_tilt - 0.25) * (-4);
    } else if (m_tilt <= 0.75) {
      multiplier = 1 - (m_tilt - 0.5) * 4;
    } else {
      multiplier = (m_tilt - 0.75) * (-4);
    }
    m_phase += m_frequency * multiplier;
  }

}

double AudioSine::point(int& y, int& x) {

  double
    // width
    x_max{static_cast<double>(m_width - 1)},
    // height
    y_max{static_cast<double>(m_height - 1)},

    rp, ra, ya, xa, ry, yv, xv, degrees, sinus;

  if(m_point_tilt <= 0.25) {
    ya = 1 - m_point_tilt * 4;
    xa = m_point_tilt * 4;
    rp = 0;
  } else if (m_point_tilt <= 0.5) {
    ra = m_point_tilt - 0.25;
    ya = ra * 4;
    xa = 1 - ra * 4;
    rp = 0;
  } else if (m_point_tilt <= 0.75) {
    ra = m_point_tilt - 0.5;
    ya = 1 - ra * 4;
    xa = ra * 4;
    rp = 180;
  } else {
    ra = m_point_tilt - 0.75;
    ya = ra * 4;
    xa = 1 - ra * 4;
    rp = 180;
  }

  if(m_point_tilt <= 0.25) {
    ry = y;          // √
  } else if (m_point_tilt <= 0.5) {
    ry = y_max - y;
  } else if (m_point_tilt <= 0.75) {
    ry = y;
  } else {
    ry = y_max - y;
  }

  yv = (ry / y_max * ya);
  xv = (x / x_max * xa);

  degrees = ((xv + yv) * m_frequency) + m_phase + rp;

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

    if (m_point_tilt == 0 && x == m_width - 1) {
      m_shape_note = sinus;
      sinus = sin_a;
    } else if (m_point_tilt == 1 && x == m_width - 1) {
      m_shape_note = sinus;
      sinus = sin_a;
    } else if (m_point_tilt == 0.5 && x == m_width - 1) {
      m_shape_note = sinus;
      sinus = sin_b;
    } else if (m_point_tilt > 0 && m_point_tilt < 0.5) {
      m_shape_note = sinus;
      sinus = sin_b;
    } else if (m_point_tilt > 0.5 && m_point_tilt < 1) {
      m_shape_note = sinus;
      sinus = sin_a;
    } else {
      sinus = sin_a;
    }

  }

  return sinus;
}
