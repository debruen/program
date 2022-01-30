
#ifndef film_h
#define film_h

// fillings
#include "spectrum.h"

class Film {

  private:

    std::string m_fill_type;

    nlohmann::json m_data;

    Coat* m_fill;

  public:

    Film();

    nlohmann::json init();

    nlohmann::json update(nlohmann::json data);

    // width, height and length should go to m_data
    cv::Mat frame(std::size_t frame, std::size_t width, std::size_t height);

    stk::StkFrames frame(std::size_t length);

    std::vector<cv::Mat> images(std::size_t frames, std::size_t width, std::size_t height);

};

#endif // film_h END
