
#include "synthesis.h"

Synthesis::Synthesis(std::vector<frame>& buffer, std::mutex& buffer_mutex, info& info, std::mutex& info_mutex)
    : m_buffer{buffer}, m_buffer_mutex{buffer_mutex}, m_info{info}, m_info_mutex{info_mutex} {

  m_data["settings"] = m_settings.data();
  m_data["filter"]   = m_filter.data();
  m_data["output"]   = m_output.data();

  m_time = data::get_int(m_data["settings"], "frame time");
  m_frames = data::get_int(m_data["settings"], "frames");

  m_info_mutex.lock();
  info.channels = 2;
  info.time = m_time;
  info.frames = m_frames;
  info.update = m_update;
  m_info_mutex.unlock();
}

void Synthesis::clear_buffer() {

  std::vector<frame> temp_buffer;

  if (m_update) {
    m_update = false;
  } else {
    m_buffer_mutex.lock();
    for (std::size_t i = 0; i < m_buffer.size(); i++) {
      temp_buffer.push_back(m_buffer[i]);
    }
    m_buffer_mutex.unlock();
  }

  m_buffer_mutex.lock();
  m_buffer = temp_buffer;
  m_buffer_mutex.unlock();

}

void Synthesis::create_buffer() {

  m_buffer_mutex.lock();
  std::size_t size = m_buffer.size();
  m_buffer_mutex.unlock();

  if(size < m_frames + 1) {
    m_full = false;
    m_info_mutex.lock();
    info.full = m_full;
    m_info_mutex.unlock();
    std::size_t new_frame_index{m_current_frame};

    if(size != 0) {
      new_frame_index = last_buffer_index() + 1;
    }

    create_frame(new_frame_index);

  } else {
    m_full = true;
    m_info_mutex.lock();
    info.full = m_full;
    m_info_mutex.unlock();
  }

}

std::size_t Synthesis::last_index() {

  std::size_t frame_index{0};

  m_buffer_mutex.lock();
  for (std::size_t i = 0; i < m_buffer.size(); i++) {
    if (m_buffer[i].index >= frame_index) {
      frame_index = m_buffer[i].index;
    }
  }
  m_buffer_mutex.unlock();

  return frame_index;
}

void Synthesis::create_frame(std::size_t index) {

  std::string type = data::get_str(m_data["settings"], "type");

  m_mutex.lock();

  cv::Mat image = m_settings.image();
  cv::Mat audio = m_settings.audio();

  // filter
  if(type == "audio") {
    std::cout << "audio type: " << type << '\n';
    m_filter.audio_frame(audio, frame_index);
    // m_output.audio_frame(image, audio, frame_index);

  } else {
    std::cout << "image type: " << type << '\n';
    m_filter.image_frame(image, frame_index);

    // m_output.image_frame(image, audio, frame_index);
  }

  m_settings.flip(image, true);

  m_mutex.unlock();

  frame new_frame = {.index = frame_index, .image = image, .audio = audio};

  m_buffer_mutex.lock();
  m_buffer.push_back(new_frame);
  m_buffer_mutex.unlock();

}

void Record::buffer() {

  while(!m_quit) {

    clear_buffer();

    create_buffer();

  }

}

nlohmann::json Synthesis::init() {

  return m_data;
}

nlohmann::json Synthesis::data(nlohmann::json data) {

  m_mutex.lock();

  m_data["settings"] = m_settings.update(data["settings"]);
  m_data["filter"]   = m_filter.update(data["filter"]);
  std::string type = data::get_str(m_data["settings"], "type");
  m_data["output"]   = m_output.update(data["output"], type);

  m_frames = data::get_int(m_data["settings"], "frames");
  m_time = data::get_int(m_data["settings"], "frame time");

  m_mutex.unlock();

  m_update = true;

  m_info_mutex.lock();
  info.frames = m_frames;
  info.time = m_time;
  info.update = m_update;
  m_info_mutex.unlock();

  return m_data;
}

bool Synthesis::quit() {

  m_quit = true;
  m_buffer_thread.join();

  return true;
}
