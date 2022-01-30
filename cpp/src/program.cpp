
#include "program.h"

Program::Program() {

  m_data["settings"] = m_settings.data();
  m_data["filter"]   = m_filter.init();
  m_data["output"]   = m_output.init();

}

frame Program::create_frame(nlohmann::json data, std::size_t f) {

  cv::Mat image;
  stk::StkFrames audio;

  frame frame = {.frame = f, .image = image, .audio = audio};

  // settings
  // filter
  // output

  return frame;
}

frame Program::get_frame(std::size_t f) {

  frame frame;
  bool check = false;

  for (std::size_t i = 0; i < m_frames.size(); i++) {
    if (m_frames[i].frame == f) {
      frame = m_frames[i];
      check = true;
      break;
    }
  }

  if(!check) throw std::invalid_argument("no matching frame");

  return frame;
}

std::string Program::work() {

  bool run = true;

  nlohmann::json data;

  while(run) {
    data = get();

  }
  // loop
  // read data
  // work on buffer

  std::string msg = "work done!";
  return msg;
} // work()



frame Program::read(std::size_t f) {

  frame frame;

  try {

    frame = get_frame(f);

  } catch (const std::invalid_argument& e) {

  }

  return frame;
}

nlohmann::json Program::get() {
  return m_data;
} // get()

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
