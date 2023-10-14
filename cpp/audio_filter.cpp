#include "audio_filter.h"

AudioFilter::AudioFilter() {

}

void AudioFilter::arg(stk::StkFrames& audio, double a, double b, double c) {

  unsigned int frames = audio.frames();

  stk::StkFrames framesL(frames, 1), framesR(frames, 1);

  framesL.setChannel(0, audio, 0);
  framesR.setChannel(0, audio, 1);

  stk::Noise noiseA, noiseB;
  stk::BiQuad biquadA, biquadB;
  biquadA.setResonance( 440.0, 0.98, true ); // automatically normalize for unity peak gain
  biquadB.setResonance( 440.0, 0.98, true ); // automatically normalize for unity peak gain

  biquadA.tick( framesL );
  biquadB.tick( framesR );

  audio.setChannel(0, framesL, 0);
  audio.setChannel(1, framesR, 0);
} // oscillator
