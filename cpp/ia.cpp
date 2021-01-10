
#include "ia.h"

/**
 * A brief history of JavaDoc-style (C-style) comments.
 *
 * This is the typical JavaDoc-style C-style comment. It starts with two
 * asterisks.
 *
 * @param theory Even if there is only one possible unified theory. it is just a
 *               set of rules and equations.
 */

Program::Program(std::string data) {

  nlohmann::json temp = nlohmann::json::parse(data);

  m_data["mode"] = "image";
  m_data["filename"] = temp["filename"];
  m_data["output"] = temp["output"];

  m_data["pages"] = 1;
  m_data["time"] = 2.96;

  m_data["files"] = nlohmann::json::array();
  m_data["processes"] = nlohmann::json::array();
  m_data["out"] = m_imageout.get_default();

  init();
}

void Program::init(SIZE size) {

  // init sizes
  m_size   = size;
  m_width  = wtc::width(m_size);
  m_height = wtc::height(m_size);
  m_bands  = wtc::bands(m_size);

  cv::Size temp_size(m_width, m_height);
  cv::Mat temp(temp_size, CV_8UC3);
  temp = 0;
  m_map = temp;

  // image in

  m_image_in["mode"] = m_mode;
  m_image_in["pages"] = 1;
  m_image_in["files"] = nlohmann::json::array();
  m_image_in["processes"] = nlohmann::json::array();

  // layer

  m_image_layer["process"] = "layer";
  m_image_layer["type"] = "fill";
  m_image_layer["blend"] = "normal";
  m_image_layer["opacity"] = 0.5;
  m_image_layer["color"] = { 0, 0, 0, 255, 255, 255 };
  m_image_layer["gradient"] = { 1.0, 0.0, 0.0 };
  m_image_layer["rectangle"] = { 0.5, 0.5, 0.5 };

  // image filter

  m_image_filter["process"] = "filter";
  m_image_filter["type"] = "color_scale";
  m_image_filter["color"] = { 0, 0, 0, 255, 255, 255 };
  m_image_filter["gamma"] = 1.0;

  // audio in

  m_audio_in["mode"] = m_mode;
  m_audio_in["time"] = 2.96;
  m_audio_in["files"] = nlohmann::json::array();
  m_audio_in["processes"] = nlohmann::json::array();

  // track

  m_audio_track["process"] = "track";
  m_audio_track["type"] = "oscillator";
  m_audio_track["blend"] = "normal";
  m_audio_track["opacity"] = 0.5;
  m_audio_track["sine"] = { 0.0, 0.0, 1.0 };
  m_audio_track["lrfade"] = 0.5;

  // audio filter

  m_audio_filter["process"] = "filter";
  m_audio_filter["type"] = "arg";
  m_audio_filter["arg"] = { 0.0, 0.0, 0.0 };

  // image out

  m_image_out["mode"] = m_mode;
  m_image_out["out_mode"] = "hsl_to_signal";
  m_image_out["time"] = 2.96;
  m_image_out["pages"] = 1;
  m_image_out["time_image"] = 2.96;
  m_image_out["narrowing"] = 0.0;
  m_image_out["channel_a"] = 0;
  m_image_out["channel_b"] = 1;
  m_image_out["channel_c"] = 2;
  m_image_out["keyboard"] = 97;

  // audio out

  m_audio_out["mode"] = m_mode;
  m_audio_out["out_mode"] = "stereo_spectogram";
  m_audio_out["time"] = 2.96;
  m_audio_out["pages"] = 1;
  m_audio_out["image_time"] = 1;
}

std::string Program::data(std::string data) {

  return data;
}

// bool Program::set_data(std::string data) {
//
//   return true;
// }
//
// std::string Program::get_data() {
//   std::string data = m_data.dump();
//
//   return data;
// }

// ----------------------------------------------------------------------------- main settings

bool Program::set_mode(std::string mode) {
  m_mode = mode;
  return true;
}
std::string& Program::get_mode() {
  return m_mode;
}

// ----------------------------------------------------------------------------- in settings

bool Program::set_image_in(std::string in) {
  m_image_in = nlohmann::json::parse(in);
  return true;
}
std::string Program::get_image_in() {
  m_image_in["mode"] = m_mode;

  std::string in = m_image_in.dump();
  // std::cout << in << std::endl;
  return in;
}

