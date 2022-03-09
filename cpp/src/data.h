
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

  nlohmann::json init_1d_float_data(std::string name, std::vector<double> values);
  nlohmann::json init_2d_uchar_data(std::string name, std::vector< std::vector<unsigned char> > values);

  // math functions
  double normalize(double min, double max, double value);
  double project(double min, double max, double value);

  // create data object
  nlohmann::json data_int(std::string name, int min = 0, int max = 256, int value = 0, bool select = true);
  nlohmann::json data_time(std::string name, int min = 1000, int max = 10000, int value = 1000, bool select = true);
  nlohmann::json data_float(std::string name, double min = 0.0, double max = 1.0, double value = 0, bool select = true);
  nlohmann::json data_value(std::string name, double value);
  nlohmann::json data_path(std::string name, std::string value);

  // get data value
  double get_value(nlohmann::json& data, std::string name);

  nlohmann::json get_data(nlohmann::json& data, std::string name);

  nlohmann::json get_array(nlohmann::json& data, std::string name);
  void set_array(nlohmann::json& data, std::string name, nlohmann::json array);


  std::string get_string(nlohmann::json& data, std::string name);

  unsigned int get_width(nlohmann::json& data, std::string name);
  unsigned int get_height(nlohmann::json& data, std::string name);


  void compute_size(nlohmann::json& data, std::string name, std::string area, double ratio);

  void spectral_rgb(double& r, double& g, double& b, double l);
  std::vector< std::vector<unsigned char> > rgb_spectrum();

  nlohmann::json data_object(std::string name, std::string type, std::vector<std::string> options, std::string value, bool select);

  nlohmann::json data_array(std::string name, bool select = true);

  nlohmann::json data_bool(std::string name, bool select = true, bool value = true);
  nlohmann::json data_button(std::string name, bool select = true, bool value = true);

  nlohmann::json data_string(std::string name, bool select = true, std::vector<std::string> options = {}, std::string value = "");

  nlohmann::json data_size(std::string name, bool select = false, int width = 0, int height = 0);

  nlohmann::json data_mask(std::string name, std::string type, bool select = true);

  nlohmann::json data_spectral_color(std::string name);

  nlohmann::json data_data(std::string name, double min, double max);

  nlohmann::json data_sine(std::string name, bool select = true);

}

#endif /* data_h */
