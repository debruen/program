#ifndef image_out_h
#define image_out_h

#include <iostream>
#include <string>
#include <vector>
#include <thread>
#include <mutex>
#include <algorithm>
#include <cmath>

#include "nlohmann/json.hpp"

#include "opencv2/core.hpp"
#include "opencv2/highgui.hpp"
#include "opencv2/imgproc.hpp"

#include "stk/SineWave.h"

#include "wtc.h"

class ImageOut {

private:

  nlohmann::json m_default;

  int m_channel_a, m_channel_b, m_channel_c, m_keys;
  double m_narrowing;

  std::vector<stk::SineWave> sineL, sineR;
  void setSine(unsigned int bands);

public:

  ImageOut();

  nlohmann::json get_default();
  void set_data(nlohmann::json data);

  void hsl_to_signal(cv::Mat& image, stk::StkFrames& audio, nlohmann::json data, unsigned int page);
  void rgb_to_chord(cv::Mat& image, stk::StkFrames& audio, nlohmann::json data, unsigned int page);

};

#endif /* image_out_h */
