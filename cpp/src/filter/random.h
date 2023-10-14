
#ifndef random_h
#define random_h

#include "filter.h"

class Random : public Filter {

  private:

    nlohmann::json random_data;

    nlohmann::json get_fillA(std::string type);

    nlohmann::json get_fillB(std::string type);

  public:
    Random(std::string type);

    virtual nlohmann::json data(std::string type);

    virtual nlohmann::json data(nlohmann::json data, std::string type);

    virtual void process(std::vector<cv::Mat>& images);

    virtual void process(stk::StkFrames& audio);

};

#endif // random_h END
