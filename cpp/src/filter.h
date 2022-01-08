
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

    nlohmann::json init();

    nlohmann::json update(nlohmann::json data, std::string type);

    void process(std::vector<cv::Mat>& images, stk::StkFrames& audio);

};

#endif /* filter_h */
