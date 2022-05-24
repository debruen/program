
#ifndef out_h
#define out_h

#include <random>

#include "nlohmann/json.hpp"

#include "opencv2/core.hpp"
#include "opencv2/imgproc.hpp"
#include "opencv2/imgcodecs.hpp"

#include "stk/SineWave.h"

#include "fftw3.h"

#include "../../functionality/data.h"

class Out {

  private:

    nlohmann::json _base_data;

  protected:

    Out();

    nlohmann::json get_data();

    void set_value(std::string data);

  public:

    virtual nlohmann::json data() = 0;

    virtual nlohmann::json update(nlohmann::json data) = 0;

    virtual void image_frame(cv::Mat& image, cv::Mat& audio, std::size_t frame_index) = 0;

    virtual void audio_frame(cv::Mat& image, cv::Mat& audio, std::size_t frame_index) = 0;

};

#endif /* out_h */
