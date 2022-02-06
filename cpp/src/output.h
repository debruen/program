
#ifndef output_h
#define output_h

#include "out/hsl.h"

class Output {

  private:

    std::string m_type;

    nlohmann::json m_data;

    Out* m_base;

  public:
    Output();

    nlohmann::json data();

    nlohmann::json update(nlohmann::json data, std::string type);

    void image_frame(cv::Mat& image, cv::Mat& audio, std::size_t frame_index);

    void audio_frame(cv::Mat& image, cv::Mat& audio, std::size_t frame_index);


    void process(std::vector<cv::Mat>& images, stk::StkFrames& audio);

};

#endif /* output_h */
