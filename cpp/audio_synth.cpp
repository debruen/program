#include "audio_synth.h"

AudioSynth::AudioSynth() {

}

void AudioSynth::oscillator(stk::StkFrames& audio, double frequency, double phase, double amplitude, double lrfade, double (*mergeFcn)(double&, double&, double&), double opacity) {
    std::cout << "audio track: oscillator START" << std::endl;

    double freq, base, tone, left, right;
    unsigned int frames;
    stk::SineWave sineL, sineR;

    freq = 27.5000;
    base = pow(2, 1 / 12.0);
    tone = pow(base, frequency * 88) * freq;
    frames = audio.frames();

    sineL.setFrequency(tone);
    sineR.setFrequency(tone);

    sineL.addPhaseOffset(phase);
    sineR.addPhaseOffset(phase);

    stk::StkFrames framesL(frames, 1), framesR(frames, 1);

    framesL.setChannel(0, audio, 0);
    framesR.setChannel(0, audio, 1);

    for(unsigned int i = 0; i < frames; ++i) {

      left = sineL.tick() * amplitude * lrfade;
      right = sineR.tick() * amplitude * (1 - lrfade);

      framesL[i] = mergeFcn(framesL[i], left, opacity);
      framesR[i] = mergeFcn(framesR[i], right, opacity);

    }

    audio.setChannel(0, framesL, 0);
    audio.setChannel(1, framesR, 0);

    std::cout << "audio track: oscillator END" << std::endl;
} // oscillator
