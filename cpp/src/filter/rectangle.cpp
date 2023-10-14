
#include "rectangle.h"

Rectangle::Rectangle(std::string type) {

  rectangle_data = init(type);

  rectangle_data[0]["value"] = "rectangle";

  rectangle_data.push_back(get_fillA(type));
  rectangle_data.push_back(get_fillB(type));

  /// 6: frequency
  nlohmann::json frequency;
  frequency["label"] = "frequency";
  frequency["select"] = "range";  // select, range, files
  frequency["type"] = "float";
  frequency["min"] = 0;
  frequency["max"] = 100;
  frequency["default"] = 1;
  frequency["value"] = frequency["default"];

  rectangle_data.push_back(frequency);

  /// 7: phase
  nlohmann::json phase;
  phase["label"] = "phase";
  phase["select"] = "range";  // select, range, files
  phase["type"] = "float";
  phase["min"] = 0;
  phase["max"] = 1;
  phase["default"] = 0.5;
  phase["value"] = phase["default"];

  rectangle_data.push_back(phase);

  /// 8: width
  nlohmann::json width;
  width["label"] = "width";
  width["select"] = "range";  // select, range, files
  width["type"] = "float";
  width["min"] = 0;
  width["max"] = 1;
  width["default"] = 0.5;
  width["value"] = width["default"];

  rectangle_data.push_back(width);

  /// 9: height
  nlohmann::json height;
  height["label"] = "height";
  height["select"] = "range";  // select, range, files, color, sine
  height["type"] = "float";
  height["min"] = 0;
  height["max"] = 1;
  height["default"] = 0.5;
  height["value"] = height["default"];

  rectangle_data.push_back(height);

  /// 10: center
  nlohmann::json center;
  center["label"] = "center";
  center["select"] = "range";  // select, range, files, color, sine
  center["type"] = "float";
  center["min"] = 0;
  center["max"] = 1;
  center["default"] = 0.5;
  center["value"] = center["default"];

  rectangle_data.push_back(center);

} // constructor END

nlohmann::json Rectangle::get_fillA(std::string type) {

  nlohmann::json data;

  if (type == "image") {

    /// 4: color / image only
    data["label"]  = "colorA";
    data["select"]  = "color";  // select, range, files, color, sine
    data["min"] = 0;
    data["max"] = 1;
    data["default"] = data["min"];
    data["a"] = data["default"];
    data["b"] = data["default"];
    data["c"] = data["default"];

  } else {

    /// 4: sine A / audio only
    data["label"]  = "sineA";
    data["select"]  = "sine";  // select, range, files, color, sine
    data["min"] = 0;
    data["max"] = 1;
    data["default"] = data["min"];
    data["a"] = data["default"];
    data["b"] = 0.5;
    data["c"] = 0.5;

  }

  return data;
}

nlohmann::json Rectangle::get_fillB(std::string type) {

  nlohmann::json data;

  if (type == "image") {

    /// 4: color / image only
    data["label"]  = "colorB";
    data["select"]  = "color";  // select, range, files, color, sine
    data["min"] = 0;
    data["max"] = 1;
    data["default"] = data["max"];
    data["a"] = data["default"];
    data["b"] = data["default"];
    data["c"] = data["default"];

  } else {

    /// 4: sine A / audio only
    data["label"]  = "sineB";
    data["select"]  = "sine";  // select, range, files, color, sine
    data["min"] = 0;
    data["max"] = 1;
    data["default"] = data["max"];
    data["a"] = data["default"];
    data["b"] = 0.5;
    data["c"] = 0.5;

  }

  return data;
}

nlohmann::json Rectangle::data(std::string type) {

  return rectangle_data;
} // data() END

nlohmann::json Rectangle::data(nlohmann::json data, std::string type) {

  std::string old_type = get_type(data);

  rectangle_data = set_data(data, type);

  if (type != old_type) {
    rectangle_data.push_back(get_fillA(type));
    rectangle_data.push_back(get_fillB(type));
  } else {
    rectangle_data.push_back(data[4]);
    rectangle_data.push_back(data[5]);
  }
  rectangle_data.push_back(data[6]);
  rectangle_data.push_back(data[7]);
  rectangle_data.push_back(data[8]);
  rectangle_data.push_back(data[9]);
  rectangle_data.push_back(data[10]);

  return rectangle_data;
} // data(data) END

