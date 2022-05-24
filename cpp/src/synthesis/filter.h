
#ifndef filter_h
#define filter_h

#include "filter/layer.h"

class Filter {

  private:

    std::string _name = "filter";

    nlohmann::json m_data = nlohmann::json::array();

    std::vector< Layer* > m_layer;

  public:
    Filter();

    nlohmann::json data();
    nlohmann::json update(nlohmann::json data);

    void image(cv::Mat& image, std::size_t index);
    void audio(cv::Mat& audio, std::size_t index);

};

#endif /* filter_h */
