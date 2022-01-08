
#include "data.h"

/// returns 0 if the objectt is empty and supposed to be an array
/// value: nlohmann::json object

std::size_t data::size(json data) {
  std::size_t size = 0;

  if (data.size())
    size = data.size();

  return size;
}

/// a json array of objects
/// value 1: nlohmann::json object
/// the object label to search for
/// value 2: std::string

double data::value_dbl(json data, std::string label) {
  std::size_t size = size(data);

  double value = 0.0;

  for (std::size_t i = 0; i < size; i++) {
    if (data[i]["label"] == label) {
      value = data[i]["value"];
      break;
    }
  }

  return value;
} // value_dbl END


int data::value_int(json data, std::string label) {
  std::size_t size = size(data);

  int value = 0;

  for (std::size_t i = 0; i < size; i++) {
    if (data[i]["label"] == label) {
      value = data[i]["value"];
      break;
    }
  }

  return value;
} // value_int END


std::string data::value_str(json data, std::string label) {
  std::size_t size = size(data);

  std::string value = "";

  for (std::size_t i = 0; i < size; i++) {
    if (data[i]["label"] == label) {
      value = data[i]["value"];
      break;
    }
  }

  return value;
} // value_str END


std::vector<unsigned char> data::value_rgb(json data, std::string label) {

  std::string color_space = value_str(data, "color space");
  std::vector<unsigned char> rgb;

  double channel_a, channel_b, channel_c;

  std::size_t size = size(data);

  for (std::size_t i = 0; i < size; i++) {
    if (data[i]["label"] == label) {
      channel_a = data[i]["a"];
      channel_b = data[i]["b"];
      channel_c = data[i]["c"];
    }
  }

  if (color_space == "RGB") {
    rgb.push_back(round(channel_a * 255));
    rgb.push_back(round(channel_b * 255));
    rgb.push_back(round(channel_c * 255));
  }

  if (color_space == "HSL") {
    rgb = color::hslToRgb({channel_a, channel_b, channel_c});
  }

  if (color_space == "Lab") {
    unsigned char lab_a, lab_b, lab_c;
    lab_a = round(channel_a * 255);
    lab_b = round(channel_b * 255);
    lab_c = round(channel_c * 255);

    cv::Mat lab_image(1,1, CV_8UC3, cv::Scalar(lab_a, lab_b, lab_c));
    cv::Mat rgb_image;
    cv::cvtColor(lab_image, rgb_image, cv::COLOR_Lab2BGR);

    cv::Vec3b rgb_ocv = rgb_image.at<cv::Vec3b>(0, 0);

    rgb.push_back(rgb_ocv[0]);
    rgb.push_back(rgb_ocv[1]);
    rgb.push_back(rgb_ocv[2]);
  }

  return rgb;
} // value_rgb END


stk::StkFrames data::value_sgl(json data, std::string label, std::size_t frames) {

  std::string waveform;
  stk::StkFrames signal(frames, 1);
  double a, b, c, freq_min, freq_max, phase_min, phase_max, tick;

  waveform = value_str(data, "waveform");

  freq_min = 16;
  freq_max = 21000;

  phase_min = -1;
  phase_max = 1;

  std::size_t size = size(data);

  for (std::size_t i = 0; i < size; i++) {
    if (data[i]["label"] == label) {
      a = data[i]["a"];
      b = data[i]["b"];
      c = data[i]["c"];
    }
  }

  a = pow(a, 6);

  a = a * (freq_max - freq_min) + freq_min;
  c = c * (phase_max - phase_min) + phase_min;

  stk::SineWave sine;

  sine.setFrequency(a);

  sine.addPhaseOffset(c);

  double temp_signal, prev;

  for(std::size_t i = 0; i < frames; ++i) {

    tick = sine.tick();

    if (waveform == "Square") {
      if (tick >= 0)
        tick = 1;
      else
        tick = -1;
    }

    if (waveform == "Triangle")
      tick = acos(tick) / M_PI_2 - 1;

    if (waveform == "Sawtooth") {
      tick = acos(tick) / M_PI_2 - 1;

      if(tick <= prev) {
        temp_signal = tick / 2 - 0.5;
      } else {
        temp_signal = tick * (-1) / 2 + 0.5;
      }
      prev = tick;
      tick = temp_signal;
    }

    tick = tick * b;

    signal[i] = tick;
  }

  return signal;
} // value_sgl END