bool Program::add_image_layer() {
  m_image_in["processes"].push_back(m_image_layer);
  return true;
}
bool Program::rmv_image_layer() {
  std::vector<nlohmann::json> processes = m_image_in["processes"];
  processes.pop_back();
  m_image_in["processes"] = processes;
  return true;
}

bool Program::add_image_filter() {
  m_image_in["processes"].push_back(m_image_filter);
  return true;
}
bool Program::rmv_image_filter() {
  std::vector<nlohmann::json> processes = m_image_in["processes"];
  processes.pop_back();
  m_image_in["processes"] = processes;
  return true;
}

// audio in

bool Program::set_audio_in(std::string in) {
  m_audio_in = nlohmann::json::parse(in);
  std::cout << m_image_in << std::endl;
  return true;
}
std::string Program::get_audio_in() {
  m_audio_in["mode"] = m_mode;
  m_audio_in["time"] = get_out_time();

  std::string in = m_audio_in.dump();
  std::cout << in << std::endl;
  return in;
}

bool Program::add_audio_track() {
  m_audio_in["processes"].push_back(m_audio_track);
  return true;
}
bool Program::rmv_audio_track() {
  std::vector<nlohmann::json> processes = m_audio_in["processes"];
  processes.pop_back();
  m_audio_in["processes"] = processes;
  return true;
}

bool Program::add_audio_filter() {                                          // audioSynth
  m_audio_in["processes"].push_back(m_audio_filter);
  return true;
}
bool Program::rmv_audio_filter() {
  std::vector<nlohmann::json> processes = m_audio_in["processes"];
  processes.pop_back();
  m_audio_in["processes"] = processes;
  return true;
}

// ----------------------------------------------------------------------------- out

bool Program::set_image_out(std::string out) {
  m_image_out = nlohmann::json::parse(out);
  // std::cout << m_image_out << std::endl;
  return true;
}
std::string Program::get_image_out() {
  m_image_out["mode"] = m_mode;
  m_image_out["time"] = get_out_time();
  m_image_out["pages"] = get_out_pages();

  std::string out = m_image_out.dump();
  // std::cout << out << std::endl;
  return out;
}

bool Program::set_audio_out(std::string out) {
  m_audio_out = nlohmann::json::parse(out);
  // std::cout << m_audio_out << std::endl;
  return true;
}
std::string Program::get_audio_out() {
  m_audio_out["mode"] = m_mode;
  m_audio_out["time"] = get_out_time();
  m_audio_out["pages"] = get_out_pages();

  std::string out = m_audio_out.dump();
  // std::cout << out << std::endl;
  return out;
}

unsigned int Program::get_out_pages() {
  unsigned int out_pages;

  if(m_mode == "image") {
    if(m_image_in["files"].size() > 0) {
      out_pages = m_image_in["files"].size();
    } else {
      out_pages = m_image_in["pages"];
    }
  }

  if (m_mode == "audio") {
    out_pages = m_audio_out["image_time"];
  }

  return out_pages;
}

double Program::get_out_time() {
  double out_time;

  if(m_mode == "image") {
    if(m_image_in["files"].size() > 0) {
      out_time = static_cast<double>(m_image_out["time_image"]) * m_image_in["files"].size();
    } else {
      out_time = static_cast<double>(m_image_out["time_image"]) * static_cast<double>(m_image_out["pages"]);
    }
  }

  if (m_mode == "audio") {
    if(m_audio_in["files"].size() > 0) {
      double audiotime = 0;
      for (unsigned int i = 0; i < m_audio_in["files"].size(); i++) {
        stk::FileRead audio(m_audio_in["files"][i]);
        audiotime += audio.fileSize() / audio.fileRate();
      }
      out_time = audiotime;
    } else {
      out_time = m_audio_in["time"];
    }
  }
  return out_time;
}

// ----------------------------------------------------------------------------- computation

