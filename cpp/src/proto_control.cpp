
#include "control.h"

Control::Control(std::vector<frame>& buffer, std::mutex& buffer_mutex, info& info, std::mutex& info_mutex)
    : m_player(buffer, buffer_mutex, info, info_mutex), m_recorder(buffer, buffer_mutex, info, info_mutex) {

}

nlohmann::json Control::init() {

  m_player.init(m_data);
  m_recorder.init(m_data);

  return m_data;
}

nlohmann::json Control::data(nlohmann::json data) {

  m_player.init(data);
  m_recorder.init(data);

  m_data = data;

  return m_data;
}

void Control::display(cv::Mat& image) {
  m_player.display(image);
}

bool Control::quit() {
  return m_recorder.quit();
}
