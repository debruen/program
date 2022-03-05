
#include "program.h"

Program::Program() : m_main{} {

  m_play_data.push_back(data::init_bool("play", false));
  m_play_data.push_back(data::init_int("frame", 0, 100, 0));
  m_play_data.push_back(data::init_time("time", 0, 3000, 0));

  m_data["settings"] = m_settings.data();
  m_data["filter"]   = m_filter.data();
  m_data["output"]   = m_output.data();

  m_frame_time = data::get_int(m_data["settings"], "frame time");

  m_main = std::thread{&Program::main, this};
  m_play = std::thread{&Program::play, this};
}

void Program::create_frame(std::size_t frame_index) {

  auto start = std::chrono::system_clock::now();

  // settings
  std::string type = data::get_string(m_data["settings"], "type");

  m_objects_mutex.lock();

  cv::Mat image = m_settings.image_frame(frame_index);
  cv::Mat audio = m_settings.audio_frame(frame_index);


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

  m_settings.flip_back(image);

  m_objects_mutex.unlock();

  frame new_frame = {.index = frame_index, .image = image, .audio = audio};

  auto end = std::chrono::system_clock::now();
  auto elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);

  std::size_t relation =  elapsed.count() / m_frame_time + 1;

  if(relation < 2) relation = 2;

  m_buffer_size = relation;

  m_buffer.push_back(new_frame);
}

void Program::update_buffer() {

  m_buffer_mutex.lock();
  if(m_buffer.size() < m_buffer_size) {

    std::size_t new_frame_index{m_current_frame};

    if(m_buffer.size() != 0) {
      new_frame_index = last_buffer_index() + 1;
    }

    create_frame(new_frame_index);
  }
  m_buffer_mutex.unlock();

}

void Program::clear_buffer() {

  m_buffer_mutex.lock();
  std::vector<frame> temp_buffer;

  if (m_update) {

    m_update = false;

  } else {

    for (std::size_t i = 0; i < m_buffer.size(); i++) {
      if (m_buffer[i].index >= m_current_frame) {
        temp_buffer.push_back(m_buffer[i]);
      }
    }

  }

  m_buffer = temp_buffer;
  m_buffer_mutex.unlock();

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

    // check data
    // work on buffer

    clear_buffer();

    //
    update_buffer();

  }

  std::cout << "main quit" << '\n';
}

// int Program::saw( void *outputBuffer, void *inputBuffer, unsigned int nBufferFrames,
//          double streamTime, RtAudioStreamStatus status)
// {
//   // unsigned int i, j;
//   // double *buffer = (double *) outputBuffer;
//   // double *lastValues = (double *) userData;
//   // if ( status )
//   //   std::cout << "Stream underflow detected!" << std::endl;
//   // // Write interleaved audio data.
//   // for ( i=0; i<nBufferFrames; i++ ) {
//   //   for ( j=0; j<2; j++ ) {
//   //     *buffer++ = lastValues[j];
//   //     lastValues[j] += 0.005 * (j+1+(j*0.1));
//   //     if ( lastValues[j] >= 1.0 ) lastValues[j] -= 2.0;
//   //   }
//   // }
//   return 0;
// }

