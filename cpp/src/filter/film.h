
#ifndef film_h
#define film_h

// fillings
#include "spectrum.h"

class Film {

  private:

    std::string m_coat_type;

    nlohmann::json m_data;

    Coat* m_coat;

  public:

    Film();

    nlohmann::json data();

    nlohmann::json update(nlohmann::json data);

    cv::Mat image_frame(cv::Mat& image, std::size_t frame_index);

    cv::Mat audio_frame(cv::Mat& audio, std::size_t frame_index);

};

#endif // film_h END
