
#ifndef filter_h
#define filter_h

#include <vector>
#include <random>

#include "nlohmann/json.hpp"

#include "opencv2/core.hpp"
#include "opencv2/imgproc.hpp"

#include "stk/SineWave.h"

#include "../functions.h"

// #include "blend.h"

class Filter {

  private:

    nlohmann::json m_data;

    std::string m_type;

    nlohmann::json base_world(std::string type);

  protected:

    Filter();

    nlohmann::json init(std::string type);

    std::string get_type(nlohmann::json data);

    nlohmann::json set_data(nlohmann::json data, std::string type);

    double project(double min, double max, double v);
    unsigned char projectChar(unsigned char min, unsigned char max, double v);
    double circle(double min, double max, double value);
    double radian(double degree);
    double normDouble(double min, double max, double value); // returns 0.0 - 1.0 from value out of range min max

    double point_gradient(unsigned int y, unsigned int x, unsigned int w, unsigned int h, double frequency = 1.0, double phase = 0.0, double tilt = 0.0);

    // blendchar m_blend;

  public:

    virtual nlohmann::json data(std::string type) = 0;

    virtual nlohmann::json data(nlohmann::json data, std::string type) = 0;

    virtual void process(std::vector<cv::Mat>& images) = 0;

    virtual void process(stk::StkFrames& audio) = 0;

};

#endif // filter_h END
