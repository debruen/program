
#include "filter.h"

Filter::Filter() {

  /// name (dimensions)
  m_data.push_back(data::init_str("name", _name));

  /// display (true)
  m_data.push_back(data::init_bool("display", true));

}

nlohmann::json Filter::data() {

  return m_data;
}

nlohmann::json Filter::update(nlohmann::json data) {

  bool display = data::get_bool(data, "display");

  nlohmann::json new_data = nlohmann::json::array();
  std::vector< Layer* > new_layer;

  std::size_t c{0}, d{0};

  for (std::size_t i = 0; i < data.size(); i++) {
    std::cout << "array ?  " << data[i].type_name() << '\n';
    if(data[i] == "add") {

      // Layer* layer = new Layer();
      // nlohmann::json content = layer->init();

      new_layer.push_back(new Layer());
      new_data.push_back(new_layer[d]->init());
      d++;
    } else if(data[i].is_array()) {

      // Layer* layer = m_layer[c];
      // nlohmann::json content = layer->update(data[i]);
      //
      // new_layer.push_back(layer);
      // new_data.push_back(content);

      new_layer.push_back(m_layer[c]);
      new_data.push_back(new_layer[d]->update(data[i]));
      c++;
      d++;
    }
  }

  m_data = new_data;
  m_layer = new_layer;

  m_data.push_back(data::init_str("name", _name));
  m_data.push_back(data::init_bool("display", display));

  return m_data;
}

void Filter::image(cv::Mat& image, std::size_t index) {
  for (std::size_t i = 0; i < m_layer.size(); i++) {
    m_layer[i]->image(image, index);
  }

}

void Filter::audio(cv::Mat& audio, std::size_t index) {
  for (std::size_t i = 0; i < m_layer.size(); i++) {
    m_layer[i]->audio(audio, index);
  }

}
