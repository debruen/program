#ifndef image_filter_h
#define image_filter_h

#include <iostream>
#include <string>
#include <cmath>
#include <vector>

#include "nlohmann/json.hpp"

#include "opencv2/core.hpp"
#include "opencv2/highgui.hpp"
#include "opencv2/imgproc.hpp"

#include "wtc.h"

class ImageFilter {

private:

  nlohmann::json m_default;

public:

  ImageFilter();

  nlohmann::json get_default();

  void color_scale(cv::Mat& image, nlohmann::json data);

  void color_strech(cv::Mat& image, nlohmann::json data);

  void color_jump(cv::Mat& image, nlohmann::json data);

  void gamma(cv::Mat& image, nlohmann::json data);

};

#endif /* image_filter_h */
