
#ifndef filtration_h
#define filtration_h

#include "filter/fill.h"
#include "filter/random.h"
#include "filter/gradient.h"
#include "filter/rectangle.h"

class Filtration {

  private:

    nlohmann::json m_data;

    std::vector< Filter* > m_filter;

    std::string get_type(nlohmann::json data);

  public:
    Filtration();

    nlohmann::json data();

    nlohmann::json data(nlohmann::json data, std::string type);

    void process(std::vector<cv::Mat>& images, stk::StkFrames& audio, std::string type);

    // void process(stk::StkFrames& audio, std::vector<cv::Mat>& images);

};

#endif /* filtration_h */
