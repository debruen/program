
#include "gradient.h"

Gradient::Gradient(std::string type) {

  gradient_data = init(type);

  gradient_data[0]["value"] = "gradient";

  gradient_data.push_back(get_fillA(type));
  gradient_data.push_back(get_fillB(type));

  /// 6: frequency
  nlohmann::json frequency;
  frequency["label"] = "frequency";
  frequency["select"] = "range";  // select, range, files
  frequency["type"] = "float";
  frequency["min"] = 0;
  frequency["max"] = 100;
  frequency["default"] = 1;
  frequency["value"] = frequency["default"];

  gradient_data.push_back(frequency);

  /// 7: phase
  nlohmann::json phase;
  phase["label"] = "phase";
  phase["select"] = "range";  // select, range, files
  phase["min"] = 0;
  phase["max"] = 1;
  phase["type"] = "float";
  phase["default"] = phase["min"];
  phase["value"] = phase["default"];

  gradient_data.push_back(phase);

  /// 8: tilt
  nlohmann::json tilt;
  tilt["label"] = "tilt";
  tilt["select"] = "range";  // select, range, files
  tilt["min"] = 0;
  tilt["max"] = 1;
  tilt["type"] = "float";
  tilt["default"] = tilt["min"];
  tilt["value"] = tilt["default"];

  gradient_data.push_back(tilt);

  // /// 9: pattern
  // nlohmann::json pattern;
  // pattern["label"] = "pattern";
  // pattern["select"] = "rule";  // select, range, files, rule
  // pattern["min"] = 0;
  // pattern["max"] = 1;
  // pattern["type"] = "float";
  // pattern["default"] = pattern["min"];
  // pattern["value"] = "iiii";
  //
  // gradient_data.push_back(pattern);

} // constructor END

nlohmann::json Gradient::get_fillA(std::string type) {

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

nlohmann::json Gradient::get_fillB(std::string type) {

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

nlohmann::json Gradient::data(std::string type) {

  return gradient_data;
} // data() END

nlohmann::json Gradient::data(nlohmann::json data, std::string type) {

  std::string old_type = get_type(data);

  gradient_data = set_data(data, type);

  if (type != old_type) {
    gradient_data.push_back(get_fillA(type));
    gradient_data.push_back(get_fillB(type));
  } else {
    gradient_data.push_back(data[4]);
    gradient_data.push_back(data[5]);
  }
  gradient_data.push_back(data[6]);
  gradient_data.push_back(data[7]);
  gradient_data.push_back(data[8]);

  return gradient_data;
} // data(data) END

void Gradient::process(std::vector<cv::Mat>& images) {

  blendchar blend;
  double opacity;
  std::vector<unsigned char> rgbA, rgbB;

  blend   = eval::get_blend(gradient_data, "blend");
  opacity = eval::data_float(gradient_data, "opacity");
  rgbA    = eval::data_rgb(gradient_data, "colorA");
  rgbB    = eval::data_rgb(gradient_data, "colorB");

  double value, pm;
  unsigned int width, height;
  uchar r, g, b;

  double frequency, phase, tilt;

  frequency = eval::data_float(gradient_data, "frequency");
  phase = eval::data_float(gradient_data, "phase");
  tilt = eval::data_float(gradient_data, "tilt");

  tilt = circle(0, 1, tilt);

    width = static_cast<unsigned int>(images[0].cols);
    height = static_cast<unsigned int>(images[0].rows);

    unsigned int c;
    uchar* ptr;

    for (unsigned int i = 0; i < images.size(); i++) {

      for (unsigned int j = 0; j < i; j++) {
        if(tilt <= 0.25) {
            pm = 1 - tilt * 4;
        } else if (tilt <= 0.5) {
            pm = (tilt - 0.25) * (-4);
        } else if (tilt <= 0.75) {
            pm = 1 - (tilt - 0.5) * 4;
        } else {
            pm = (tilt - 0.75) * (-4);
        }

      }

      phase += frequency * pm;

      for (unsigned int y = 0; y < height; y++) {

        ptr = images[i].ptr<uchar>(y);
        for (unsigned int x = 0; x < width; x++) {
          c = x * 3;

          value = point_gradient(y, x, width, height, frequency, phase, tilt);
          r = projectChar(rgbA[0], rgbB[0], value);
          g = projectChar(rgbA[1], rgbB[1], value);
          b = projectChar(rgbA[2], rgbB[2], value);

          ptr[c + 2] = blend(ptr[c + 2], r, opacity);
          ptr[c + 1] = blend(ptr[c + 1], g, opacity);
          ptr[c] = blend(ptr[c], b, opacity);
        }
      }
    }
} // process(images, audio) END

void Gradient::process(stk::StkFrames& audio) {

  blendfloat blend;
  double opacity, frequency, phase, tilt, value, pm;
  std::size_t frames, channels;
  stk::StkFrames signalA, signalB;
  stk::StkFloat tick;

  blend    = eval::get_blendf(gradient_data, "blend");
  opacity  = eval::data_float(gradient_data, "opacity");
  frames   = audio.frames();
  channels = audio.channels();
  signalA  = eval::data_signal(gradient_data, "sineA", frames);
  signalB  = eval::data_signal(gradient_data, "sineB", frames);

  frequency = eval::data_float(gradient_data, "frequency");
  phase     = eval::data_float(gradient_data, "phase");
  tilt      = eval::data_float(gradient_data, "tilt");

  tilt = circle(0, 1, tilt);

  stk::StkFrames framesL(frames, 1), framesR(frames, 1);
  framesL.setChannel(0, audio, 0);
  framesR.setChannel(0, audio, 1);

  if(tilt <= 0.25) {
    pm = 1 - tilt * 4;
  } else if (tilt <= 0.5) {
    pm = (tilt - 0.25) * (-4);
  } else if (tilt <= 0.75) {
    pm = 1 - (tilt - 0.5) * 4;
  } else {
    pm = (tilt - 0.75) * (-4);
  }
  phase += frequency * pm;

  for (std::size_t y = 0; y < frames; y++) {
    for (std::size_t x = 0; x < channels; x++) {

      value = point_gradient(y, x, channels, frames, frequency, phase, tilt);

      tick = project(signalA[y], signalB[y], value);

      if (x == 0)
        framesL[y] = blend(framesL[y], tick, opacity);
      else
        framesR[y] = blend(framesR[y], tick, opacity);

    }
  }

  audio.setChannel(0, framesL, 0);
  audio.setChannel(1, framesR, 0);

} // process(audio, images) END
