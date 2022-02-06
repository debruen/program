
#include "program.h"

Program::Program() {

  m_data["settings"] = m_settings.data();
  m_data["filter"]   = m_filter.data();
  m_data["output"]   = m_output.data();

  m_frame_time = data::get_int(m_data["settings"], "frame time");

}

void Program::create_frame(std::size_t frame_index) {

  auto start = std::chrono::system_clock::now();

  // settings
  std::string type = data::get_string(m_data["settings"], "type");

  cv::Mat image = m_settings.image_frame(frame_index);
  cv::Mat audio = m_settings.audio_frame(frame_index);

  // filter
  if(type == "audio") {
    m_filter.audio_frame(audio, frame_index);
    m_output.audio_frame(image, audio, frame_index);

  } else {
    m_filter.image_frame(image, frame_index);
    m_output.image_frame(image, audio, frame_index);
  }

  frame new_frame = {.index = frame_index, .image = image, .audio = audio};

  auto end = std::chrono::system_clock::now();
  auto elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);

  std::size_t relation =  elapsed.count() / m_frame_time + 1;

  std::cout << "frame_index: " << frame_index << '\n';
  std::cout << "m_buffer_size: " << m_buffer_size << '\n';
  std::cout << "m_frame_time: " << m_frame_time << '\n';
  std::cout << "elapsed: " << elapsed.count() << '\n';
  std::cout << "relation: " << relation << '\n';

  if(relation < 2) relation = 2;

  m_buffer_size = relation;

  m_buffer.push_back(new_frame);

}

void Program::update_buffer() {

  if(m_buffer.size() < m_buffer_size) {

    std::size_t new_frame_index{m_current_frame};

    if(m_buffer.size() != 0) {
      new_frame_index = last_buffer_index() + 1;
    }

    create_frame(new_frame_index);
  }

}

void Program::clean_buffer() {

  std::vector<frame> temp_buffer;

  for (std::size_t i = 0; i < m_buffer.size(); i++) {

    if (m_buffer[i].index >= m_current_frame) {
      temp_buffer.push_back(m_buffer[i]);
    }

  }

  m_buffer = temp_buffer;

}

std::size_t Program::last_buffer_index() {

  std::size_t frame_index{0};

  for (std::size_t i = 0; i < m_buffer.size(); i++) {

    if (m_buffer[i].index >= frame_index) {

      frame_index = m_buffer[i].index;

    }

  }

  return frame_index;
}


frame Program::get_frame(std::size_t f) {

  frame frame;
  bool check = false;

  for (std::size_t i = 0; i < m_buffer.size(); i++) {
    if (m_buffer[i].index == f) {
      frame = m_buffer[i];
      check = true;
      break;
    }
  }

  if(!check) throw std::invalid_argument("no matching frame");

  return frame;
}

void Program::main() {

  while(m_work) {

    // check data
    // work on buffer

    if (m_update) {
      std::vector<frame> temp_buffer;
      m_buffer = temp_buffer;

      m_update = false;
    }

    clean_buffer();

    //
    update_buffer();

  }

  m_running = false;

}

nlohmann::json Program::data() {
  return m_data;
}
nlohmann::json Program::update(nlohmann::json data) {

  std::string type = data::get_string(data["settings"], "type");

  m_data["settings"] = m_settings.update(data["settings"]);
  m_data["filter"]   = m_filter.update(data["filter"], type);
  m_data["output"]   = m_output.update(data["output"], type);

  m_update = true;

  return m_data;
}

frame Program::read(std::size_t frame_index) {

  frame frame;

  try {

    frame = get_frame(frame_index);

  } catch (const std::invalid_argument& e) {

  }

  return frame;
}

void Program::quit() {

  std::cout << "****** quit ******" << '\n';
  m_work = false;

  std::size_t a{0};
  while (m_running) {
    std::cout << "a ## " << a << '\n';
    a++;
  }
}


nlohmann::json Program::init() {
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
