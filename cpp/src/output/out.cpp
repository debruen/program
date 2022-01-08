
#include "out.h"

Out::Out() {

  /// 0: output type selection
  nlohmann::json type;
  type["label"]   = "type";
  type["select"]  = "select";  // select, range, files
  type["options"] = {"hsl", "rgb"};
  type["default"] = type["options"][0];
  type["value"]   = type["default"];

  out_data.push_back(type);

}

nlohmann::json Out::get_data() {

  return out_data;
}

nlohmann::json Out::get_size() {

  nlohmann::json size;
  size["label"]   = "size";
  size["select"]  = "select";  // select, range, files
  size["options"] = {"A6", "A5", "A4", "A3", "A2"};
  size["default"] = size["options"][2];
  size["value"]   = size["default"];

  return size;
}

nlohmann::json Out::get_save() {

  nlohmann::json save;
  save["label"]   = "save";
  save["select"]  = "button";  // select, range, files, button
  save["value"]   = false;

  return save;
}

void Out::set_value(std::string value) {

  out_data[0]["value"] = value;

}

void Out::set_sine(unsigned int bands) {
    std::vector<stk::SineWave> sine_l(bands);
    std::vector<stk::SineWave> sine_r(bands);

    m_sine_l = sine_l;
    m_sine_r = sine_r;

    std::mt19937 generator(8);
    std::uniform_real_distribution<double> distribution(-1.0,1.0);

    std::vector<double> phase;
    for (unsigned int i = 0; i < bands; ++i) {
        phase.push_back(distribution(generator));
    }
    for (unsigned int i = 0; i < bands; ++i) {
      m_sine_l[i].addPhase(phase[i]);
      m_sine_r[i].addPhase(phase[(bands - 1) - i]);
    }
}
