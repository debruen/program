
#ifndef data_h
#define data_h

#include "nlohmann/json.hpp"

namespace data = nlohmann;

typedef unsigned char (*blendchar)(unsigned char&, unsigned char&, double&);
typedef double (*blendfloat)(double&, double&, double&);

namespace data {

  std::size_t size(json data);

  double value_dbl(json data, std::string label);

  int value_int(json data, std::string label);

  std::string value_str(json data, std::string label);

  std::vector<unsigned char> value_rgb(json data, std::string label);

  stk::StkFrames value_sgl(json data, std::string label, std::size_t frames);

  blendchar int_blend(json data, std::string label);

  blendfloat dbl_blend(json data, std::string label);

} // eval

class data_color {

  private:

    nlohmann::json m_data;

  public:
    data_color();

    nlohmann::json data();

    nlohmann::json data(nlohmann::json data, std::string type);

    void process(std::vector<cv::Mat>& images, stk::StkFrames& audio, std::string type);

};

#endif /* data_h */
