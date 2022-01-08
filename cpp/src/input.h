
#ifndef input_h
#define input_h

// #include <fstream>
// #include <vector>

#include "nlohmann/json.hpp"

#include "opencv2/core.hpp"
#include "opencv2/imgcodecs.hpp"
#include "opencv2/imgproc.hpp"
#include "opencv2/highgui.hpp"

#include "stk/FileRead.h"

class Input {

  private:

    nlohmann::json m_data = nlohmann::json::array();

    void crop(cv::Mat& image, unsigned int width, unsigned int height);

    void flip(cv::Mat& image, bool back = false);

  public:
    Input();

    nlohmann::json data();

    nlohmann::json data(nlohmann::json data);

    void process(std::vector<cv::Mat>& images, stk::StkFrames& audio);

    void flip_back(std::vector<cv::Mat>& images);
};

#endif /* input_h */
