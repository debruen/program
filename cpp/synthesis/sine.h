
#ifndef sine_h
#define sine_h

#include "functions.h"

class Sine {

private:

  // type settings
  typ m_type;

  // blend settings
  std::string m_blend;
  blendimage m_image_blend;
  blendaudio m_audio_blend;
  double m_opacity;

  // color settings
  uchar m_ar, m_ag, m_ab, m_br, m_bg, m_bb;

  // sine settings
  double m_frequency, m_amplitude, m_phase, m_shift, m_tilt, m_balance;

public:

  Sine(typ type = IMAG);

  nlohmann::json init();

  nlohmann::json init(nlohmann::json settings);

  void compute(cv::Mat& image, int frame = 0);

  void compute(stk::StkFrames& audio);

};

#endif /* sine_h */
