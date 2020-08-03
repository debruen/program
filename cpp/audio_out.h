#ifndef audio_out_h
#define audio_out_h

#include <iostream>
#include <string>
#include <vector>
#include <cmath>
#include <random>

#include "opencv2/core.hpp"
#include "opencv2/highgui.hpp"
#include "opencv2/imgproc.hpp"

#include "stk/SineWave.h"

#include "wtc.h"

class AudioOut {

private:

public:

    AudioOut();

    void spectogramm(stk::StkFrames& audio, cv::Mat& image);

    void stereo_spectogramm(stk::StkFrames& audio, cv::Mat& image);
    void stereo_shape(stk::StkFrames& audio, cv::Mat& image);
};

#endif /* audio_out_h */
