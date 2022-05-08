
#include "program.h"

Program::Program()
    : m_synthesis(m_buffer, m_buffer_mutex, m_info), m_control(m_buffer, m_buffer_mutex, m_info) {

}


nlohmann::json Program::init_synthesis() {
  return m_synthesis.init();
}

nlohmann::json Program::data_synthesis(nlohmann::json data) {
  return m_synthesis.data(data);
}


nlohmann::json Program::init_control() {
return m_control.init();
}

nlohmann::json Program::data_control(nlohmann::json data) {
  return m_control.data(data);
}


nlohmann::json Program::new_frame() {
  return m_control.new_frame();
}

void Program::display(cv::Mat& image, cv::Mat& left, cv::Mat& right) {
  m_control.display(image, left, right);
}

bool Program::record() {
  return m_control.record();
}

bool Program::quit() {
  bool quit{false};

  if (m_synthesis.quit()) quit = true;

  return quit;
}
