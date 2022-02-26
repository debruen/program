
#ifndef program_h
#define program_h

#include <string>
#include <chrono>
#include <thread>
#include <mutex>

#include "rtaudio/RtAudio.h"

#include "settings.h"
#include "filter.h"
#include "output.h"

typedef struct {
  std::size_t index;
  cv::Mat image;
  cv::Mat audio;
} frame;

typedef unsigned char (*osc)(void*, void*, unsigned int, double, RtAudioStreamStatus);

class Program {

  private:

    nlohmann::json m_data, m_play_data = nlohmann::json::array();

    std::size_t m_frame_time, m_buffer_size{2}, m_current_frame{0};

    bool m_work = true, m_update = false;

    Settings m_settings;
    Filter   m_filter;
    Output   m_output;
    std::mutex m_objects_mutex;

    std::vector<frame> m_buffer;
    std::mutex m_buffer_mutex;

    std::thread m_main;
    std::thread m_play;

    static int static_oscillator(void *outputBuffer, void *inputBuffer, unsigned int nFrames, double streamTime, RtAudioStreamStatus status, void *userData);

    int oscillator(void *outputBuffer, void *inputBuffer, unsigned int nFrames, double streamTime, RtAudioStreamStatus status);

    RtAudio m_rtaudio;

    void main();
    void play();

    void create_frame(std::size_t frame_index);

    void clear_buffer();
    void update_buffer();

    frame get_frame(std::size_t frame_index);

    std::size_t last_buffer_index();
    bool frame_exists(std::size_t frame_index);

  public:
    Program();


    // •   main()
    // •   play()

    // <-  data()
    // <-> update(data)

    // <-> read(image, data)  / writes to image data, returns play data

    //  -> quit

    nlohmann::json data();

    nlohmann::json update(nlohmann::json data);

    void read(cv::Mat& image, cv::Mat& audio, std::size_t frame_index);

    void buffer(nlohmann::json data, cv::Mat& image);

    void quit();


    // void preview(std::vector<cv::Mat>& images, stk::StkFrames& audio);

    // void save();

};

#endif /* program_h */
