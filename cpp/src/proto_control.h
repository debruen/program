
#ifndef control_h
#define control_h

#include "control/player.h"
#include "control/recorder.h"

class Control {

  private:

    Player   m_player;
    Recorder m_recorder;

    nlohmann::json m_data;

  public:
    Control(std::vector<frame>& buffer, std::mutex& buffer_mutex, info& info, std::mutex& info_mutex);

    nlohmann::json init();

    nlohmann::json data(nlohmann::json data);

    void display(cv::Mat image);

    bool quit();

};

#endif /* control_h */
