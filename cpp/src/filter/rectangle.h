
#ifndef rectangle_h
#define rectangle_h

#include "filter.h"

class Rectangle : public Filter {

  private:

    nlohmann::json rectangle_data;

    nlohmann::json get_fillA(std::string type);

    nlohmann::json get_fillB(std::string type);

  public:
    Rectangle(std::string type);

    virtual nlohmann::json data(std::string type);

    virtual nlohmann::json data(nlohmann::json data, std::string type);

    virtual void process(std::vector<cv::Mat>& images);

    virtual void process(stk::StkFrames& audio);

};

#endif // rectangle_h END
