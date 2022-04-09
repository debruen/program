
#include "program.h"

Program::Program()
    : m_synthesis(m_buffer, m_buffer_mutex, m_info, m_info_mutex), m_control(m_buffer, m_buffer_mutex, m_info, m_info_mutex) {

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

void Program::display(cv::Mat& image) {
  m_control.display(image);
}

bool Program::quit() {
  bool quit{false};

  if (m_synthesis.quit() && m_control.quit())
    quit = true;

  return quit;
}
