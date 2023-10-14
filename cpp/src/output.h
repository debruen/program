
#ifndef output_h
#define output_h

#include "output/hsl.h"

class Output {

  private:

    nlohmann::json m_data;

    Out* m_base;

  public:
    Output();

    nlohmann::json data();

    nlohmann::json data(nlohmann::json data);

    void process(std::vector<cv::Mat>& images, stk::StkFrames& audio);

    void process(stk::StkFrames& audio, std::vector<cv::Mat>& images);

};

#endif /* output_h */