blendchar data::int_blend(json data, std::string label) {

  std::string blend = value_str(data, label);

  blendchar function = blend::normal;

  if(blend == "normal") function = blend::normal;
  if(blend == "arithmetic") function = blend::arithmetic;
  if(blend == "geometric") function = blend::geometric;
  if(blend == "harmonic") function = blend::harmonic;
  if(blend == "darken") function = blend::darken;
  if(blend == "multiply") function = blend::multiply;
  if(blend == "colorburn") function = blend::colorburn;
  if(blend == "linearburn") function = blend::linearburn;
  if(blend == "lighten") function = blend::lighten;
  if(blend == "screen") function = blend::screen;
  if(blend == "colordodge") function = blend::colordodge;
  if(blend == "lineardodge") function = blend::lineardodge;
  if(blend == "overlay") function = blend::overlay;
  if(blend == "softlight") function = blend::softlight;
  if(blend == "hardlight") function = blend::hardlight;
  if(blend == "vividlight") function = blend::vividlight;
  if(blend == "linearlight") function = blend::linearlight;
  if(blend == "pinlight") function = blend::pinlight;
  if(blend == "hardmix") function = blend::hardmix;
  if(blend == "difference") function = blend::difference;
  if(blend == "exclusion") function = blend::exclusion;

  return function;
} // int_blend END


blendfloat data::dbl_blend(json data, std::string label) {

  std::string blend = value_str(data, label);

  blendfloat function = blend::normal;

  if(blend == "normal") function = blend::normal;
  if(blend == "arithmetic") function = blend::arithmetic;
  if(blend == "geometric") function = blend::geometric;
  if(blend == "harmonic") function = blend::harmonic;
  if(blend == "darken") function = blend::darken;
  if(blend == "multiply") function = blend::multiply;
  if(blend == "colorburn") function = blend::colorburn;
  if(blend == "linearburn") function = blend::linearburn;
  if(blend == "lighten") function = blend::lighten;
  if(blend == "screen") function = blend::screen;
  if(blend == "colordodge") function = blend::colordodge;
  if(blend == "lineardodge") function = blend::lineardodge;
  if(blend == "overlay") function = blend::overlay;
  if(blend == "softlight") function = blend::softlight;
  if(blend == "hardlight") function = blend::hardlight;
  if(blend == "vividlight") function = blend::vividlight;
  if(blend == "linearlight") function = blend::linearlight;
  if(blend == "pinlight") function = blend::pinlight;
  if(blend == "hardmix") function = blend::hardmix;
  if(blend == "difference") function = blend::difference;
  if(blend == "exclusion") function = blend::exclusion;

  return function;
} // dbl_blend END


std::vector<unsigned char> color::hslToRgb(std::vector<double> hsl) {  // H, s, l = 0.0 - 1.0 | R, G, B = 0 - 255
    unsigned char R, G, B;
    double a, b;

    auto hueToRgb = [](double v1, double v2, double vH) -> double { // useb by hslToRgb
        if(vH < 0) vH += 1;
        if(vH > 1) vH -= 1;

        if((6 * vH) < 1) return v1 + (v2 - v1) * 6 * vH;
        if((2 * vH) < 1) return v2;
        if((3 * vH) < 2) return v1 + (v2 - v1) * ((2 / 3) - vH) * 6;
        return v1;
    };

    if(hsl[1] == 0) { // no Chromatic data
        R = round(hsl[2] * 255.0);
        G = round(hsl[2] * 255.0);
        B = round(hsl[2] * 255.0);
    } else {         // Chromatic data
        if (hsl[2] < 0.5) {
            b = hsl[2] * (1 + hsl[1]);
        } else {
            b = (hsl[2] + hsl[1]) - (hsl[1] * hsl[2]);
        }

        a = 2 * hsl[2] - b;

        R = round(255.0 * hueToRgb(a, b, hsl[0] + (1 / 3)));
        G = round(255.0 * hueToRgb(a, b, hsl[0]));
        B = round(255.0 * hueToRgb(a, b, hsl[0] - (1 / 3)));
    }

    std::vector<unsigned char> rgb{R, G, B};
    return rgb;
}

double blend::normal(double& a, double& b, double& o) {
    return  a * (1 - o) + b * o;
}
unsigned char blend::normal(unsigned char& aChar, unsigned char& bChar, double& o) {
    double a = static_cast<double>(aChar) / 255, b = static_cast<double>(bChar) / 255, c;
    c = normal(a, b, o);
    return round(c * 255);
}

// mean

double blend::arithmetic(double& a, double& b, double& o) {

    double c = (a + b) / 2;

    return  a * (1 - o) + c * o;
}
unsigned char blend::arithmetic(unsigned char& aChar, unsigned char& bChar, double& o) {
    double a = static_cast<double>(aChar) / 255, b = static_cast<double>(bChar) / 255, c;
    c = arithmetic(a, b, o);
    return round(c * 255);
}

