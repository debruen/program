
#ifndef play_h
#define play_h

#include <thread>
#include <mutex>

#include "rtaudio/RtAudio.h"

#include "../data.h"

class Play {

  private:

    std::vector<frame>& m_buffer;
    std::mutex m_buffer_mutex;

    bool m_thread = true;

    RtAudio m_rtaudio;

    bool m_play, m_reset;
    std::size_t m_frame{0};

    void play();
    std::thread m_play_thread;

    cv::Mat get_audio(unsigned int nFrames, double streamTime);

    static int oscillator(void *outputBuffer, void *inputBuffer, unsigned int nFrames, double streamTime, RtAudioStreamStatus status, void *userData);
    int oscillator(void *outputBuffer, void *inputBuffer, unsigned int nFrames, double streamTime, RtAudioStreamStatus status);

  public:
    Play(std::vector<frame>& buffer);

    bool start();

    bool pause();

    bool reset();


    bool quit();

};

#endif /* play_h */
