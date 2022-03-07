
#include "audiosine.h"

AudioSine::AudioSine(const std::size_t& width, const std::size_t& height, const std::size_t& frame_index, const double& frequency, double phase, const double& tilt, const std::string& shape)
  : m_width(width), m_height(height), m_frame_index(frame_index), m_frequency(frequency), m_phase(phase), m_tilt(tilt), m_shape(shape) {

}

void AudioSine::frame_phase() {

  double multiplier{1};

  for (std::size_t i = 0; i < m_frame_index; i++) {
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

double AudioSine::point(std::size_t& y, std::size_t& x) {

  double
    // freguency
    f{m_frequency * 360},
    // phase
    p{m_phase * 360},
    // m_tilt
    t{1 - m_tilt},
    // width
    w{static_cast<double>(m_width - 1)},
    // height
    h{static_cast<double>(m_height - 1)},

    rp, ra, ya, xa, ry, yv, xv, degrees, sinus;

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
