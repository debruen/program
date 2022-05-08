
#include "synthesis.h"

Synthesis::Synthesis(std::vector<frame>& buffer, std::mutex& buffer_mutex, info& info)
    : m_buffer{buffer}, m_buffer_mutex{buffer_mutex}, m_info{info} {

  m_data["settings"] = m_settings.data();
  m_data["filter"]   = m_filter.data();
  m_data["output"]   = m_output.data();

  m_time = data::get_int(m_data["settings"], "frame time");
  m_frames = data::get_int(m_data["settings"], "frames");

  // m_info_mutex.lock();
  m_info.channels = 2;
  m_info.time = m_time;
  m_info.frames = m_frames;
  m_info.update = m_update;
  m_info.start = m_start;
  // m_info_mutex.unlock();

  m_thread = std::thread{&Synthesis::thread, this};
}

void Synthesis::create_frame(std::size_t frame_index) {

  std::string type = data::get_str(m_data["settings"], "type");

  m_mutex.lock();

  cv::Mat image = m_settings.image();
  cv::Mat audio = m_settings.audio();

  // filter
  if(type == "audio") {
    m_filter.audio(audio, frame_index);
    // m_output.audio_frame(image, audio, frame_index);

  } else {
    m_filter.image(image, frame_index);

    // m_output.image_frame(image, audio, frame_index);
  }

  m_settings.flip(image, true);

  m_mutex.unlock();

  frame new_frame = {.index = frame_index, .image = image, .audio = audio};

  m_buffer_mutex.lock();
  m_buffer.push_back(new_frame);
  m_buffer_mutex.unlock();

}

void Synthesis::create_buffer() {

  m_buffer_mutex.lock();
  std::size_t size = m_buffer.size();
  m_buffer_mutex.unlock();

  if(size < m_frames + 1) {
    m_full = false;
    // m_info_mutex.lock();
    m_info.full = m_full;
    // m_info_mutex.unlock();
    std::size_t new_frame_index{0};

    if(size != 0) {
      new_frame_index = last_index() + 1;
    }

    create_frame(new_frame_index);

  } else {
    m_full = true;
    // m_info_mutex.lock();
    m_info.full = m_full;
    // m_info_mutex.unlock();
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

void Synthesis::thread() {
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
  m_start = data::get_int(m_data["settings"], "start");
  m_time = data::get_int(m_data["settings"], "frame time");

  m_mutex.unlock();

  m_update = true;

  // m_info_mutex.lock();
  m_info.frames = m_frames;
  m_info.start = m_start;
  m_info.time = m_time;
  m_info.update = m_update;
  // m_info_mutex.unlock();

  return m_data;
}

bool Synthesis::quit() {

  m_quit = true;
  m_thread.join();

  return m_quit;
}
