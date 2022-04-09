
#include "player.h"

Player::Player(std::vector<frame>& buffer, std::mutex& buffer_mutex, info& info, std::mutex& info_mutex)
    : m_buffer(buffer), m_buffer_mutex(buffer_mutex), m_info(info), m_info_mutex(info_mutex) {

}

void Player::set() {

  m_info_mutex.lock();
  m_channels = m_info.channels;
  m_frame_time = m_info.frame_time;
  m_info_mutex.unlock();

  if(m_rtaudio.getDeviceCount() < 1)
    std::cout << "No audio devices found!"  << '\n';

  RtAudio::StreamParameters parameters;
  parameters.deviceId = m_rtaudio.getDefaultOutputDevice();
  parameters.nChannels = m_channels;
  parameters.firstChannel = 0;
  unsigned int sampleRate = m_sample_rate;
  unsigned int bufferFrames = 256; // 256 sample frames

  try {
    m_rtaudio.openStream(&parameters, NULL, RTAUDIO_FLOAT64, sampleRate, &bufferFrames, &oscillator, this);
  } catch(RtAudioError& e) {
    e.printMessage();
  }
}

bool Program::frame_exists(std::size_t& frame_index) {
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
frame Player::get_frame(std::size_t& frame_index) {

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

cv::Mat Player::get_audio(unsigned int nFrames, double streamTime) {

  m_current_frame = streamTime * 1000 / m_frame_time;
  if (m_current_frame > m_frame_count) {
    m_new = true;
    m_frame_count = m_current_frame;
  }
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

  int width = m_channels, height = nFrames;
  cv::Mat buffer_data = cv::Mat::zeros(cv::Size(width, height), CV_64F);
  double* audio_pointer,* buffer_pointer;

  std::size_t f{m_current_frame};
  int ticks{start_tick};
  unsigned int g{0};

  for (int i = 0; i < frames_needed; i++) {

    if(i > 0) ticks = 0;

    frame frame = get_frame(f);
    f++;

    for (int j = ticks; j < frame_ticks; j++) {
      audio_pointer = frame.audio.ptr<double>(j);
      buffer_pointer = buffer_data.ptr<double>(g);

      for (int k = 0; k < m_channels; k++) {
        buffer_pointer[k] = audio_pointer[k];
      }

      g++;
      if (g == nFrames) break;
    }

  }

  return buffer_data;
}
int Player::oscillator(void *outputBuffer, void *inputBuffer, unsigned int nFrames, double streamTime, RtAudioStreamStatus status, void *userData) {
  return static_cast<Player*>(userData)->oscillator(outputBuffer, inputBuffer, nFrames, streamTime, status);
}
int Player::oscillator(void *outputBuffer, void *inputBuffer, unsigned int nFrames, double streamTime, RtAudioStreamStatus status) {

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


void Player::init(nlohmann::json& data) {
  data["play"] = false;
  data["reset"] = false;
}

void Player::data(nlohmann::json& data) {

  if (data["reset"]) {
    // if(m_rtaudio.isStreamRunning()) m_rtaudio.stopStream();
    if(m_rtaudio.isStreamOpen()) m_rtaudio.closeStream();
    // set();
    data["play"] = false;
    data["reset"] = false;
  }

  if(data["play"]) {
    if(!m_rtaudio.isStreamOpen()) set();
    if(!m_rtaudio.isStreamRunning()) m_rtaudio.startStream();
  } else {
    if(m_rtaudio.isStreamRunning()) m_rtaudio.stopStream();
  }

}

void Player::display(cv::Mat& image) {

  if(m_new) {
    while (!frame_exists(m_current_frame)) {
      // std::cout << "waiting" << '\n';
    }

    frame frame = get_frame(m_current_frame);
    cv::resize(frame.image, image, cv::Size(image.cols,image.rows), 0, 0, cv::INTER_CUBIC);
  }

}