void Program::crop(cv::Mat& image, unsigned int width, unsigned int height) {

  if (static_cast<unsigned int>(image.cols) != width || static_cast<unsigned int>(image.rows) != height) {

    cv:: Mat temp;

    double a = width / static_cast<double>(height);
    double b = image.cols / static_cast<double>(image.rows);
    if(a < b) { // portrait
      unsigned int w, x0, x1;

      w = round(image.cols * (static_cast<float>(height) / image.rows));

      cv::Size size(w, height);
      cv::resize(image, temp, size, 0, 0, cv::INTER_CUBIC);

      x0 = (temp.cols - width) / 2;
      x1 = x0 + width;

      cv::Rect roi(x0, 0, x1, temp.rows);
      image = temp(roi);

    } else { // landscape
      unsigned int h, y0, y1;

      h = round(image.rows * (static_cast<float>(width) / image.cols));

      cv::Size size(width, h);
      cv::resize(image, temp, size, 0, 0, cv::INTER_CUBIC);

      y0 = (temp.rows - height) / 2;
      y1 = y0 + height;

      cv::Rect roi(0, y0, temp.cols, y1);
      image = temp(roi);

    }
  }

}

// void Program::image_layer(cv::Mat& image, unsigned char index, unsigned int page) {
//
//   if(m_image_in["processes"][index]["type"] == "fill") {
//     m_imagesynth.fill(image, m_image_in["processes"][index]);
//   }
//   if(m_image_in["processes"][index]["type"] == "noise") {
//     m_imagesynth.noise(image, m_image_in["processes"][index]);
// }
//   if(m_image_in["processes"][index]["type"] == "walker") {
//     m_imagesynth.walker(image, m_image_in["processes"][index]);
//   }
//   if(m_image_in["processes"][index]["type"] == "gradient") {
//     m_imagesynth.gradient(image, m_image_in["processes"][index], page);
//   }
//   if(m_image_in["processes"][index]["type"] == "rectangle") {
//     m_imagesynth.rectangle(image, m_image_in["processes"][index], page);
//   }
//
// } // image_layer openCV

void Program::image_filter(cv::Mat& image, unsigned char index, unsigned int page) {

  if(m_image_in["processes"][index]["type"] == "color_scale") {
    m_imagefilter.color_scale(image, m_image_in["processes"][index]);
  }

  if(m_image_in["processes"][index]["type"] == "color_strech") {
    m_imagefilter.color_strech(image, m_image_in["processes"][index]);
  }

  if(m_image_in["processes"][index]["type"] == "color_jump") {
    m_imagefilter.color_jump(image, m_image_in["processes"][index]);
  }

  if(m_image_in["processes"][index]["type"] == "gamma") {
    m_imagefilter.gamma(image, m_image_in["processes"][index]);
  }

} // image_filter openCV

void Program::audio_filter(stk::StkFrames& audio, unsigned char index) {

  if(m_audio_in["processes"][index]["type"] == "arg") {
    m_audiofilter.arg(audio, m_audio_in["processes"][index]["arg"][0], m_audio_in["processes"][index]["arg"][1], m_audio_in["processes"][index]["arg"][2]);
  }

}

void Program::audio_track(stk::StkFrames& audio, unsigned char index) {

  double (*mergeFcn)(double&, double&, double&) = who::blend::normal;

    if(m_audio_in["processes"][index]["blend"] == "normal") {
      mergeFcn = who::blend::normal;
    }
    if(m_audio_in["processes"][index]["blend"] == "arithmetic") {
      mergeFcn = who::blend::arithmetic;
    }
    if(m_audio_in["processes"][index]["blend"] == "geometric") {
      mergeFcn = who::blend::geometric;
    }
    if(m_audio_in["processes"][index]["blend"] == "harmonic") {
      mergeFcn = who::blend::harmonic;
    }
    if(m_audio_in["processes"][index]["blend"] == "darken") {
      mergeFcn = who::blend::darken;
    }
    if(m_audio_in["processes"][index]["blend"] == "multiply") {
      mergeFcn = who::blend::multiply;
    }
    if(m_audio_in["processes"][index]["blend"] == "colorburn") {
      mergeFcn = who::blend::colorburn;
    }
    if(m_audio_in["processes"][index]["blend"] == "linearburn") {
      mergeFcn = who::blend::linearburn;
    }
    if(m_audio_in["processes"][index]["blend"] == "lighten") {
      mergeFcn = who::blend::lighten;
    }
    if(m_audio_in["processes"][index]["blend"] == "screen") {
      mergeFcn = who::blend::screen;
    }
    if(m_audio_in["processes"][index]["blend"] == "colordodge") {
      mergeFcn = who::blend::colordodge;
    }
    if(m_audio_in["processes"][index]["blend"] == "lineardodge") {
      mergeFcn = who::blend::lineardodge;
    }
    if(m_audio_in["processes"][index]["blend"] == "overlay") {
      mergeFcn = who::blend::overlay;
    }
    if(m_audio_in["processes"][index]["blend"] == "softlight") {
      mergeFcn = who::blend::softlight;
    }
    if(m_audio_in["processes"][index]["blend"] == "hardlight") {
      mergeFcn = who::blend::hardlight;
    }
    if(m_audio_in["processes"][index]["blend"] == "vividlight") {
      mergeFcn = who::blend::vividlight;
    }
    if(m_audio_in["processes"][index]["blend"] == "linearlight") {
      mergeFcn = who::blend::linearlight;
    }
    if(m_audio_in["processes"][index]["blend"] == "pinlight") {
      mergeFcn = who::blend::pinlight;
    }
    if(m_audio_in["processes"][index]["blend"] == "hardmix") {
      mergeFcn = who::blend::hardmix;
    }
    if(m_audio_in["processes"][index]["blend"] == "difference") {
      mergeFcn = who::blend::difference;
    }
    if(m_audio_in["processes"][index]["blend"] == "exclusion") {
      mergeFcn = who::blend::exclusion;
    }

  if(m_audio_in["processes"][index]["type"] == "oscillator") {
    m_audiosynth.oscillator(audio, m_audio_in["processes"][index]["sine"][0], m_audio_in["processes"][index]["sine"][1], m_audio_in["processes"][index]["sine"][2], m_audio_in["processes"][index]["lrfade"], mergeFcn, m_audio_in["processes"][index]["opacity"]);
  }

}

