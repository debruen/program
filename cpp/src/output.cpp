
#include "output.h"

Output::Output() {

  m_base = new Hsl();
  m_data = m_base->data();
}

nlohmann::json Output::init() {

  return m_data;
}

nlohmann::json Output::update(nlohmann::json data, std::string type) {

  m_type = type;

  /// clear transform class if value is changed
  if(data[0]["value"] != m_data[0]["value"]) {
    if (data[0]["value"] == "hsl")
      m_base = new Hsl();

    if (data[0]["value"] == "rgb")
      m_base = new Hsl();

    data = m_base->data();
  } else {
    data = m_base->update(data);

  }

  m_data = data;

  return m_data;
}

void Output::image_frame(cv::Mat& image, cv::Mat& audio, std::size_t frame_index) {
  m_base->image_frame(image, audio, frame_index);
}

void Output::audio_frame(cv::Mat& image, cv::Mat& audio, std::size_t frame_index) {
  m_base->audio_frame(image, audio, frame_index);
}



void Output::process(std::vector<cv::Mat>& images, stk::StkFrames& audio) {

  if (m_type == "audio") {
    m_base->process(audio, images);
  } else {
    m_base->process(images, audio);
  }

}