cv::Mat Program::get_audio(unsigned int nFrames, double streamTime) {

  // std::size_t sizo = m_buffer.size();

  std::size_t current_milliseconds = streamTime * 1000;

  // std::size_t milliseconds_for_buffer = nFrames / 44100 * 1000;

  m_current_frame = current_milliseconds / m_frame_time;


  std::size_t past_ticks = streamTime * 44100;

  std::size_t ticks_needed = nFrames;

  // audio height
  std::size_t frame_ticks = m_frame_time / 1000 * 44100;

  std::size_t start_tick = past_ticks - m_current_frame * frame_ticks;

  std::size_t rest_ticks = frame_ticks - start_tick;

  std::size_t frames_needed = ticks_needed / frame_ticks + 1;

  if(rest_ticks < ticks_needed) frames_needed++;

  // std::size_t milliseconds_of_frame = current_milliseconds - m_current_frame * m_frame_time;

  // std::size_t frames_needed = nFrames / ((m_frame_time / 1000) * 44100) + milliseconds_of_frame / m_frame_time + 1;

  // a.k.a audio height
  // std::size_t ticks = (m_frame_time / 1000) * 44100;

  std::cout << "frames_needed: " << frames_needed << '\n';
  std::cout << "start_tick: " << start_tick << '\n';

  std::cout << "m_current_frame: " << m_current_frame << '\n';


  cv::Mat buffer_data = cv::Mat::zeros(cv::Size(m_audio_channels, nFrames), CV_64F);
  double* audio_pointer,* buffer_pointer;

  std::size_t f{m_current_frame}, g{0}, ticks{start_tick};

  for (std::size_t i = 0; i < frames_needed; i++) {

    if(i > 0) ticks = 0;

    frame frame = get_frame(f);
    f++;

    for (std::size_t j = ticks; j < frame_ticks; j++) {
      audio_pointer = frame.audio.ptr<double>(j);
      buffer_pointer = buffer_data.ptr<double>(g);

      for (std::size_t k = 0; k < m_audio_channels; k++) {
        buffer_pointer[k] = audio_pointer[k];
      }

      g++;
      if (g == nFrames) break;
    }

  }

  return buffer_data;
}


int Program::static_oscillator(void *outputBuffer, void *inputBuffer, unsigned int nFrames, double streamTime, RtAudioStreamStatus status, void *userData) {
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

  // nlohmann::json json = info;

  std::cout << "info: " << info.nativeFormats << '\n';

  RtAudio::StreamParameters parameters;
  parameters.deviceId = m_rtaudio.getDefaultOutputDevice();
  parameters.nChannels = m_audio_channels;
  parameters.firstChannel = 0;
  unsigned int sampleRate = 44100;
  unsigned int bufferFrames = 256; // 256 sample frames
  // double ddd[2] = {0, 0};

  // int sss = std::bind(saw, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3, std::placeholders::_4, std::placeholders::_5);

  try {
    m_rtaudio.openStream(&parameters, NULL, RTAUDIO_FLOAT64,
                    sampleRate, &bufferFrames, &static_oscillator, this);
    // m_rtaudio.startStream();
  }
  catch ( RtAudioError& e ) {
    e.printMessage();
  }

  while(m_work) {

    if(m_play_state) {
      if (!m_rtaudio.isStreamRunning()) m_rtaudio.startStream();

    } else {
      if (m_rtaudio.isStreamRunning()) m_rtaudio.stopStream();

    }

  }

  std::cout << "play quit" << '\n';
}

nlohmann::json Program::data() {
  return m_data;
}
nlohmann::json Program::update(nlohmann::json data) {

  m_objects_mutex.lock();

  m_data["settings"] = m_settings.update(data["settings"]);

  m_frame_time = data::get_int(m_data["settings"], "frame time");
  std::string type = data::get_string(m_data["settings"], "type");

  m_data["filter"]   = m_filter.update(data["filter"], type);
  m_data["output"]   = m_output.update(data["output"], type);

  m_objects_mutex.unlock();

  m_update = true;

  return m_data;
}

// void Program::read(cv::Mat& image, cv::Mat& audio, std::size_t frame_index) {
//
//   m_current_frame = frame_index;
//   frame frame;
//
//   while (m_buffer.size() != m_buffer_size) {
//     if (frame_exists(frame_index)){
//        break;
//     }
//     std::cout << "waiting" << '\n';
//   }
//
//   if (frame_exists(frame_index)){
//
//     frame = get_frame(frame_index);
//     cv::resize(frame.image, image, cv::Size(image.cols,image.rows), 0, 0, cv::INTER_CUBIC);
//     audio = frame.audio;
//
//   }
//
//   std::cout << "m_current_frame: " << m_current_frame << '\n';
// }

nlohmann::json Program::buffer(nlohmann::json data, cv::Mat& image) {

  m_play_state   = data["play"];
  m_record_state = data["record"];

  while (!frame_exists(m_current_frame)) {

    std::cout << "waiting" << '\n';
  }

  frame frame = get_frame(m_current_frame);
  cv::resize(frame.image, image, cv::Size(image.cols,image.rows), 0, 0, cv::INTER_CUBIC);

  return data;
}

void Program::quit() {

  std::cout << "****** quit ******" << '\n';
  m_work = false;

  m_play.join();
  m_main.join();
}
