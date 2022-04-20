
#ifndef program_h
#define program_h

#include "synthesis.h"
#include "control.h"

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

    bool new_frame();
    void display(cv::Mat& image);
    bool record();

    bool quit();
};

#endif /* program_h */
