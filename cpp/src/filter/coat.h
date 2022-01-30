
#ifndef coat_h
#define coat_h

#include "../data.h"
#include "../math.h"

class Coat {

private:

  nlohmann::json m_coat = nlohmann::json::array();

  std::string m_type{"spectrum"};

protected:

  Coat();

  nlohmann::json coat_data();

public:

  virtual nlohmann::json data() = 0;

  virtual nlohmann::json update(nlohmann::json data) = 0;

  virtual cv::Mat image_frame(cv::Mat& image, std::size_t frame_index) = 0;

  virtual cv::Mat audio_frame(cv::Mat& audio, std::size_t frame_index) = 0;


  virtual cv::Mat frame(std::size_t width, std::size_t height) = 0;

  virtual stk::StkFrames frame(std::size_t length) = 0;

};

#endif // coat_h END
