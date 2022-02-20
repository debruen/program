
#include "film.h"

Film::Film() {

  m_coat_type = "spectrum";

  m_coat = new Spectrum();
  m_data = m_coat->data();
}

nlohmann::json Film::data() {

  return m_data;
}

nlohmann::json Film::update(nlohmann::json data) {

  std::string type = data::get_string(data, "type");

  if (type != m_coat_type) {

    if (type == "noise")
      m_coat = new Spectrum();
    else
      m_coat = new Spectrum();

    m_data = m_coat->data();

  } else {
    m_data = m_coat->update(data);
  }

  m_coat_type = type;


  return m_data;
}


cv::Mat Film::image_frame(cv::Mat& image, std::size_t frame_index) {

  return m_coat->image_frame(image, frame_index);
}

cv::Mat Film::audio_frame(cv::Mat& audio, std::size_t frame_index) {

  return m_coat->audio_frame(audio, frame_index);
}
