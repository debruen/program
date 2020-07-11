#ifndef ia_h
#define ia_h

#include <fstream>
#include <cmath>
#include <string>
#include <vector>

#include "nlohmann/json.hpp"

#include "opencv2/core.hpp"
#include "opencv2/highgui.hpp"
#include "opencv2/imgproc.hpp"

#include "stk/SineWave.h"
#include "stk/FileRead.h"
#include "stk/FileWvOut.h"

#include "wtc.h"
#include "who.h"

#include "image_filter.h"
#include "image_synth.h"
#include "image_out.h"

#include "audio_filter.h"
#include "audio_synth.h"
#include "audio_out.h"

class Program {

  private:

    SIZE m_size; // dimensions
    unsigned int m_width;
    unsigned int m_height;
    unsigned int m_bands;

    void init(SIZE size = C);

    ImageFilter m_imagefilter;
    ImageSynth m_imagesynth;
    ImageOut m_imageout;

    AudioFilter m_audiofilter;
    AudioSynth m_audiosynth;
    AudioOut m_audioout;

    double sample_rate = 44100.0;
    double prelis_rate = 22050.0;

    // ------------------------------------------------------------------------- data

    nlohmann::json m_data;

    // ------------------------------------------------------------------------- main

    std::string m_mode = "image";

    // ------------------------------------------------------------------------- in

    nlohmann::json m_image_in;

    nlohmann::json m_image_layer;
    nlohmann::json m_image_filter;

    nlohmann::json m_audio_in;

    nlohmann::json m_audio_track;
    nlohmann::json m_audio_filter;

    // ------------------------------------------------------------------------- out

    nlohmann::json m_image_out;
    nlohmann::json m_audio_out;

    // ------------------------------------------------------------------------- computation

    stk::StkFrames m_prelistening;

    cv::Mat m_premap;

    cv::Mat m_map;

    stk::StkFrames m_audio;
    std::ofstream m_text;

    void crop(cv::Mat& image, unsigned int width, unsigned int height);

    void image_layer(cv::Mat& image, unsigned char index, unsigned int page);
    void image_filter(cv::Mat& image, unsigned char index, unsigned int page);

    void audio_filter(stk::StkFrames& audio, unsigned char index);
    void audio_track(stk::StkFrames& audio, unsigned char index);

    std::string m_file[3];
    void getOutput();

    void save_image(unsigned int page);
    void save();

  public:
    Program(std::string data); // constructor

    bool set_data(std::string data);
    std::string get_data();

    // ------------------------------------------------------------------------- main settings

    bool set_mode(std::string mode);
    std::string& get_mode();

    // ------------------------------------------------------------------------- in settings

    bool set_image_in(std::string in);
    std::string get_image_in();

    bool add_image_layer();
    bool rmv_image_layer();

    bool add_image_filter();
    bool rmv_image_filter();

    bool set_audio_in(std::string in);
    std::string get_audio_in();

    bool add_audio_track();
    bool rmv_audio_track();

    bool add_audio_filter();
    bool rmv_audio_filter();

    // ------------------------------------------------------------------------- out settings

    bool set_image_out(std::string out);
    std::string get_image_out();

    bool set_audio_out(std::string out);
    std::string get_audio_out();

    unsigned int get_out_pages();
    double get_out_time();

    // ------------------------------------------------------------------------- computation

    void preview(std::vector< std::vector< std::vector< std::vector<unsigned char> > > >& vector, std::vector<double>& vectorL, std::vector<double>& vectorR, bool audio = false);
    bool run();
};

#endif /* ia_h */
