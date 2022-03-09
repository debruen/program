
#ifndef program_h
#define program_h

#include <string>
#include <chrono>
#include <thread>
#include <mutex>

#include "stk/SineWave.h"
#include "rtaudio/RtAudio.h"

#include "settings.h"
#include "filter.h"
#include "output.h"

#include "record.h"

class Program {

  private:

    bool m_work = true;

    nlohmann::json m_data;

    Settings m_settings;
    Filter   m_filter;
    Output   m_output;
    std::mutex m_objects_mutex;


    std::size_t m_frame_time, m_buffer_size{2}, m_current_frame{0}, m_audio_channels{2};

    bool m_update_main{false}, m_update_play{false}, m_buffer_full{false}, m_recording{false};


    std::vector<frame> m_buffer;
    std::mutex m_buffer_mutex;

    std::thread m_main;
    void main();


    std::vector<frame> m_buffer_storage;
    std::mutex m_buffer_storage_mutex;

    void play();
    std::thread m_play;

    static int static_oscillator(void *outputBuffer, void *inputBuffer, unsigned int nFrames, double streamTime, RtAudioStreamStatus status, void *userData);

    int oscillator(void *outputBuffer, void *inputBuffer, unsigned int nFrames, double streamTime, RtAudioStreamStatus status);

    cv::Mat get_audio(unsigned int nFrames, double streamTime);

    RtAudio m_rtaudio;

    bool m_play_state   = false;
    bool m_record_state = false;

    void create_frame(std::size_t frame_index);

    void clear_buffer();
    void update_buffer();

    frame get_frame(std::size_t frame_index);

    std::size_t last_buffer_index();
    bool frame_exists(std::size_t frame_index);

  public:
    Program();

    nlohmann::json data();
    nlohmann::json update(nlohmann::json data);

    nlohmann::json buffer(nlohmann::json data, cv::Mat& image);

    void quit();

};

#endif /* program_h */