void Rectangle::process(std::vector<cv::Mat>& images) {

  blendchar blend;
  double opacity;
  std::vector<unsigned char> rgbA, rgbB;

  blend   = eval::get_blend(rectangle_data, "blend");
  opacity = eval::data_float(rectangle_data, "opacity");
  rgbA    = eval::data_rgb(rectangle_data, "colorA");
  rgbB    = eval::data_rgb(rectangle_data, "colorB");

  double value;
  unsigned int w, wr, left, right, width, height;
  uchar r, g, b;

  double frequency, phase, ww, hh, center;

  frequency = eval::data_float(rectangle_data, "frequency");
  phase = eval::data_float(rectangle_data, "phase");
  center = eval::data_float(rectangle_data, "center");
  ww = eval::data_float(rectangle_data, "width");
  hh = eval::data_float(rectangle_data, "height");

  width = static_cast<unsigned int>(images[0].cols);
  height = static_cast<unsigned int>(images[0].rows);

  w = static_cast<double>(width) - 1;
  wr = w * ww;
  left = round((w - wr) * center);
  right = round(wr + left);

  unsigned int c;
  uchar* ptr;

  for (unsigned int i = 0; i < images.size(); i++) {

    // for (unsigned int j = 0; j < i; j++) {
    //   phase += frequency;
    // }

    phase += frequency;

    for (unsigned int y = 0; y < height; y++) {
      ptr = images[i].ptr<uchar>(y);
        for (unsigned int x = 0; x < width; x++) {
          c = x * 3;

          value = point_gradient(y, x, width, height, frequency, phase, 0);

          if(value < hh && x >= left && x <= right) {
            r = rgbA[0];
            g = rgbA[1];
            b = rgbA[2];
          } else {
            r = rgbB[0];
            g = rgbB[1];
            b = rgbB[2];
          }

          ptr[c + 2] = blend(ptr[c + 2], r, opacity);
          ptr[c + 1] = blend(ptr[c + 1], g, opacity);
          ptr[c] = blend(ptr[c], b, opacity);
        }
      }
    }
} // process(images) END

void Rectangle::process(stk::StkFrames& audio) {

  blendfloat blend;
  double opacity, frequency, phase, center, height, width, value, w, wr;
  std::size_t frames, channels, left, right;
  stk::StkFrames signalA, signalB;
  stk::StkFloat tick;

  blend    = eval::get_blendf(rectangle_data, "blend");
  opacity  = eval::data_float(rectangle_data, "opacity");
  frames   = audio.frames();
  channels = audio.channels();
  signalA  = eval::data_signal(rectangle_data, "sineA", frames);
  signalB  = eval::data_signal(rectangle_data, "sineB", frames);

  frequency = eval::data_float(rectangle_data, "frequency");
  phase     = eval::data_float(rectangle_data, "phase");
  center    = eval::data_float(rectangle_data, "center");
  width     = eval::data_float(rectangle_data, "width");
  height    = eval::data_float(rectangle_data, "height");

  stk::StkFrames framesL(frames, 1), framesR(frames, 1);
  framesL.setChannel(0, audio, 0);
  framesR.setChannel(0, audio, 1);

  w = static_cast<double>(channels) - 1;
  wr = w * width;
  left = round((w - wr) * center);
  right = round(wr + left);

  phase += frequency;

  for (std::size_t y = 0; y < frames; y++) {

    for (std::size_t x = 0; x < channels; x++) {

      value = point_gradient(y, x, channels, frames, frequency, phase, 0);

      if(value < height && x >= left && x <= right)
        tick = signalA[y];
      else
        tick = signalB[y];

      if (x == 0)
        framesL[y] = blend(framesL[y], tick, opacity);
      else
        framesR[y] = blend(framesR[y], tick, opacity);
    }
  }

  audio.setChannel(0, framesL, 0);
  audio.setChannel(1, framesR, 0);

} // process(audio, images) END
