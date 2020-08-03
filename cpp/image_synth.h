
#ifndef image_synth_h
#define image_synth_h

#include <iostream>
#include <string>
#include <vector>
#include <cmath>
#include <random>

#include "nlohmann/json.hpp"

#include "opencv2/core.hpp"
#include "opencv2/highgui.hpp"
#include "opencv2/imgproc.hpp"

#include "wtc.h"
#include "who.h"

typedef uchar (*blendchar)(unsigned char&, unsigned char&, double&);

class ImageSynth {

private:

  uchar m_temp_color_a, m_temp_color_b, m_temp_color_c, m_temp_color_d, m_temp_color_e, m_temp_color_f;
  double m_temp_opacity, m_temp_frequency, m_temp_phase, m_temp_tilt, m_temp_width, m_temp_height, m_temp_center;

  blendchar m_temp_blend;

  blendchar get_blend(std::string blend);
  void set_data(nlohmann::json data);

  double point_gradient(unsigned int y, unsigned int x, unsigned int w, unsigned int h, double frequency = 1.0, double phase = 0.0, double tilt = 0.0);

  void fill(cv::Mat& image);
  void noise(cv::Mat& image);
  void walker(cv::Mat& image);
  void gradient(cv::Mat& image, int page = 0);
  void rectangle(cv::Mat& image, int page = 0);

public:

  ImageSynth();

  void synthesis(cv::Mat& image, nlohmann::json data, int page = 0);

};

#endif /* image_synth_h */
