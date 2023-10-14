
#include "out.h"

Out::Out() {

  /// 0: output type selection
  std::vector<std::string> type_options {"hsl"};
  nlohmann::json type = data::init_str("type", type_options, type_options[0]);
  _base_data.push_back(type);

}

nlohmann::json Out::get_data() {

  return _base_data;
}

void Out::set_value(std::string value) {

  _base_data[0]["value"] = value;
}
