
#ifndef filter_h
#define filter_h

#include "filter/layer.h"

class Filter {

  private:

    std::string m_type;

    nlohmann::json m_data;

    std::vector< Layer* > m_layer;

  public:
    Filter();

    nlohmann::json data();

    nlohmann::json update(nlohmann::json data, std::string type);

    void image_frame(cv::Mat& image, std::size_t frame_index);

    void audio_frame(cv::Mat& audio, std::size_t frame_index);

};

#endif /* filter_h */
