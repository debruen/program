
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

    std::vector<cv::Mat> images(std::size_t frames, std::size_t width, std::size_t height);

    stk::StkFrames audio(std::size_t length);

};

#endif // film_h END
