
#ifndef specone_h
#define specone_h

#include "out.h"

class Specone : public Out {

  private:

    nlohmann::json specone_data;

    double normDouble(double min, double max, double value); // returns 0.0 - 1.0 from value out of range min max

    std::vector<double> minMax(std::vector<double>& v);

    std::vector<double> fft(std::vector<double> input);

    void norm3dDouble(std::vector< std::vector< std::vector<double> > >& vector);

  public:
    Specone();

    virtual nlohmann::json data();

    virtual nlohmann::json data(nlohmann::json data);

    virtual void process(std::vector<cv::Mat>& images, stk::StkFrames& audio);

    virtual void process(stk::StkFrames& audio, std::vector<cv::Mat>& images);

};

#endif /* specone_h */