void Program::preview(std::vector< std::vector< std::vector< std::vector<unsigned char> > > >& vector, std::vector<double>& vectorL, std::vector<double>& vectorR, bool audio) {

  unsigned int pages, width, height, frames, pageframes, bands;

  pages = static_cast<unsigned int>(vector.size());
  width = static_cast<unsigned int>(vector[0][0].size());
  height = static_cast<unsigned int>(vector[0].size());
  bands = m_bands / 8;

  frames = static_cast<unsigned int>(vectorL.size());

  pageframes = round(static_cast<double>(frames) / pages);

  m_prelistening.setDataRate(sample_rate);

  if (m_mode == "image") {

    for (unsigned int i = 0; i < pages; i++) { // page loop

      if(m_image_in["files"].size() > 0) {
        std::string file = m_image_in["files"][i];

        cv::Mat img;
        img = cv::imread(m_image_in["files"][i], cv::IMREAD_COLOR);

        crop(img, width, height);

        m_premap = img;
      } else {

        cv::Size size(width, height);
        cv::Mat img(size, CV_8UC3);
        img = 0;
        m_premap = img;
      }

      for (unsigned char p = 0; p < m_image_in["processes"].size(); p++) {
        if (m_image_in["processes"][p]["process"] == "filter") { // filter
          image_filter(m_premap, p, i);
        }
        if (m_image_in["processes"][p]["process"] == "layer") { // layer
          m_imagesynth.synthesis(m_premap, m_image_in["processes"][p], i);
        }
      }

      m_prelistening.resize(pageframes, 2);

      if(audio) {
        if(m_image_out["out_mode"] == "hsl_to_signal") {
          m_imageout.hsl_to_signal(m_premap, m_prelistening, m_image_out, i);
        }  else if(m_image_out["out_mode"] == "rgb_to_chord") {
          m_imageout.rgb_to_chord(m_premap, m_prelistening, m_image_out, i);
        }
      }

      stk::StkFrames framesL(pageframes, 1), framesR(pageframes, 1);

      framesL.setChannel(0, m_prelistening, 0);
      framesR.setChannel(0, m_prelistening, 1);

      // write to buffer

      unsigned int start, stop, f;
      start = i * pageframes;
      stop = (i + 1) * pageframes;
      f = 0;
      for (unsigned int a = start; a < stop; a++) {
        vectorL[a] = framesL[f];
        vectorR[a] = framesR[f];
        f++;
      }

      unsigned int c;
      uchar* ptr;
      for (unsigned int y = 0; y < height; y++) {

        ptr = m_premap.ptr<uchar>(y);
        for (unsigned int x = 0; x < width; x++) {
          c = x * 3;
          vector[i][y][x][0] = ptr[c + 2];
          vector[i][y][x][1] = ptr[c + 1];
          vector[i][y][x][2] = ptr[c];
        }
      }

    } // page loop

  } // mode image

  if (m_mode == "audio") {

    if(m_audio_in["files"].size() > 0) {
      stk::FileRead audio(m_audio_in["files"][0]);
      m_prelistening.resize(audio.fileSize(), audio.channels());
      audio.read(m_prelistening);
    } else {
      m_prelistening.resize(frames, 2);
      stk::StkFrames empL(frames, 1), empR(frames, 1);
      empL.setChannel(0, m_prelistening, 0);
      empR.setChannel(0, m_prelistening, 1);
      for (unsigned int i = 0; i < empL.frames(); i++) {
        empL[i] = 0;
        empR[i] = 0;
      }
      m_prelistening.setChannel(0, empL, 0);
      m_prelistening.setChannel(1, empR, 0);
    }

    std::cout << "A" << std::endl;

    stk::StkFrames preL(frames, 1), preR(frames, 1);

    for (unsigned char p = 0; p < m_audio_in["processes"].size(); p++) {
      if (m_audio_in["processes"][p]["process"] == "filter") { // filter
        audio_filter(m_prelistening, p);
      }
      if (m_audio_in["processes"][p]["process"] == "track") { // track
        audio_track(m_prelistening, p);
      }
    }

    preL.setChannel(0, m_prelistening, 0);
    preR.setChannel(0, m_prelistening, 1);

    stk::StkFrames framesL(pageframes, 1), framesR(pageframes, 1), audio_comp(pageframes, 2);

    cv::Size size(width, height);
    cv::Mat img(size, CV_8UC3);
    img = 0;

    for (unsigned int i = 0; i < pages; i++) { // page loop

      unsigned int start, stop, f;
      start = round(i * static_cast<double>(pageframes));
      stop = round((i + 1) * static_cast<double>(pageframes));
      f = 0;
      for (unsigned int a = start; a < stop; a++) {
        framesL[f] = preL[a];
        framesR[f] = preR[a];
        f++;
      }
      audio_comp.setChannel(0, framesL, 0);
      audio_comp.setChannel(1, framesR, 0);

      m_premap = img;

      if(m_audio_out["out_mode"] == "stereo_spectogram") {
        m_audioout.stereo_spectogramm(audio_comp, m_premap);
      } else if(m_audio_out["out_mode"] == "stereo_shape") {
        m_audioout.stereo_shape(audio_comp, m_premap);
      }

      unsigned int c;
      uchar* ptr;
      for (unsigned int y = 0; y < height; y++) {

        ptr = m_premap.ptr<uchar>(y);
        for (unsigned int x = 0; x < width; x++) {
          c = x * 3;
          vector[i][y][x][0] = ptr[c + 2];
          vector[i][y][x][1] = ptr[c + 1];
          vector[i][y][x][2] = ptr[c];
        }
      }

    } // page loop

    for (unsigned int i = 0; i < frames; i++) {
      vectorL[i] = preL[i];
      vectorR[i] = preR[i];
    }

  } // mode audio

}

