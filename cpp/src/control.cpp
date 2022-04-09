
#include "control.h"

Control::Control(std::vector<frame>& buffer, std::mutex& mutex)
    : m_play(buffer, mutex), m_record(buffer, mutex) {

  m_data.push_back(data::init_bool("play", false));
  m_data.push_back(data::init_bool("ready", false));
  m_data.push_back(data::init_bool("record", false));
  m_data.push_back(data::init_bool("reset", false));

}

nlohmann::json Control::init() {

  return m_data;
}

nlohmann::json Control::data(nlohmann::json data) {

  bool play, ready, record, reset;

  play   = data::get_bool(data, "play");
  ready  = data::get_bool(data, "ready");
  record = data::get_bool(data, "record");
  reset  = data::get_bool(data, "reset");


  m_data = data;

  return m_data;
}
