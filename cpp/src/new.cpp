
#include "new.h"

Color::Color() {

  m_data["name"]     = "color";
  m_data["input"]    = "color";

  m_data["space"]    = nlohmann::json::object();
  m_data["space"]["input"]    = "select";
  m_data["space"]["name"]   = "color space";
  m_data["space"]["options"] = {"RGB", "HSL"}; // "Lab", "LCh"
  m_data["space"]["value"]   = "RGB";

  m_data["min"]      = 0.0;
  m_data["max"]      = 1.0;
  m_data["channelA"] = 0.5;
  m_data["channelB"] = 0.5;
  m_data["channelC"] = 0.5;

}

nlohmann::json Color::data() {

  return m_data;
}
