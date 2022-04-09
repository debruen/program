
#include "program.h"

Program::Program()
    : m_main{}, m_play_thread{}, m_record_thread{}, m_control(m_buffer, m_buffer_mutex) {

  m_data["settings"] = m_settings.data();
  m_data["filter"]   = m_filter.data();
  m_data["output"]   = m_output.data();

  m_frame_time = data::get_int(m_data["settings"], "frame time");
  m_frames = data::get_int(m_data["settings"], "frames");

  // //
  // m_control["reset"]  = m_reset;
  // m_control["play"]   = m_play;
  // m_control["record"] = m_record;

  m_main = std::thread{&Program::main, this};
  m_play_thread = std::thread{&Program::play, this};
}

// private

void Program::create_frame(std::size_t frame_index) {

  std::string type = data::get_str(m_data["settings"], "type");

  m_objects_mutex.lock();

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

  m_objects_mutex.unlock();

  frame new_frame = {.index = frame_index, .image = image, .audio = audio};

  m_buffer_mutex.lock();
  m_buffer.push_back(new_frame);
  m_buffer_mutex.unlock();

}

void Program::update_buffer() {

  m_buffer_mutex.lock();
  std::size_t size = m_buffer.size();
  m_buffer_mutex.unlock();

  if(size < m_frames + 1) {
    m_buffer_full = false;
    std::size_t new_frame_index{m_current_frame};

    if(size != 0) {
      new_frame_index = last_buffer_index() + 1;
    }

    auto start = std::chrono::system_clock::now();
    create_frame(new_frame_index);
    auto end = std::chrono::system_clock::now();
    auto elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
    std::size_t relation =  elapsed.count() / m_frame_time + 1;
    if(relation < 2) relation = 2;
    m_buffer_size = relation + 1;
  } else {
    m_buffer_full = true;
  }

}

void Program::clear_buffer() {

  std::vector<frame> temp_buffer;

  if (m_update_main) {

    m_update_main = false;

  } else {

    m_buffer_mutex.lock();
    for (std::size_t i = 0; i < m_buffer.size(); i++) {
      // if (m_buffer[i].index >= m_current_frame)
        temp_buffer.push_back(m_buffer[i]);
    }
    m_buffer_mutex.unlock();

  }

  m_buffer_mutex.lock();
  m_buffer = temp_buffer;
  m_buffer_mutex.unlock();

}

