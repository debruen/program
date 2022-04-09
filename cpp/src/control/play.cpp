
#include "play.h"

Play::Play(std::vector<frame>& buffer, std::mutex& mutex)
    : m_buffer(buffer), m_buffer_mutex(mutex), m_play_thread{} {

  m_play_thread = std::thread{&Play::play, this};
}

frame Play::get_frame(std::size_t& index) {

  frame frame;

  m_buffer_mutex.lock();
  for (std::size_t i = 0; i < m_buffer.size(); i++) {
    if (m_buffer[i].index == index) {
      frame = m_buffer[i];
      break;
    }
  }
  m_buffer_mutex.unlock();

  return frame;
}

cv::Mat Play::get_audio(unsigned int nFrames, double streamTime) {

  m_frame = streamTime * 1000 / m_time;
  // = audio height
  int frame_ticks = m_time / 1000 * 44100;

  int start_tick = round(streamTime * 44100 - m_frame * static_cast<double>(frame_ticks));

  unsigned int rest_ticks = frame_ticks - start_tick;

  int frames_needed = nFrames / frame_ticks + 1;

  if(rest_ticks < nFrames) frames_needed++;

  std::cout << "current frame: " << m_frame << '\n';
  std::cout << "frame ticks: " << frame_ticks << '\n';
  std::cout << "start ticks: " << start_tick << '\n';
  std::cout << "rest ticks: " << rest_ticks << '\n';
  std::cout << "frames needed: " << frames_needed << '\n';

  int width = m_channels, height = nFrames;
  cv::Mat buffer_data = cv::Mat::zeros(cv::Size(width, height), CV_64F);
  double* audio_pointer,* buffer_pointer;

  std::size_t f{m_frame};
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


int Play::oscillator(void *outputBuffer, void *inputBuffer, unsigned int nFrames, double streamTime, RtAudioStreamStatus status, void *userData) {
  return static_cast<Play*>(userData)->oscillator(outputBuffer, inputBuffer, nFrames, streamTime, status);
}

int Play::oscillator(void *outputBuffer, void *inputBuffer, unsigned int nFrames, double streamTime, RtAudioStreamStatus status) {

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

void Play::play() {

  if(m_rtaudio.getDeviceCount() < 1)
    std::cout << "No audio devices found!"  << '\n';

  RtAudio::StreamParameters parameters;
  parameters.deviceId = m_rtaudio.getDefaultOutputDevice();
  parameters.nChannels = 2;
  parameters.firstChannel = 0;
  unsigned int sampleRate = 44100;
  unsigned int bufferFrames = 256; // 256 sample frames

  try {
    m_rtaudio.openStream(&parameters, NULL, RTAUDIO_FLOAT64, sampleRate, &bufferFrames, &oscillator, this);
  } catch(RtAudioError& e) {
    e.printMessage();
  }

  while(m_thread) {

    if(m_play) {
      if(!m_rtaudio.isStreamRunning()) m_rtaudio.startStream();
    } else {
      if(m_rtaudio.isStreamRunning()) m_rtaudio.stopStream();
    }

    if(m_reset) {
      if(m_rtaudio.isStreamRunning()) m_rtaudio.stopStream();
      m_rtaudio.setStreamTime(0.0);
      m_frame = 0;
      m_reset = false;
    }

  }
}

bool Play::start(int& channels, int& time) {
  m_channels = channels;
  m_time = time;

  m_play = true;

  return m_play;
}

bool Play::pause(int& channels, int& time) {
  m_channels = channels;
  m_time = time;

  m_play = false;

  return m_play;
}

bool Play::reset(int& channels, int& time) {
  m_channels = channels;
  m_time = time;

  m_reset = true;

  std::size_t c{0};
  while(m_reset) {
    if(c == 0) std::cout << "waiting to reset play ..." << '\n';
    c++;
  }
  if(c == 0) std::cout << "... play is reset!" << '\n';

  return m_reset;
}

bool Play::quit() {

  m_thread = false;

  m_play_thread.join();

  return true;
}