double blend::geometric(double& a, double& b, double& o) {

    bool test = false;

    double temp = a * b;

    if(temp < 0) test = true;

    double c = sqrt(fabs(temp));

    if (test) c *= -1;

    return a * (1 - o) + c * o;
}
unsigned char blend::geometric(unsigned char& aChar, unsigned char& bChar, double& o) {
    double a = static_cast<double>(aChar) / 255, b = static_cast<double>(bChar) / 255, c;
    c = geometric(a, b, o);
    return round(c * 255);
}

double blend::harmonic(double& a, double& b, double& o) {

    double c = 2 / ((1 / a) + (1 / b));

    if(c > 1) c = 1;
    if(c < -1) c = -1;
    return a * (1 - o) + c * o;
}
unsigned char blend::harmonic(unsigned char& aChar, unsigned char& bChar, double& o) {
    double a = static_cast<double>(aChar) / 255, b = static_cast<double>(bChar) / 255, c;
    c = harmonic(a, b, o);
    return round(c * 255);
}

// blend

double blend::darken(double& a, double& b, double& o) {
    double c;

    if(a < b) c = a;
    if(a > b) c = b;

    return a * (1 - o) + c * o;
}
unsigned char blend::darken(unsigned char& aChar, unsigned char& bChar, double& o) {
    double a = static_cast<double>(aChar) / 255, b = static_cast<double>(bChar) / 255, c;
    c = darken(a, b, o);
    return round(c * 255);
}

double blend::multiply(double& a, double& b, double& o) {
    double c = a * b;
    return a * (1 - o) + c * o;
}
unsigned char blend::multiply(unsigned char& aChar, unsigned char& bChar, double& o) {
    double a = static_cast<double>(aChar) / 255, b = static_cast<double>(bChar) / 255, c;
    c = multiply(a, b, o);
    return round(c * 255);
}

double blend::colorburn(double& a, double& b, double& o) {
    double c = 1 - (1 - b) / a;

    if(c < 0) c = 0;
    return a * (1 - o) + c * o;
}
unsigned char blend::colorburn(unsigned char& aChar, unsigned char& bChar, double& o) {
    double a = static_cast<double>(aChar) / 255, b = static_cast<double>(bChar) / 255, c;
    c = colorburn(a, b, o);
    return round(c * 255);
}

double blend::linearburn(double& a, double& b, double& o) {
    double c = a + b - 1;

    if(c < 0) c = 0;
    return a * (1 - o) + c * o;
}
unsigned char blend::linearburn(unsigned char& aChar, unsigned char& bChar, double& o) {
    double a = static_cast<double>(aChar) / 255, b = static_cast<double>(bChar) / 255, c;
    c = linearburn(a, b, o);
    return round(c * 255);
}


double blend::lighten(double& a, double& b, double& o) {
    double c;

    if(b < a) c = a;
    if(b > a) c = b;

    return a * (1 - o) + c * o;
}
unsigned char blend::lighten(unsigned char& aChar, unsigned char& bChar, double& o) {
    double a = static_cast<double>(aChar) / 255, b = static_cast<double>(bChar) / 255, c;
    c = lighten(a, b, o);
    return round(c * 255);
}

double blend::screen(double& a, double& b, double& o) {

    double c = 1 - (1 - a) * (1 - b);

    return a * (1 - o) + c * o;
}
unsigned char blend::screen(unsigned char& aChar, unsigned char& bChar, double& o) {
    double a = static_cast<double>(aChar) / 255, b = static_cast<double>(bChar) / 255, c;
    c = screen(a, b, o);
    return round(c * 255);
}

double blend::colordodge(double& a, double& b, double& o) {

    double c = a / (1 - b);

    if(c > 1) c = 1;
    return a * (1 - o) + c * o;
}
unsigned char blend::colordodge(unsigned char& aChar, unsigned char& bChar, double& o) {
    double a = static_cast<double>(aChar) / 255, b = static_cast<double>(bChar) / 255, c;
    c = colordodge(a, b, o);
    return round(c * 255);
}

double blend::lineardodge(double& a, double& b, double& o) {

    double c = a + b;

    if(c > 1) c = 1;
    return a * (1 - o) + c * o;
}
unsigned char blend::lineardodge(unsigned char& aChar, unsigned char& bChar, double& o) {
    double a = static_cast<double>(aChar) / 255, b = static_cast<double>(bChar) / 255, c;
    c = lineardodge(a, b, o);
    return round(c * 255);
}

double blend::overlay(double& a, double& b, double& o) {
    double c;

    if(a < 0.5) {
        c = 2 * a * b;
    } else {
        c = 1 - 2 * (1 - a) * (1 - b);
    }

    return a * (1 - o) + c * o;
}
unsigned char blend::overlay(unsigned char& aChar, unsigned char& bChar, double& o) {
    double a = static_cast<double>(aChar) / 255, b = static_cast<double>(bChar) / 255, c;
    c = overlay(a, b, o);
    return round(c * 255);
}

