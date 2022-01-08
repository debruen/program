
#include "fill.h"

Fill::Fill(std::string type) {

  m_fill_data = init(type);

  m_fill_data[0]["value"] = "fill";

  /// 4: world settings / color for images, sine for audio
  m_fill_data.push_back(get_fill(type));
} // constructor END

nlohmann::json Fill::get_fill(std::string type) {

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

nlohmann::json Fill::data(std::string type) {

  return m_fill_data;
} // data() END

nlohmann::json Fill::data(nlohmann::json data, std::string type) {
  std::string old_type = get_type(data);

  m_fill_data = set_data(data, type);

  if (type != old_type) {
    m_fill_data.push_back(get_fill(type));
  } else {
    m_fill_data.push_back(data[4]);
  }

  return m_fill_data;
} // data(data) END

void Fill::process(std::vector<cv::Mat>& images) {

  blendchar blend;
  double opacity;
  std::vector<unsigned char> rgb;

  blend   = eval::get_blend(m_fill_data, "blend");
  opacity = eval::data_float(m_fill_data, "opacity");
  rgb     = eval::data_rgb(m_fill_data, "color");

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

void Fill::process(stk::StkFrames& audio) {

  blendfloat blend;
  double opacity;
  std::size_t frames;
  stk::StkFrames signal;

  blend   = eval::get_blendf(m_fill_data, "blend");
  opacity = eval::data_float(m_fill_data, "opacity");
  frames  = audio.frames();
  signal  = eval::data_signal(m_fill_data, "sine", frames);

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
