
#include "filter.h"

Filter::Filter() {
  m_data = nlohmann::json::array();
}

nlohmann::json Filter::data() {

  return m_data;
}

nlohmann::json Filter::update(nlohmann::json data, std::string type) {

  nlohmann::json new_data = nlohmann::json::array();
  std::vector< Layer* > new_layer;

  std::size_t c = 0;

  for (std::size_t i = 0; i < data.size(); i++) {

    if(data[i] == "add") {
      new_layer.push_back(new Layer());
      new_data.push_back(new_layer[i]->init(type));
    } else {
      new_layer.push_back(m_layer[c]);
      new_data.push_back(new_layer[i]->update(data[i], type));

      c++;
    }

  }

  m_type = type;
  m_data = new_data;
  m_layer = new_layer;

  return m_data;
}

void Filter::image_frame(cv::Mat& image, std::size_t frame_index) {

  for (std::size_t i = 0; i < m_layer.size(); i++) {
    m_layer[i]->audio_frame(image, frame_index);
  }

}

void Filter::audio_frame(cv::Mat& audio, std::size_t frame_index) {

  for (std::size_t i = 0; i < m_layer.size(); i++) {
    m_layer[i]->audio_frame(audio, frame_index);
  }

}


void Filter::process(std::vector<cv::Mat>& images, stk::StkFrames& audio) {

  for (std::size_t i = 0; i < m_layer.size(); i++) {
    m_layer[i]->process(images, audio);
  }

}
