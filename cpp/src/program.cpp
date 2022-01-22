
#include "program.h"

Program::Program() {

  m_data["settings"] = m_settings.init();
  m_data["filter"]   = m_filter.init();
  m_data["output"]   = m_output.init();

}

std::string Program::work() {

  std::string msg = "work done!";
  return msg;
}

nlohmann::json Program::init() {
  return m_data;
} // data()

nlohmann::json Program::update(nlohmann::json data) {

  std::string type = data::get_string(data["settings"], "type");

  m_data["settings"] = m_settings.update(data["settings"]);
  m_data["filter"]   = m_filter.update(data["filter"], type);
  m_data["output"]   = m_output.update(data["output"], type);

  return m_data;
} // data()

nlohmann::json Program::get() {
  return m_data;
} // get()

void Program::preview(std::vector<cv::Mat>& images, stk::StkFrames& audio) {

  m_settings.preview(images, audio);

  m_filter.process(images, audio);

  m_output.process(images, audio);

  m_settings.flip_back(images);

} // preview()

void Program::save() {

  std::vector<cv::Mat> images;
  stk::StkFrames audio;

  m_settings.file(images, audio);

  m_filter.process(images, audio);

  m_output.process(images, audio);

  m_settings.flip_back(images);

  m_settings.save(images, audio);

} // save
