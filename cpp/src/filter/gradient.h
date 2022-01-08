
#ifndef gradient_h
#define gradient_h

#include "filter.h"

class Gradient : public Filter {

  private:

    nlohmann::json gradient_data;

    nlohmann::json get_fillA(std::string type);

    nlohmann::json get_fillB(std::string type);

  public:
    Gradient(std::string type);

    virtual nlohmann::json data(std::string type);

    virtual nlohmann::json data(nlohmann::json data, std::string type);

    virtual void process(std::vector<cv::Mat>& images);

    virtual void process(stk::StkFrames& audio);

};

#endif // gradient_h END
