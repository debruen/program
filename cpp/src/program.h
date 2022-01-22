
#ifndef program_h
#define program_h

#include <string>

#include "settings.h"
#include "filter.h"
#include "output.h"

class Program {

  private:

    nlohmann::json m_data;

    Settings  m_settings;
    Filter m_filter;
    Output m_output;

  public:
    Program();

    std::string work();

    nlohmann::json init();

    nlohmann::json update(nlohmann::json data);

    nlohmann::json get();

    void preview(std::vector<cv::Mat>& images, stk::StkFrames& audio);

    void save();

};

#endif /* program_h */
