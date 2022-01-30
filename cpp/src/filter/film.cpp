
#include "film.h"

Film::Film() {

  m_fill_type = "spectrum";

  m_fill = new Spectrum();
  m_data = m_fill->data();
}

nlohmann::json Film::init() {

  return m_data;
}

nlohmann::json Film::update(nlohmann::json data) {

  std::string type = data::get_string(data, "type");

  if (type != m_fill_type) {

    if (type == "noise")
      m_fill = new Spectrum();
    else
      m_fill = new Spectrum();

    m_data = m_fill->data();

  } else {
    m_data = m_fill->update(data);
  }

  m_fill_type = type;


  return m_data;
}

cv::Mat Film::frame(std::size_t frame, std::size_t width, std::size_t height) {

  return m_fill->frame(width, height);
}


stk::StkFrames Film::frame(std::size_t length) {

  return m_fill->frame(length);

}

std::vector<cv::Mat> Film::images(std::size_t frames, std::size_t width, std::size_t height) {

  std::vector<cv::Mat> film;

  for (size_t i = 0; i < frames; i++) {
    film.push_back(m_fill->frame(width, height));
  }

  return film;
}
