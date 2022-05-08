
#ifndef control_h
#define control_h

#include "control/play.h"
#include "control/record.h"

class Control {

  private:

    Play   m_play;
    Record m_record;

    nlohmann::json m_data;

  public:
    Control(std::vector<frame>& buffer, std::mutex& buffer_mutex, info& info);

    nlohmann::json init();
    nlohmann::json data(nlohmann::json data);

    nlohmann::json new_frame();
    void display(cv::Mat& image, cv::Mat& left, cv::Mat& right);

    bool record();

    // bool quit();

};

#endif /* control_h */
