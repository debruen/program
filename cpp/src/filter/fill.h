
#ifndef fill_h
#define fill_h

#include "filter.h"

class Fill : public Filter {

  private:

    nlohmann::json m_fill_data;

    nlohmann::json get_fill(std::string type);

  public:
    Fill(std::string type);

    virtual nlohmann::json data(std::string type);

    virtual nlohmann::json data(nlohmann::json data, std::string type);

    virtual void process(std::vector<cv::Mat>& images);

    virtual void process(stk::StkFrames& audio);

};

#endif // fill_h END
