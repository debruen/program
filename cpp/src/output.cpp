
#include "output.h"

Output::Output() {

  m_base = new Hsl();
  m_data = m_base->data();
}

nlohmann::json Output::data() {

  return m_data;
}

nlohmann::json Output::update(nlohmann::json data, std::string type) {

  m_type = type;

  std::string old_t, new_t;

  old_t = data::get_str(m_data, "type");
  new_t = data::get_str(data, "type");

  /// clear transform class if value is changed
  if(new_t != old_t) {
    if (new_t == "hsl")
      m_base = new Hsl();

    if (new_t == "rgb")
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
