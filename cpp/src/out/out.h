
#ifndef out_h
#define out_h

#include <random>

#include "nlohmann/json.hpp"

#include "opencv2/core.hpp"
#include "opencv2/imgproc.hpp"
#include "opencv2/imgcodecs.hpp"

#include "stk/SineWave.h"

#include "fftw3.h"

#include "../data.h"

class Out {

  private:

    nlohmann::json out_data;

  protected:

    Out();

    nlohmann::json get_data();
    nlohmann::json get_save();

    void set_value(std::string data);

    std::vector<stk::SineWave> m_sine_l, m_sine_r;
    void set_sine(unsigned int bands);

    std::vector< std::vector<stk::SineWave> > make_sine(unsigned int bands);

  public:

    virtual nlohmann::json data() = 0;

    virtual nlohmann::json update(nlohmann::json data) = 0;

    virtual void image_frame(cv::Mat& image, cv::Mat& audio, std::size_t frame_index) = 0;

    virtual void audio_frame(cv::Mat& image, cv::Mat& audio, std::size_t frame_index) = 0;



    virtual void process(std::vector<cv::Mat>& images, stk::StkFrames& audio) = 0;

    virtual void process(stk::StkFrames& audio, std::vector<cv::Mat>& images) = 0;

};

#endif /* out_h */