bool Program::run() {

  getOutput();

  unsigned int pages, width, height, frames, pageframes, bands;
  double time;

  pages = get_out_pages();
  time = get_out_time();
  // width = m_image.width();
  width = m_map.cols;
  // height = m_image.height();
  height = m_map.rows;
  bands = m_bands;
  frames = round(time * sample_rate);
  pageframes = frames / pages;

  // std::cout << "pages: " << pages << std::endl;
  // std::cout << "time: " << time << std::endl;

  if (m_mode == "image") {

    stk::StkFrames framesL(frames, 1), framesR(frames, 1);

    for (unsigned int i = 0; i < pages; i++) { // page loop

      if(m_image_in["files"].size() > 0) {
        std::string file = m_image_in["files"][i];
        cv::Mat img;

        img = cv::imread(m_image_in["files"][i], cv::IMREAD_COLOR);
        crop(img, width, height);
        m_map = img;
      } else {
        cv::Size size(width, height);
        cv::Mat img(size, CV_8UC3);
        img = 0;
        m_map = img;
      }

      m_audio.setDataRate(sample_rate);
      m_audio.resize(frames, 2);

      for (unsigned char p = 0; p < m_image_in["processes"].size(); p++) {
        if (m_image_in["processes"][p]["process"] == "filter") { // filter
          image_filter(m_map, p, i);
        }
        if (m_image_in["processes"][p]["process"] == "layer") { // layer
          m_imagesynth.synthesis(m_map, m_image_in["processes"][p], i);
        }
      }
      stk::StkFrames audio(pageframes, 2);
      audio.setDataRate(sample_rate);

      if(m_image_out["out_mode"] == "hsl_to_signal") {
        m_imageout.hsl_to_signal(m_map, audio, m_image_out, i);
      }  else if(m_image_out["out_mode"] == "rgb_to_chord") {
        m_imageout.rgb_to_chord(m_map, audio, m_image_out, i);
      }
      stk::StkFrames audioL(pageframes, 1), audioR(pageframes, 1);

      audioL.setChannel(0, audio, 0);
      audioR.setChannel(0, audio, 1);

      // write to audio
      unsigned int start, stop, f;
      start = i * pageframes;
      stop = (i + 1) * pageframes;
      f = 0;
      for (unsigned int a = start; a < stop; a++) {
        framesL[a] = audioL[f];
        framesR[a] = audioR[f];
        f++;
      }

      save_image(i);
    } // page loop

    m_audio.setChannel(0, framesL, 0);
    m_audio.setChannel(1, framesR, 0);

  } // mode image

  if (m_mode == "audio") {

    if(m_audio_in["files"].size() > 0) {
      stk::FileRead audio(m_audio_in["files"][0]);
      m_audio.resize(audio.fileSize(), audio.channels());
      audio.read(m_audio);
    } else {
      m_audio.resize(frames, 2);
      stk::StkFrames empL(frames, 1), empR(frames, 1);
      empL.setChannel(0, m_audio, 0);
      empR.setChannel(0, m_audio, 1);
      for (unsigned int i = 0; i < empL.frames(); i++) {
        empL[i] = 0;
        empR[i] = 0;
      }
      m_audio.setChannel(0, empL, 0);
      m_audio.setChannel(1, empR, 0);
    }

    // std::cout << m_audio.frames() << std::endl;

    for (unsigned char p = 0; p < m_audio_in["processes"].size(); p++) {
      if (m_audio_in["processes"][p]["process"] == "filter") { // filter
        audio_filter(m_audio, p);
      }
      if (m_audio_in["processes"][p]["process"] == "track") { // track
        audio_track(m_audio, p);
      }
    }

    stk::StkFrames preL(frames, 1), preR(frames, 1);

    preL.setChannel(0, m_audio, 0);
    preR.setChannel(0, m_audio, 1);

    stk::StkFrames framesL(pageframes, 1), framesR(pageframes, 1), audio_comp(pageframes, 2);

    // m_image.assign(width, height, 1, 3, 0);

    cv::Size size(width, height);
    cv::Mat img(size, CV_8UC3);
    img = 0;

    for (unsigned int i = 0; i < pages; i++) { // page loop

      unsigned int start, stop, f;
      start = i * pageframes;
      stop = (i + 1) * pageframes;
      f = 0;

      for (unsigned int a = start; a < stop; a++) {
        framesL[f] = preL[a];
        framesR[f] = preR[a];
        f++;
      }

      audio_comp.setChannel(0, framesL, 0);
      audio_comp.setChannel(1, framesR, 0);

      m_map = img;

      if(m_audio_out["out_mode"] == "stereo_spectogram") {
        m_audioout.stereo_spectogramm(audio_comp, m_map);
      }  else if(m_audio_out["out_mode"] == "stereo_shape") {
        m_audioout.stereo_shape(audio_comp, m_map);
      }

      save_image(i);
    } // page loop

  } // mode audio

  save();

  return true;
}

void Program::getOutput() {
  std::string filename, output;

  filename = m_data["filename"];
  output = m_data["output"];

  m_file[0] = wtc::fs::blankFile(filename, output); // file without ".type"
  m_file[1] = wtc::fs::audioFile(filename, output);
  m_file[2] = wtc::fs::textFile(filename, output);

  m_text.open(m_file[2]);
} // getOutput

void Program::save_image(unsigned int page) {
  std::string file = m_file[0] + " - " + std::to_string(page + 1) + ".tiff";

  cv::imwrite( file, m_map );

  // m_image.save_tiff(file.c_str());
} // save_image

void Program::save() {

  m_text.close();

  stk::FileWvOut output;
  try {
      output.openFile( m_file[1], m_audio.channels(), stk::FileWrite::FILE_AIF, stk::Stk::STK_SINT16 );
  } catch (stk::StkError &) {
      exit(1);
  }
  output.tick(m_audio);
} // save
