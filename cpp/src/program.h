
#ifndef program_h
#define program_h

#include <string>

#include "settings.h"
#include "filter.h"
#include "output.h"

typedef struct {
  std::size_t frame;
  cv::Mat image;
  stk::StkFrames audio;
} frame;

class Program {

  private:

    nlohmann::json m_data;

    // just for the data
    Settings m_settings;
    Filter m_filter;
    Output m_output;

    std::size_t m_active_frame{0};

    std::vector<frame> m_frames;

    frame create_frame(nlohmann::json data, std::size_t f);

    frame get_frame(std::size_t f);

  public:
    Program();

    std::string work();

    frame read(std::size_t f);

    nlohmann::json get();

    nlohmann::json init();

    nlohmann::json update(nlohmann::json data);

    void preview(std::vector<cv::Mat>& images, stk::StkFrames& audio);

    void save();

};

#endif /* program_h */
