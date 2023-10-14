
#include "noise.h"

Noise::Noise(std::string type) {

  m_noise_data = init(type);

  m_noise_data[0]["value"] = "noise";

  /// 4: world settings / color for images, sine for audio
  m_noise_data.push_back(get_noise(type));
} // constructor END

nlohmann::json Noise::get_noise(std::string type) {

  nlohmann::json data;

  if (type == "image") {

    /// 4: color / image only
    data["label"]  = "color";
    data["select"]  = "color";  // select, range, files, color, sine
    data["min"] = 0;
    data["max"] = 1;
    data["default"] = 0.5;
    data["a"] = data["default"];
    data["b"] = data["default"];
    data["c"] = data["default"];

  } else {

    /// 4: sine_values / audio only
    data["label"]  = "sine";
    data["select"]  = "sine";  // select, range, files, color, sine
    data["min"] = 0;
    data["max"] = 1;
    data["default"] = 0.5;
    data["a"] = data["default"];
    data["b"] = data["default"];
    data["c"] = data["default"];

  }

  return data;
}

nlohmann::json Noise::data(std::string type) {

  return m_noise_data;
} // data() END

nlohmann::json Noise::data(nlohmann::json data, std::string type) {
  std::string old_type = get_type(data);

  m_noise_data = set_data(data, type);

  if (type != old_type) {
    m_noise_data.push_back(get_noise(type));
  } else {
    m_noise_data.push_back(data[4]);
  }

  return m_noise_data;
} // data(data) END

void Noise::process(std::vector<cv::Mat>& images) {

  blendchar blend;
  double opacity;
  std::vector<unsigned char> rgb;

  blend   = eval::get_blend(m_noise_data, "blend");
  opacity = eval::data_float(m_noise_data, "opacity");
  rgb     = eval::data_rgb(m_noise_data, "color");

  cv::Mat table(1, 256, CV_8UC3);

  uchar r, g, b, value;

  for (int i = 0; i < 256; ++i) {
    value = static_cast<uchar>(i);

    r = blend(value, rgb[0], opacity);
    g = blend(value, rgb[1], opacity);
    b = blend(value, rgb[2], opacity);

    table.at<cv::Vec3b>(0,i) = cv::Vec3b(b, g, r);
  }

  for (unsigned int i = 0; i < images.size(); i++) {
    cv::LUT(images[i], table, images[i]);
  }

} // process(images) END

void Noise::process(stk::StkFrames& audio) {

  blendfloat blend;
  double opacity;
  std::size_t frames;
  stk::StkFrames signal;

  blend   = eval::get_blendf(m_noise_data, "blend");
  opacity = eval::data_float(m_noise_data, "opacity");
  frames  = audio.frames();
  signal  = eval::data_signal(m_noise_data, "sine", frames);

  stk::StkFrames framesL(frames, 1), framesR(frames, 1);

  framesL.setChannel(0, audio, 0);
  framesR.setChannel(0, audio, 1);

  for(std::size_t i = 0; i < frames; ++i) {

    framesL[i] = blend(framesL[i], signal[i], opacity);
    framesR[i] = blend(framesR[i], signal[i], opacity);

  }

  audio.setChannel(0, framesL, 0);
  audio.setChannel(1, framesR, 0);

} // process(audio) END
