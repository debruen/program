
#ifndef coat_h
#define coat_h

#include "../data.h"

class Coat {

private:

  nlohmann::json m_coat = nlohmann::json::array();

  std::string m_type{"spectrum"};

protected:

  Coat();

  nlohmann::json get_data();

public:

  virtual nlohmann::json init() = 0;

  virtual nlohmann::json update(nlohmann::json data) = 0;

  virtual cv::Mat image(std::size_t width, std::size_t height) = 0;

  virtual stk::StkFrames audio(std::size_t length) = 0;

};

#endif // coat_h END
