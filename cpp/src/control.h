
#ifndef control_h
#define control_h

#include "control/play.h"
#include "control/record.h"

class Control {

  private:

    Play   m_play;
    Record m_record;

    nlohmann::json m_data = nlohmann::json::array();

  public:
    Control(std::vector<frame>& buffer);

    nlohmann::json data();

    nlohmann::json update(nlohmann::json data);

};

#endif /* control_h */
