#ifndef audio_synth_h
#define audio_synth_h

#include <iostream>
#include <string>
#include <vector>
#include <cmath>

#include "stk/SineWave.h"

#include "who.h"

class AudioSynth {

private:


public:

    AudioSynth();

    void oscillator(stk::StkFrames& audio, double frequency, double phase, double amplitude, double lrfade, double (*mergeFcn)(double&, double&, double&) = who::blend::normal, double opacity = 1);

};

#endif /* audio_synth_h */
