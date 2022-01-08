
#include "random.h"

Random::Random(std::string type) {

  random_data = init(type);

  random_data[0]["value"] = "random";

  random_data.push_back(get_fillA(type));
  random_data.push_back(get_fillB(type));

} // constructor END

nlohmann::json Random::get_fillA(std::string type) {

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

nlohmann::json Random::get_fillB(std::string type) {

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

nlohmann::json Random::data(std::string type) {

  return random_data;
} // data() END

nlohmann::json Random::data(nlohmann::json data, std::string type) {

  std::string old_type = get_type(data);

  random_data = set_data(data, type);

  if (type != old_type) {
    random_data.push_back(get_fillA(type));
    random_data.push_back(get_fillB(type));
  } else {
    random_data.push_back(data[4]);
    random_data.push_back(data[5]);
  }

  return random_data;
} // data(data) END

void Random::process(std::vector<cv::Mat>& images) {

  blendchar blend;
  double opacity;
  std::vector<unsigned char> rgbA, rgbB;

  blend   = eval::get_blend(random_data, "blend");
  opacity = eval::data_float(random_data, "opacity");
  rgbA    = eval::data_rgb(random_data, "colorA");
  rgbB    = eval::data_rgb(random_data, "colorB");

  unsigned int width, height;
  uchar r, g, b, value;

  std::mt19937 generator(8);
  std::uniform_int_distribution<double> distribution(0,1);

  width = static_cast<unsigned int>(images[0].cols);
  height = static_cast<unsigned int>(images[0].rows);

  unsigned int c;
  uchar* ptr;

  for (unsigned int i = 0; i < images.size(); i++) {
    for (unsigned int y = 0; y < height; ++y) {

      ptr = images[i].ptr<uchar>(y);
      for (unsigned int x = 0; x < width; ++x) {
        c = x * 3;

        value = distribution(generator);

        r = projectChar(rgbA[0], rgbB[0], value);
        g = projectChar(rgbA[1], rgbB[1], value);
        b = projectChar(rgbA[2], rgbB[2], value);

        ptr[c + 2] = blend(ptr[c + 2], r, opacity);
        ptr[c + 1] = blend(ptr[c + 1], g, opacity);
        ptr[c] = blend(ptr[c], b, opacity);
      }
    }
  }
} // process(images) END

void Random::process(stk::StkFrames& audio) {

  std::mt19937 generator(1);
  std::uniform_int_distribution<double> distribution(0,1);

  blendfloat blend;
  double opacity, value;
  std::size_t frames;
  stk::StkFrames signalA, signalB;
  stk::StkFloat tick;

  blend   = eval::get_blendf(random_data, "blend");
  opacity = eval::data_float(random_data, "opacity");
  frames  = audio.frames();
  signalA = eval::data_signal(random_data, "sineA", frames);
  signalB = eval::data_signal(random_data, "sineB", frames);

  stk::StkFrames framesL(frames, 1), framesR(frames, 1);

  framesL.setChannel(0, audio, 0);
  framesR.setChannel(0, audio, 1);

  for(std::size_t i = 0; i < frames; ++i) {

    value = distribution(generator);
    tick = project(signalA[i], signalB[i], value);

    framesL[i] = blend(framesL[i], tick, opacity);
    framesR[i] = blend(framesR[i], tick, opacity);

  }

  audio.setChannel(0, framesL, 0);
  audio.setChannel(1, framesR, 0);
} // process(audio) END
