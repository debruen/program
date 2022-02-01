
#ifndef hsl_h
#define hsl_h

#include "out.h"

class Hsl : public Out {

  private:

    nlohmann::json hsl_data;

    double normDouble(double min, double max, double value); // returns 0.0 - 1.0 from value out of range min max

    std::vector<double> minMax(std::vector<double>& v);

    std::vector<double> fft(std::vector<double> input);

    void norm3dDouble(std::vector< std::vector< std::vector<double> > >& vector);

  public:
    Hsl();

    virtual nlohmann::json data();

    virtual nlohmann::json update(nlohmann::json data);

    virtual void image_frame(cv::Mat& image, cv::Mat& audio, std::size_t frame_index);

    virtual void audio_frame(cv::Mat& image, cv::Mat& audio, std::size_t frame_index);


    virtual void process(std::vector<cv::Mat>& images, stk::StkFrames& audio);

    virtual void process(stk::StkFrames& audio, std::vector<cv::Mat>& images);

};

#endif /* hsl_h */
