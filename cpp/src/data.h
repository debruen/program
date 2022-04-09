
#ifndef data_h
#define data_h

#include <iostream>
#include <string>
#include <vector>
#include <cmath>
#include <random>

#include "nlohmann/json.hpp"

#include "opencv2/core.hpp"
#include "opencv2/imgcodecs.hpp"
#include "opencv2/imgproc.hpp"

#include "stk/SineWave.h"

typedef struct {
  std::size_t index;
  cv::Mat image;
  cv::Mat audio;
} frame;

typedef struct {
  int channels;
  int time;
  std::size_t frames;
  bool update;
  bool next; // display
  bool full;
} info;

namespace data {

  // internal functions
  std::size_t array_size(nlohmann::json& data);

  // string functions
  nlohmann::json init_str(std::string name, std::vector<std::string> options, std::string value);
  std::string get_str(nlohmann::json& data, std::string name);

  // int functions
  nlohmann::json init_int(std::string name, std::size_t min, std::size_t max, std::size_t value);
  std::size_t get_int(nlohmann::json& data, std::string name);

  // time functions
  nlohmann::json init_time(std::string name, std::size_t min, std::size_t max, std::size_t value);

  // float functions
  nlohmann::json init_float(std::string name, double min, double max, double value);
  double get_float(nlohmann::json& data, std::string name);

  // boolean
  nlohmann::json init_bool(std::string name, bool value = true);
  bool get_bool(nlohmann::json& data, std::string name);

  nlohmann::json init_button(std::string name, bool value = true);

  // value
  nlohmann::json init_value(std::string name, double value);
  double get_value(nlohmann::json& data, std::string name);

  nlohmann::json init_min_max(std::string name, double min, double max);

  nlohmann::json init_1d_float_data(std::string name, std::vector<double> values);
  nlohmann::json init_2d_uchar_data(std::string name, std::vector< std::vector<unsigned char> > values);

}

#endif /* data_h */
