
#ifndef player_h
#define player_h

#include <thread>
#include <mutex>

#include "rtaudio/RtAudio.h"

#include "../data.h"

class Player {

  private:

    // array of frames
    //  index
    //  image
    //  audio
    std::vector<frame>& m_buffer;
    std::mutex& m_buffer_mutex;

    // int channels
    // int frame_time
    // std::size_t start_frame
    // std::size_t total_frames
    info& m_info;
    std::mutex& m_info_mutex;

    // bool play
    // bool reset
    nlohmann::json m_data;

    unsigned int m_sample_rate{44100}
    int m_sample_rate{44100}, m_channels{}, m_frame_time{}, m_frame_count{-1};
    bool m_new{true};
    std::size_t m_current_frame{0};

    RtAudio m_rtaudio;

    void set();

    bool frame_exists(std::size_t& frame_index);
    frame get_frame(std::size_t& frame_index);

    cv::Mat get_audio(unsigned int nFrames, double streamTime);
    static int oscillator(void *outputBuffer, void *inputBuffer, unsigned int nFrames, double streamTime, RtAudioStreamStatus status, void *userData);
    int oscillator(void *outputBuffer, void *inputBuffer, unsigned int nFrames, double streamTime, RtAudioStreamStatus status);

  public:
    Player(std::vector<frame>& buffer, std::mutex& buffer_mutex, info& info, std::mutex& info_mutex);

    void init(nlohmann::json& data);

    void data(nlohmann::json& data);

    void display(cv::Mat& image);
};

#endif /* player_h */
