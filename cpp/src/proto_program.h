
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

#include "control.h"
// #include "record.h"

class Program {

  private:

    std::vector<frame> m_buffer;
    std::mutex m_buffer_mutex;

    info m_info;
    std::mutex m_info_mutex;

    Synthesis m_synthesis;
    Control   m_control;

  public:
    Program();

    nlohmann::json init_synthesis();
    nlohmann::json data_synthesis(nlohmann::json data);

    nlohmann::json init_control();
    nlohmann::json data_control(nlohmann::json data);

    void display(cv::Mat image);

    bool quit();

};

#endif /* program_h */
