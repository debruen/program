
#include "sine.h"

Sine::Sine(int& width, int& height, std::size_t& index, const std::string& shape, double frequency, double phase, double tilt, std::string type)
    : m_width{width}, m_height{height}, m_index{index}, m_shape{shape}, m_proto_frequency{frequency}, m_proto_phase(phase), m_proto_tilt(tilt), m_type(type) {

  adjust_basics();
  adjust_tilt();
}


void Sine::adjust_basics() {
  double multiplier{1}, phase = m_proto_phase;

  if(m_proto_tilt <= 0.25) {
    multiplier = 1 - m_proto_tilt * 4;
  } else if (m_proto_tilt <= 0.5) {
    multiplier = (m_proto_tilt - 0.25) * (-4);
  } else if (m_proto_tilt <= 0.75) {
    multiplier = 1 - (m_proto_tilt - 0.5) * 4;
  } else {
    multiplier = (m_proto_tilt - 0.75) * (-4);
  }

  for (std::size_t i = 0; i < m_index; i++) {
    phase += m_proto_frequency * multiplier;
  }

  if (m_type == "audio") {
    phase = phase * -1;
  }

  m_frequency = m_proto_frequency * 360;
  m_phase = phase * 360;
  m_tilt = 1 - m_proto_tilt;

}

void Sine::adjust_tilt() {
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
  adjust_basics();
}

void Sine::set_phase(double phase) {
  m_proto_phase = phase;
  adjust_basics();
}

void Sine::set_tilt(double tilt) {
  adjust_basics();
  adjust_tilt();
}

double Sine::point(int& y, int& x) {
  double y_frame, x_frame;

  y_frame = y;
  x_frame = x;

  double adjusted_y, yv, xv, degrees, sinus;
  if(m_tilt <= 0.25) {
    adjusted_y = y_frame;
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

  // if(y == 0 && x == 0) {
  //   std::cout << "phase 3: " << m_phase << '\n';
  //   std::cout << "degrees: " << degrees << '\n';
  // }

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

double Sine::point_gradient(unsigned int y, unsigned int x, unsigned int width, unsigned int height, double frequency, double phase, double tilt) {

    double r, p, a, w, h, rp, ra, ya, xa, ry, yv, xv, degrees, sinus;

    w = static_cast<double>(width) - 1;
    h = static_cast<double>(height) - 1;

    r = frequency * 360;
    p  = phase * 360;
    a = 1 - tilt;

    if(a <= 0.25) {
        ya = 1 - a * 4;
        xa = a * 4;
        rp = 0;
    } else if (a <= 0.5) {
        ra = a - 0.25;
        ya = ra * 4;
        xa = 1 - ra * 4;
        rp = 0;
    } else if (a <= 0.75) {
        ra = a - 0.5;
        ya = 1 - ra * 4;
        xa = ra * 4;
        rp = 180;
    } else {
        ra = a - 0.75;
        ya = ra * 4;
        xa = 1 - ra * 4;
        rp = 180;
    }

    if(a <= 0.25) {
        ry = y;          // √
    } else if (a <= 0.5) {
        ry = h - y;
    } else if (a <= 0.75) {
        ry = y;
    } else {
        ry = h - y;
    }

    yv = (ry / h * ya);
    xv = (x / w * xa);

    degrees = ((xv + yv) * r) + p + rp;
    sinus = sin(math::radian(degrees));

    // return wtc::number::normDouble(-1, 1, sinus);
    return sinus;
} // point_gradient
