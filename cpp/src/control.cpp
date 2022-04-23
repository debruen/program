
#include "control.h"

Control::Control(std::vector<frame>& buffer, std::mutex& buffer_mutex, info& info, std::mutex& info_mutex)
    : m_play(buffer, buffer_mutex, info, info_mutex), m_record(buffer, buffer_mutex, info, info_mutex) {

}

nlohmann::json Control::init() {

  m_play.init(m_data);
  m_record.init(m_data);

  return m_data;
}

nlohmann::json Control::data(nlohmann::json data) {

  m_play.data(data);
  // m_record.data(data);

  m_data = data;

  return m_data;
}

nlohmann::json Control::new_frame() {
  return m_play.new_frame();
}

void Control::display(cv::Mat& image) {
  m_play.display(image);
}

bool Control::record() {
  return m_record.record();
}

// bool Control::quit() {
//   return m_record.quit();
// }
