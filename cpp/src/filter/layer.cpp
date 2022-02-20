
#include "layer.h"

Layer::Layer() {

  m_data = nlohmann::json::array();

  m_blend = new Blend();
  m_film = new Film();
  m_data.push_back(m_film->data());
  m_data.push_back(m_blend->data());

}

nlohmann::json Layer::init(std::string type) {
  m_type = type;

  return m_data;
}

nlohmann::json Layer::update(nlohmann::json data, std::string type) {

  m_type = type;

  nlohmann::json new_data = nlohmann::json::array();

  new_data.push_back(m_film->update(data[0]));
  new_data.push_back(m_blend->update(data[1]));

  m_data = new_data;

  return m_data;
}

void Layer::image_frame(cv::Mat& image, std::size_t frame_index) {

  cv::Mat film = m_film->image_frame(image, frame_index);
  m_blend->image_frame(image, film, frame_index);

  // m_blend->process(image, film);
}

void Layer::audio_frame(cv::Mat& audio, std::size_t frame_index) {

  cv::Mat film = m_film->audio_frame(audio, frame_index);
  m_blend->audio_frame(audio, film, frame_index);
}
