
#ifndef program_h
#define program_h

#include <string>
#include <chrono>

#include "settings.h"
#include "filter.h"
#include "output.h"

typedef struct {
  std::size_t index;
  cv::Mat image;
  cv::Mat audio;
} frame;

class Program {

  private:

    nlohmann::json m_data;

    std::size_t m_frame_time, m_buffer_size{2}, m_current_frame{0};

    // just for the data
    Settings m_settings;
    Filter   m_filter;
    Output   m_output;

    std::vector<frame> m_buffer;

    void create_frame(std::size_t frame_index);

    void clean_buffer();

    void update_buffer();


    std::size_t last_buffer_index();

    frame get_frame(std::size_t f);

  public:
    Program();

    // •   work()

    // <-  data()
    // <-> update(data) / including play ???

    // <-  read(frame)  / returns image and audio data

    std::string work();

    nlohmann::json data();
    nlohmann::json update(nlohmann::json data);

    // reference image and audio for pick up?

    frame read(std::size_t f);



    nlohmann::json init();

    void preview(std::vector<cv::Mat>& images, stk::StkFrames& audio);

    void save();

};

#endif /* program_h */
