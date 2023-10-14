
#include "sine.h"

Sine::Sine(typ type) {

  m_type = type;

  m_blend = "normal";

  if(m_type == IMAG) {
    func::get_blend(m_image_blend, m_blend);
  }
  if(m_type == AUDI) {
    func::get_blend(m_audio_blend, m_blend);
  }

  m_opacity = 1;

  if(m_type == IMAG) {
    m_ar = 0;
    m_ag = 0;
    m_ab = 0;
    m_br = 255;
    m_bg = 255;
    m_bb = 255;
  }

  m_frequency = 1;
  m_amplitude = 1;
  m_phase = 0;
  m_shift = 0;

  if(m_type == IMAG) {
    m_tilt = 0;
  }
  if(m_type == AUDI) {
    m_balance = 0.5;
  }
}

nlohmann::json Sine::init() {

  nlohmann::json settings;

  // blend settings
  settings["opacity"] = m_opacity;
  settings["blend"] = m_blend;

  // color settings
  if(m_type == IMAG) {
    settings["ar"] = m_ar;
    settings["ag"] = m_ag;
    settings["ab"] = m_ab;
    settings["br"] = m_br;
    settings["bg"] = m_bg;
    settings["bb"] = m_bb;
  }

  // sine settings
  settings["frequency"] = m_frequency;
  settings["amplitude"] = m_amplitude;
  settings["phase"] = m_phase;
  settings["shift"] = m_shift;
  if(m_type == IMAG) {
    settings["tilt"] = m_tilt;
  }
  if(m_type == AUDI) {
    settings["balance"] = m_balance;
  }

  return settings;
}

nlohmann::json Sine::init(nlohmann::json settings) {

  // blend settings
  m_opacity = settings["opacity"];
  m_blend = settings["blend"];

  if(m_type == IMAG) {
    func::get_blend(m_image_blend, m_blend);
  }
  if(m_type == AUDI) {
    func::get_blend(m_audio_blend, m_blend);
  }

  // color settings
  if(m_type == IMAG) {
    m_ar = settings["ar"];
    m_ag = settings["ag"];
    m_ab = settings["ab"];
    m_br = settings["br"];
    m_bg = settings["bg"];
    m_bb = settings["bb"];
  }

  // sine settings
  m_frequency = settings["frequency"];
  m_amplitude = settings["amplitude"];
  m_phase = settings["phase"];
  m_shift = settings["shift"];
  if(m_type == IMAG) {
    m_tilt = settings["tilt"];
  }
  if(m_type == AUDI) {
    m_balance = settings["balance"];
  }

  return settings;
}

void Sine::compute(cv::Mat& image, int frame) {
  double value, pm;
  unsigned int width, height;
  uchar r, g, b;

  m_tilt = func::number::circle(0, 1, m_tilt);

    for (int i = 0; i < frame; i++) {
      if(m_tilt <= 0.25) {
          pm = 1 - m_tilt * 4;
      } else if (m_tilt <= 0.5) {
          pm = (m_tilt - 0.25) * (-4);
      } else if (m_tilt <= 0.75) {
          pm = 1 - (m_tilt - 0.5) * 4;
      } else {
          pm = (m_tilt - 0.75) * (-4);
      }
      m_phase += m_frequency * pm;
    }

    width = static_cast<unsigned int>(image.cols);
    height = static_cast<unsigned int>(image.rows);

    unsigned int c;
    uchar* ptr;
    for (unsigned int y = 0; y < height; y++) {

      ptr = image.ptr<uchar>(y);
      for (unsigned int x = 0; x < width; x++) {
        c = x * 3;

        value = func::point_gradient(y, x, width, height, m_frequency, m_phase, m_tilt);

        r = func::scale::projectChar(m_ar, m_br, value);
        g = func::scale::projectChar(m_ag, m_bg, value);
        b = func::scale::projectChar(m_ab, m_bb, value);

        ptr[c + 2] = m_image_blend(ptr[c + 2], r, m_opacity);
        ptr[c + 1] = m_image_blend(ptr[c + 1], g, m_opacity);
        ptr[c] = m_image_blend(ptr[c], b, m_opacity);
      }
    }
}

void Sine::compute(stk::StkFrames& audio) {

  double freq, base, tone, left, right;
  unsigned int frames;
  stk::SineWave sineL, sineR;

  freq = 27.5000;
  base = pow(2, 1 / 12.0);
  tone = pow(base, m_frequency * 88) * freq;
  frames = audio.frames();

  sineL.setFrequency(tone);
  sineR.setFrequency(tone);

  sineL.addPhaseOffset(m_phase);
  sineR.addPhaseOffset(m_phase);

  stk::StkFrames framesL(frames, 1), framesR(frames, 1);

  framesL.setChannel(0, audio, 0);
  framesR.setChannel(0, audio, 1);

  // first tick sine into frames and then do the calculations !! in mono

  for(unsigned int i = 0; i < frames; ++i) {

    left = sineL.tick() * m_amplitude * m_balance;
    right = sineR.tick() * m_amplitude * (1 - m_balance);

    framesL[i] = m_audio_blend(framesL[i], left, m_opacity);
    framesR[i] = m_audio_blend(framesR[i], right, m_opacity);

  }

  audio.setChannel(0, framesL, 0);
  audio.setChannel(1, framesR, 0);
}
