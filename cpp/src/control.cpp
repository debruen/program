
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

  m_player.data(data);
  // m_recorder.data(data);

  m_data = data;

  return m_data;
}

bool Control::new_frame() {
  return m_player.new_frame();
}

void Control::display(cv::Mat& image) {
  m_player.display(image);
  std::cout << "Display" << '\n';
}

bool Control::record() {
  return m_recorder.record();
}

// bool Control::quit() {
//   return m_recorder.quit();
// }
