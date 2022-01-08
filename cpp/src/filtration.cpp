
#include "filtration.h"

Filtration::Filtration() {

  m_data = nlohmann::json::array();
}

std::string Filtration::get_type(nlohmann::json data) {

  std::string type;

  if (data[3]["label"] == "waveform") {
    type = "audio";
  } else {
    type = "image";
  }

  return type;
}

nlohmann::json Filtration::data() {

  return m_data;
}

nlohmann::json Filtration::data(nlohmann::json data, std::string type) {

  nlohmann::json old_data = m_data;

  m_data.clear();
  m_filter.clear();

  int add_count = 0;

  for (std::size_t i = 0; i < data.size(); i++) {

    std::string old_filter_type, old_type, filter_type = "";
    Filter* filter;

    if (data[i] != "add")
      filter_type = eval::data_str(data[i], "type");

    if (data[i] == "add" || filter_type == "fill")
      filter = new Fill(type);
    else if (data[i] != "add" && filter_type == "random")
      filter = new Random(type);
    else if (data[i] != "add" && filter_type == "gradient")
      filter = new Gradient(type);
    else if (data[i] != "add" && filter_type == "rectangle")
      filter = new Rectangle(type);

    m_filter.push_back(filter);

    old_filter_type = eval::data_str(old_data[i - add_count], "type");
    old_type = get_type(old_data[i - add_count]);

    if (data[i] == "add" || filter_type != old_filter_type || type != old_type) {
      m_data.push_back(m_filter[i]->data(type));
    } else {
      m_data.push_back(m_filter[i]->data(data[i], type));
    }

    if (data[i] == "add")
      add_count++;

  }

  return m_data;
}

void Filtration::process(std::vector<cv::Mat>& images, stk::StkFrames& audio, std::string type) {

  if (type == "audio") {
    for (std::size_t i = 0; i < m_filter.size(); i++) {
      m_filter[i]->process(audio);
    }
  } else {
    for (std::size_t i = 0; i < m_filter.size(); i++) {
      m_filter[i]->process(images);
    }
  }

}

// void Filtration::process(stk::StkFrames& audio, std::vector<cv::Mat>& images) {
//
// }
