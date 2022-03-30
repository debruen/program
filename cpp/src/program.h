
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

    // dimensions

    // image bit depth {8 16 32}
    // image color space {RGB HSL Lab Lch}

    // width, height | int
    // resolution
    // presets {A6 A5 A4 A3 A2 A1 A0 ...}

    // audio bit depth {16 32 64}
    // audio sampling rate {44.1 48 88.2 96 176.4 192}

    // channels, frame time | int

    // mask bit depth {32 64}

    // recording length in frames

    // globals image audio mask ??

    bool m_work = true;

    /// main data object
    nlohmann::json m_data;

    /// main creation classes
    Settings m_settings;
    Filter   m_filter;
    Output   m_output;

    std::mutex m_objects_mutex;

    std::size_t m_frame_time, m_buffer_size{2}, m_frames{1}, m_current_frame{0};

    int m_audio_channels{2};

    bool m_update_main{false}, m_update_play{false}, m_buffer_full{false}, m_recording{false}, m_stop{false};
    std::mutex m_stop_mutex;


    std::vector<frame> m_buffer;
    std::mutex m_buffer_mutex;

    std::thread m_main;
    void main();

    std::vector<frame> m_buffer_storage;
    std::mutex m_buffer_storage_mutex;

    static int oscillator(void *outputBuffer, void *inputBuffer, unsigned int nFrames, double streamTime, RtAudioStreamStatus status, void *userData);

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


    // nlohmann::json m_control = nlohmann::json::array();

    void play();
    std::thread m_play_thread;

    bool m_play{false};

    void record();
    std::thread m_record_thread;

    bool m_record{false};

    bool m_reset{false};

  public:
    Program();

    // init_synthesis
    // data_synthesis

      // thread_synthesis


    // nlohmann::json init_controls();
    // data_controls

      // thread_play
      // thread_save


    // work_display


    // quit

    nlohmann::json data();
    nlohmann::json update(nlohmann::json data);

    nlohmann::json buffer(nlohmann::json data, cv::Mat& image);

    void quit();

};

#endif /* program_h */
