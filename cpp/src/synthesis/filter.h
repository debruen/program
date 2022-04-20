
#ifndef filter_h
#define filter_h

#include "filter/layer.h"

class Filter {

  private:

    nlohmann::json m_data = nlohmann::json::array();

    std::vector< Layer* > m_layer;

  public:
    Filter();

    nlohmann::json data();

    nlohmann::json update(nlohmann::json data);

    void image_frame(cv::Mat& image, std::size_t frame_index, std::string type);

    void audio_frame(cv::Mat& audio, std::size_t frame_index, std::string type);

};

#endif /* filter_h */