std::size_t Program::last_buffer_index() {

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


frame Program::get_frame(std::size_t frame_index) {

  frame frame;

  m_buffer_mutex.lock();
  for (std::size_t i = 0; i < m_buffer.size(); i++) {
    if (m_buffer[i].index == frame_index) {
      frame = m_buffer[i];
      break;
    }
  }
  m_buffer_mutex.unlock();

  return frame;
}

bool Program::frame_exists(std::size_t frame_index) {
  bool check = false;

  m_buffer_mutex.lock();
  for (std::size_t i = 0; i < m_buffer.size(); i++) {
    if (m_buffer[i].index == frame_index) {
      check = true;
      break;
    }
  }
  m_buffer_mutex.unlock();

  return check;
}


void Program::main() {

  while(m_work) {

    clear_buffer();

    update_buffer();

  }

  std::cout << "main quit" << '\n';
}

cv::Mat Program::get_audio(unsigned int nFrames, double streamTime) {

  m_current_frame = streamTime * 1000 / m_frame_time;
  // = audio height
  int frame_ticks = m_frame_time / 1000 * 44100;

  int start_tick = round(streamTime * 44100 - m_current_frame * static_cast<double>(frame_ticks));

  unsigned int rest_ticks = frame_ticks - start_tick;

  int frames_needed = nFrames / frame_ticks + 1;

  if(rest_ticks < nFrames) frames_needed++;

  std::cout << "current frame: " << m_current_frame << '\n';
  std::cout << "frame ticks: " << frame_ticks << '\n';
  std::cout << "start ticks: " << start_tick << '\n';
  std::cout << "rest ticks: " << rest_ticks << '\n';
  std::cout << "frames needed: " << frames_needed << '\n';

  int width = m_audio_channels, height = nFrames;
  cv::Mat buffer_data = cv::Mat::zeros(cv::Size(width, height), CV_64F);
  double* audio_pointer,* buffer_pointer;

  int f(m_current_frame), ticks{start_tick};
  unsigned int g{0};

  for (int i = 0; i < frames_needed; i++) {

    if(i > 0) ticks = 0;

    frame frame = get_frame(f);
    f++;

    for (int j = ticks; j < frame_ticks; j++) {
      audio_pointer = frame.audio.ptr<double>(j);
      buffer_pointer = buffer_data.ptr<double>(g);

      for (int k = 0; k < m_audio_channels; k++) {
        buffer_pointer[k] = audio_pointer[k];
      }

      g++;
      if (g == nFrames) break;
    }

  }

  return buffer_data;
}


int Program::oscillator(void *outputBuffer, void *inputBuffer, unsigned int nFrames, double streamTime, RtAudioStreamStatus status, void *userData) {
  return static_cast<Program*>(userData)->oscillator(outputBuffer, inputBuffer, nFrames, streamTime, status);
}

int Program::oscillator(void *outputBuffer, void *inputBuffer, unsigned int nFrames, double streamTime, RtAudioStreamStatus status) {

  cv::Mat buffer_data = get_audio(nFrames, streamTime);

  double* buffer_pointer,* buffer = (double*) outputBuffer;

  if ( status )
    std::cout << "Stream underflow detected!" << std::endl;

  // Write interleaved audio data.
  unsigned int i, j;
  for ( i=0; i<nFrames; i++ ) {
    buffer_pointer = buffer_data.ptr<double>(i);
    for ( j=0; j<2; j++ ) {
      *buffer++ = buffer_pointer[j];
    }
  }

  return 0;
}

void Program::play() {

  if ( m_rtaudio.getDeviceCount() < 1 ) {
    std::cout << "\nNo audio devices found!\n";
  }

  RtAudio::DeviceInfo info = m_rtaudio.getDeviceInfo(m_rtaudio.getDefaultOutputDevice());

  std::cout << "info: " << info.nativeFormats << '\n';

  RtAudio::StreamParameters parameters;
  parameters.deviceId = m_rtaudio.getDefaultOutputDevice();
  parameters.nChannels = m_audio_channels;
  parameters.firstChannel = 0;
  unsigned int sampleRate = 44100;
  unsigned int bufferFrames = 256; // 256 sample frames

  try {
    m_rtaudio.openStream(&parameters, NULL, RTAUDIO_FLOAT64, sampleRate, &bufferFrames, &oscillator, this);
  }
  catch ( RtAudioError& e ) {
    e.printMessage();
  }

  // Record record(m_buffer);

  while(m_work) {

    if(m_play_state) {
      if (!m_rtaudio.isStreamRunning()) m_rtaudio.startStream();

    } else {
      if (m_rtaudio.isStreamRunning()) {
        m_rtaudio.stopStream();
        m_record_state = false;
      }
    }

    if(m_record_state && !m_recording) {
      m_play_state = false;
      if (m_rtaudio.isStreamRunning()) m_rtaudio.stopStream();
      double stream_time = m_current_frame * (m_frame_time / 1000);
      m_rtaudio.setStreamTime(stream_time);
      // m_recording = record.start();
      std::cout << "start recording" << '\n';
    }



    if(!m_record_state && m_recording) {
      if (m_play_state) m_play_state = false;
      if (m_record_state) m_record_state = false;
      if (m_rtaudio.isStreamRunning()) m_rtaudio.stopStream();
      // m_recording = record.stop();
      std::cout << "stop recording" << '\n';
    }

    if(m_play_state && m_recording) {
      // record.save(m_current_frame);
    }



    if(m_update_play || m_current_frame == m_frames) {
      if (m_rtaudio.isStreamRunning()) m_rtaudio.stopStream();
      m_rtaudio.setStreamTime(0.0);

      m_current_frame = 0;

      m_play_state = false;
      m_record_state = false;

      m_update_play = false;

      m_stop_mutex.lock();
      m_stop = true;
      m_stop_mutex.unlock();
    }

  }

  std::cout << "play quit" << '\n';
}

// public

// nlohmann::json init_controls(){
//   return m_control;
// }

nlohmann::json Program::data() {
  return m_data;
}
nlohmann::json Program::update(nlohmann::json data) {

  m_objects_mutex.lock();

  m_data["settings"] = m_settings.update(data["settings"]);

  m_frames = data::get_int(m_data["settings"], "frames");
  m_frame_time = data::get_int(m_data["settings"], "frame time");
  std::string type = data::get_str(m_data["settings"], "type");

  m_data["filter"]   = m_filter.update(data["filter"]);
  m_data["output"]   = m_output.update(data["output"], type);

  m_objects_mutex.unlock();

  m_update_main = true;
  m_update_play = true;

  return m_data;
}

nlohmann::json Program::buffer(nlohmann::json data, cv::Mat& image) {

  m_stop_mutex.lock();
  bool test = m_stop;
  m_stop_mutex.unlock();

  if(test) {
    data["play"] = false;
    data["record"] = false;
    m_stop_mutex.lock();
    m_stop = false;
    m_stop_mutex.unlock();
  } else {
    m_play_state   = data["play"];
    m_record_state = data["record"];
  }

  while (!frame_exists(m_current_frame)) {
    // std::cout << "waiting" << '\n';
  }

  frame frame = get_frame(m_current_frame);
  cv::resize(frame.image, image, cv::Size(image.cols,image.rows), 0, 0, cv::INTER_CUBIC);

  m_buffer_mutex.lock();
  std::size_t size = m_buffer.size();
  m_buffer_mutex.unlock();

  if (size < m_frames + 1) {
    data["block"] = true;
  } else {
    data["block"] = false;
  }

  return data;
}

void Program::quit() {

  std::cout << "****** quit ******" << '\n';
  m_work = false;

  m_play_thread.join();
  m_main.join();
}

// ---------------------------------------- * ----------------------------------------

nlohmann::json Program::init_control() {
  nlohmann::json data = m_control.init();
  return data;
}

nlohmann::json Program::data_control(nlohmann::json data) {
  data = m_control.data(data);
  return data;
}
