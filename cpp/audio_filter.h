#ifndef audio_filter_h
#define audio_filter_h

#include <iostream>
#include <string>
#include <vector>
#include <cmath>

#include "stk/SineWave.h"
#include "stk/BiQuad.h"
#include "stk/Noise.h"

#include "wtc.h"

class AudioFilter {

private:

public:

    AudioFilter();

    void arg(stk::StkFrames& audio, double a, double b, double c);

};

#endif /* audio_filter_h */
