
#ifndef program_h
#define program_h

#include <fstream>
#include <sys/stat.h>

#include "stk/FileWvOut.h"

#include "functions.h"

#include "input.h"
#include "filtration.h"
#include "output.h"

enum TYPE {
    AUDIO,
    IMAGE,
    TEXT,
    NONE
};

class Program {

  private:

    /// computation modules
    Input      m_input;
    Filtration m_filtration;
    Output     m_output;

    /// data object
    nlohmann::json m_data;

    /// input type (image or audio)
    std::string m_type;

    // the standard area in pixel for saved image files
    unsigned int   m_area;

    std::string home();
    bool fileExists(const std::string& file);
    std::string getFreeFile(std::string& filename, std::string& path, TYPE t);
    std::string audioFile(std::string filename, std::string path);
    std::string blankFile(std::string filename, std::string path);

    void save_image(cv::Mat& image, std::string file, unsigned int frame);
    void save(stk::StkFrames& audio, std::string file);

  public:
    Program(); // constructor

    nlohmann::json data();

    nlohmann::json data(nlohmann::json data);

    void process(std::vector<cv::Mat>& images, stk::StkFrames& audio);

    void process();

};

#endif /* program_h */
