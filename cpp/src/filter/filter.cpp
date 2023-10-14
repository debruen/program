
#include "filter.h"

Filter::Filter() {

} // constructor END

nlohmann::json Filter::base_world(std::string type) {

  nlohmann::json data;

  if (type == "audio") {

    data["label"]   = "waveform";
    data["select"]  = "select";  // select, range, files, color, sine
    data["options"] = {"Sine", "Square", "Triangle", "Sawtooth"};
    data["default"] = data["options"][0];
    data["value"]   = data["default"];

  } else {

    data["label"]   = "color space";
    data["select"]  = "select";  // select, range, files, color, sine
    data["options"] = {"RGB", "HSL", "Lab", "LCh"};
    data["default"] = data["options"][0];
    data["value"]   = data["default"];

  }

  return data;
}

nlohmann::json Filter::init(std::string type) {

  m_type = type;
  /// basic filter options for all filter types

  /// 0: filter type / image and audio
  nlohmann::json filter_type;
  filter_type["label"]   = "type";
  filter_type["select"]  = "select";  // select, range, files, color, sine
  filter_type["options"] = {"fill", "random", "gradient", "rectangle"};
  filter_type["default"] = filter_type["options"][0];
  filter_type["value"]   = filter_type["default"];

  m_data.push_back(filter_type);

  /// 1: blend mode / image and audio
  nlohmann::json blend;
  blend["label"]   = "blend";
  blend["select"]  = "select";  // select, range, files, color, sine
  blend["options"] = {"normal", "arithmetic", "geometric", "harmonic", "darken", "multiply", "colorburn", "linearburn", "lighten", "screen", "colordodge", "lineardodge", "overlay", "softlight", "hardlight", "vividlight", "linearlight", "pinlight", "hardmix", "difference", "exclusion"};
  blend["default"] = blend["options"][0];
  blend["value"]   = blend["default"];

  m_data.push_back(blend);

  /// 2: opacity / image and audio
  nlohmann::json opacity;
  opacity["label"]   = "opacity";
  opacity["select"]  = "range";  // select, range, files, color, sine
  opacity["type"]    = "float";
  opacity["min"]     = 0.0;
  opacity["max"]     = 1.0;
  opacity["default"] = 0.5;
  opacity["value"]   = opacity["default"];

  m_data.push_back(opacity);

  /// 3: world / color space for images, waveform for audio
  m_data.push_back(base_world(type));

  return m_data;
} // init END

nlohmann::json Filter::set_data(nlohmann::json data, std::string type) {

  m_data[0] = data[0];
  m_data[1] = data[1];
  m_data[2] = data[2];
  if (type != m_type) {
    m_data[3] = base_world(type);
  } else {
    m_data[3] = data[3];
  }

  m_type = type;

  return m_data;
} // set_data END


std::string Filter::get_type(nlohmann::json data) {

  std::string type;

  if (data[3]["label"] == "waveform") {
    type = "audio";
  } else {
    type = "image";
  }

  return type;
}

double Filter::project(double min, double max, double v) {                        // from normalized to range
    return (v * (max - min)) + min;
}

unsigned char Filter::projectChar(unsigned char min, unsigned char max, double v) { // from normalized to range

  return round((v * (static_cast<double>(max) - static_cast<double>(min))) + static_cast<double>(min));
}

double Filter::circle(double min, double max, double value) {
  if(value > max) {
    value = value - max;
  }
  if(value < min) {
    value = value + max;
  }
  if(value > max || value < min) {
    circle(min, max, value);
  }

  return value;
}

double Filter::radian(double degree) {
  degree = circle(0, 360, degree);

  return degree * M_PI / 180;
}

double Filter::normDouble(double min, double max, double value) {
  return (value - min) / (max - min);
}

double Filter::point_gradient(unsigned int y, unsigned int x, unsigned int width, unsigned int height, double frequency, double phase, double tilt) {

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
  sinus = sin(radian(degrees));

  return normDouble(-1, 1, sinus);
} // point_gradient