double blend::softlight(double& a, double& b, double& o) {
    double c;

    if(b < 0.5) {
        c = 2 * a * b + (pow(a, 2) * (1 - 2 * b));
    } else {
        c = 2 * a * (1 - b) + (sqrt(a) * (2 * b - 1));
    }

    return a * (1 - o) + c * o;
}
unsigned char blend::softlight(unsigned char& aChar, unsigned char& bChar, double& o) {
    double a = static_cast<double>(aChar) / 255, b = static_cast<double>(bChar) / 255, c;
    c = softlight(a, b, o);
    return round(c * 255);
}

double blend::hardlight(double& a, double& b, double& o) {
    double c;

    if(b < 0.5) {
        c = 2 * b * a;
    } else {
        c = 1 - 2 * (1 - b) * (1 - a);
    }

    return a * (1 - o) + c * o;
}
unsigned char blend::hardlight(unsigned char& aChar, unsigned char& bChar, double& o) {
    double a = static_cast<double>(aChar) / 255, b = static_cast<double>(bChar) / 255, c;
    c = hardlight(a, b, o);
    return round(c * 255);
}

double blend::vividlight(double& a, double& b, double& o) {
    double c;

    if(a <= 0.5) c = 1 - (1 - b) / (2 * a);
    if(a > 0.5) c = b / (2 * (1 - a));

    if(c < 0) c = 0;
    if(c > 1) c = 1;
    return a * (1 - o) + c * o;
}
unsigned char blend::vividlight(unsigned char& aChar, unsigned char& bChar, double& o) {
    double a = static_cast<double>(aChar) / 255, b = static_cast<double>(bChar) / 255, c;
    c = vividlight(a, b, o);
    return round(c * 255);
}

double blend::linearlight(double& a, double& b, double& o) {

    double c = b + 2 * a - 1;

    if(c < 0) c = 0;
    if(c > 1) c = 1;
    return a * (1 - o) + c * o;
}
unsigned char blend::linearlight(unsigned char& aChar, unsigned char& bChar, double& o) {
    double a = static_cast<double>(aChar) / 255, b = static_cast<double>(bChar) / 255, c;
    c = linearlight(a, b, o);
    return round(c * 255);
}

double blend::pinlight(double& a, double& b, double& o) {
    double c;

    if(b > 0.5) {
      if(a > 2 * (b - 0.5)) c = a;
      if(a < 2 * (b - 0.5)) c = 2 * (b - 0.5);
    } else {
      if(a < 2 * b) c = a;
      if(a > 2 * b) c = 2 * b;
    }

    return a * (1 - o) + c * o;
}
unsigned char blend::pinlight(unsigned char& aChar, unsigned char& bChar, double& o) {
    double a = static_cast<double>(aChar) / 255, b = static_cast<double>(bChar) / 255, c;
    c = pinlight(a, b, o);
    return round(c * 255);
}

double blend::hardmix(double& a, double& b, double& o) {
    double c;

    if(a < 1 - b) c = 0;
    if(a > 1 - b) c = 1;

    return a * (1 - o) + c * o;
}
unsigned char blend::hardmix(unsigned char& aChar, unsigned char& bChar, double& o) {
    double a = static_cast<double>(aChar) / 255, b = static_cast<double>(bChar) / 255, c;
    c = hardmix(a, b, o);
    return round(c * 255);
}

double blend::difference(double& a, double& b, double& o) {

    double c = fabs(a - b);

    return a * (1 - o) + c * o;
}
unsigned char blend::difference(unsigned char& aChar, unsigned char& bChar, double& o) {
    double a = static_cast<double>(aChar) / 255, b = static_cast<double>(bChar) / 255, c;
    c = difference(a, b, o);
    return round(c * 255);
}

double blend::exclusion(double& a, double& b, double& o) {

    double c = 0.5 - 2 * (a - 0.5) * (b - 0.5);

    return a * (1 - o) + c * o;
}
unsigned char blend::exclusion(unsigned char& aChar, unsigned char& bChar, double& o) {
    double a = static_cast<double>(aChar) / 255, b = static_cast<double>(bChar) / 255, c;
    c = exclusion(a, b, o);
    return round(c * 255);
}

data_color::data_color() {

  m_data["name"]     = "color";
  m_data["space"]    = nlohmann::json::object();
  m_data["space"]["form"]    = "select";
  m_data["space"]["label"]   = "color space";
  m_data["space"]["options"] = {"RGB", "HSL", "Lab", "LCh"};
  m_data["space"]["value"]   = "RGB";
  m_data["min"]      = 0.0;
  m_data["max"]      = 1.0;
  m_data["channelA"] = 0.5;
  m_data["channelB"] = 0.5;
  m_data["channelC"] = 0.5;

}

nlohmann::json data_color::data() {

  return m_data;
}
