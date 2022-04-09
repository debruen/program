
#ifndef play_h
#define play_h

#include <thread>
#include <mutex>

#include "rtaudio/RtAudio.h"

#include "../data.h"

class Play {

  private:

    std::vector<frame>& m_buffer;
    std::mutex& m_buffer_mutex;

    int m_channels, m_time;

    bool m_thread = true;

    RtAudio m_rtaudio;

    bool m_play{false}, m_reset{false};
    std::size_t m_frame{0};

    void play();
    std::thread m_play_thread;

    frame get_frame(std::size_t& index);
    cv::Mat get_audio(unsigned int nFrames, double streamTime);

    static int oscillator(void *outputBuffer, void *inputBuffer, unsigned int nFrames, double streamTime, RtAudioStreamStatus status, void *userData);
    int oscillator(void *outputBuffer, void *inputBuffer, unsigned int nFrames, double streamTime, RtAudioStreamStatus status);

  public:
    Play(std::vector<frame>& buffer, std::mutex& mutex);


    bool start(int& channels, int& time);

    bool pause(int& channels, int& time);

    bool reset(int& channels, int& time);


    bool quit();

};

#endif /* play_h */
