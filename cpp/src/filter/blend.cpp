
#include "blend.h"

Blend::Blend() {

  nlohmann::json data_blend, data_array;

  m_data = nlohmann::json::array();

  std::vector<std::string> blend_options{"normal", "arithmetic", "geometric", "harmonic", "darken", "multiply", "colorburn", "linearburn", "lighten", "screen", "colordodge", "lineardodge", "overlay", "softlight", "hardlight", "vividlight", "linearlight", "pinlight", "hardmix", "difference", "exclusion"};
  data_blend   = data::data_string("blend", true, blend_options, blend_options[0]);
  data_array   = nlohmann::json::array();

  m_masks.push_back(new Gradient);

  data_array.push_back(m_masks[0]->init());

  m_blendc   = normal;
  m_blendf   = normal;
  m_array    = data_array;

  m_data.push_back(data_blend);
  m_data.push_back(data_array);

}

blendchar Blend::get_blendc(nlohmann::json data, std::string label) {

  std::string blend = data::get_string(data, label);

  blendchar function = normal;

  if(blend == "normal") function = normal;
  if(blend == "arithmetic") function = arithmetic;
  if(blend == "geometric") function = geometric;
  if(blend == "harmonic") function = harmonic;
  if(blend == "darken") function = darken;
  if(blend == "multiply") function = multiply;
  if(blend == "colorburn") function = colorburn;
  if(blend == "linearburn") function = linearburn;
  if(blend == "lighten") function = lighten;
  if(blend == "screen") function = screen;
  if(blend == "colordodge") function = colordodge;
  if(blend == "lineardodge") function = lineardodge;
  if(blend == "overlay") function = overlay;
  if(blend == "softlight") function = softlight;
  if(blend == "hardlight") function = hardlight;
  if(blend == "vividlight") function = vividlight;
  if(blend == "linearlight") function = linearlight;
  if(blend == "pinlight") function = pinlight;
  if(blend == "hardmix") function = hardmix;
  if(blend == "difference") function = difference;
  if(blend == "exclusion") function = exclusion;

  return function;
}

blendfloat Blend::get_blendf(nlohmann::json data, std::string label) {

  std::string blend = data::get_string(data, label);

  blendfloat function = normal;

  if(blend == "normal") function = normal;
  if(blend == "arithmetic") function = arithmetic;
  if(blend == "geometric") function = geometric;
  if(blend == "harmonic") function = harmonic;
  if(blend == "darken") function = darken;
  if(blend == "multiply") function = multiply;
  if(blend == "colorburn") function = colorburn;
  if(blend == "linearburn") function = linearburn;
  if(blend == "lighten") function = lighten;
  if(blend == "screen") function = screen;
  if(blend == "colordodge") function = colordodge;
  if(blend == "lineardodge") function = lineardodge;
  if(blend == "overlay") function = overlay;
  if(blend == "softlight") function = softlight;
  if(blend == "hardlight") function = hardlight;
  if(blend == "vividlight") function = vividlight;
  if(blend == "linearlight") function = linearlight;
  if(blend == "pinlight") function = pinlight;
  if(blend == "hardmix") function = hardmix;
  if(blend == "difference") function = difference;
  if(blend == "exclusion") function = exclusion;

  return function;
}

double Blend::normal(double& a, double& b, double& o) {
    return  a * (1 - o) + b * o;
}

unsigned char Blend::normal(unsigned char& aChar, unsigned char& bChar, double& o) {
    double a = static_cast<double>(aChar) / 255, b = static_cast<double>(bChar) / 255, c;
    c = normal(a, b, o);
    return round(c * 255);
}

// mean

double Blend::arithmetic(double& a, double& b, double& o) {

    double c = (a + b) / 2;

    return  a * (1 - o) + c * o;
}

unsigned char Blend::arithmetic(unsigned char& aChar, unsigned char& bChar, double& o) {
    double a = static_cast<double>(aChar) / 255, b = static_cast<double>(bChar) / 255, c;
    c = arithmetic(a, b, o);
    return round(c * 255);
}

double Blend::geometric(double& a, double& b, double& o) {

    bool test = false;

    double temp = a * b;

    if(temp < 0) test = true;

    double c = sqrt(fabs(temp));

    if (test) c *= -1;

    return a * (1 - o) + c * o;
}

unsigned char Blend::geometric(unsigned char& aChar, unsigned char& bChar, double& o) {
    double a = static_cast<double>(aChar) / 255, b = static_cast<double>(bChar) / 255, c;
    c = geometric(a, b, o);
    return round(c * 255);
}

double Blend::harmonic(double& a, double& b, double& o) {

    double c = 2 / ((1 / a) + (1 / b));

    if(c > 1) c = 1;
    if(c < -1) c = -1;
    return a * (1 - o) + c * o;
}

unsigned char Blend::harmonic(unsigned char& aChar, unsigned char& bChar, double& o) {
    double a = static_cast<double>(aChar) / 255, b = static_cast<double>(bChar) / 255, c;
    c = harmonic(a, b, o);
    return round(c * 255);
}

// blend

double Blend::darken(double& a, double& b, double& o) {
    double c;

    if(a < b) c = a;
    if(a > b) c = b;

    return a * (1 - o) + c * o;
}

unsigned char Blend::darken(unsigned char& aChar, unsigned char& bChar, double& o) {
    double a = static_cast<double>(aChar) / 255, b = static_cast<double>(bChar) / 255, c;
    c = darken(a, b, o);
    return round(c * 255);
}

double Blend::multiply(double& a, double& b, double& o) {
    double c = a * b;
    return a * (1 - o) + c * o;
}
unsigned char Blend::multiply(unsigned char& aChar, unsigned char& bChar, double& o) {
    double a = static_cast<double>(aChar) / 255, b = static_cast<double>(bChar) / 255, c;
    c = multiply(a, b, o);
    return round(c * 255);
}

double Blend::colorburn(double& a, double& b, double& o) {
    double c = 1 - (1 - b) / a;

    if(c < 0) c = 0;
    return a * (1 - o) + c * o;
}

unsigned char Blend::colorburn(unsigned char& aChar, unsigned char& bChar, double& o) {
    double a = static_cast<double>(aChar) / 255, b = static_cast<double>(bChar) / 255, c;
    c = colorburn(a, b, o);
    return round(c * 255);
}

double Blend::linearburn(double& a, double& b, double& o) {
    double c = a + b - 1;

    if(c < 0) c = 0;
    return a * (1 - o) + c * o;
}

unsigned char Blend::linearburn(unsigned char& aChar, unsigned char& bChar, double& o) {
    double a = static_cast<double>(aChar) / 255, b = static_cast<double>(bChar) / 255, c;
    c = linearburn(a, b, o);
    return round(c * 255);
}

double Blend::lighten(double& a, double& b, double& o) {
    double c;

    if(b < a) c = a;
    if(b > a) c = b;

    return a * (1 - o) + c * o;
}

unsigned char Blend::lighten(unsigned char& aChar, unsigned char& bChar, double& o) {
    double a = static_cast<double>(aChar) / 255, b = static_cast<double>(bChar) / 255, c;
    c = lighten(a, b, o);
    return round(c * 255);
}

double Blend::screen(double& a, double& b, double& o) {

    double c = 1 - (1 - a) * (1 - b);

    return a * (1 - o) + c * o;
}

unsigned char Blend::screen(unsigned char& aChar, unsigned char& bChar, double& o) {
    double a = static_cast<double>(aChar) / 255, b = static_cast<double>(bChar) / 255, c;
    c = screen(a, b, o);
    return round(c * 255);
}

double Blend::colordodge(double& a, double& b, double& o) {

    double c = a / (1 - b);

    if(c > 1) c = 1;
    return a * (1 - o) + c * o;
}

unsigned char Blend::colordodge(unsigned char& aChar, unsigned char& bChar, double& o) {
    double a = static_cast<double>(aChar) / 255, b = static_cast<double>(bChar) / 255, c;
    c = colordodge(a, b, o);
    return round(c * 255);
}

double Blend::lineardodge(double& a, double& b, double& o) {

    double c = a + b;

    if(c > 1) c = 1;
    return a * (1 - o) + c * o;
}

unsigned char Blend::lineardodge(unsigned char& aChar, unsigned char& bChar, double& o) {
    double a = static_cast<double>(aChar) / 255, b = static_cast<double>(bChar) / 255, c;
    c = lineardodge(a, b, o);
    return round(c * 255);
}

double Blend::overlay(double& a, double& b, double& o) {
    double c;

    if(a < 0.5) {
        c = 2 * a * b;
    } else {
        c = 1 - 2 * (1 - a) * (1 - b);
    }

    return a * (1 - o) + c * o;
}

unsigned char Blend::overlay(unsigned char& aChar, unsigned char& bChar, double& o) {
    double a = static_cast<double>(aChar) / 255, b = static_cast<double>(bChar) / 255, c;
    c = overlay(a, b, o);
    return round(c * 255);
}

double Blend::softlight(double& a, double& b, double& o) {
    double c;

    if(b < 0.5) {
        c = 2 * a * b + (pow(a, 2) * (1 - 2 * b));
    } else {
        c = 2 * a * (1 - b) + (sqrt(a) * (2 * b - 1));
    }

    return a * (1 - o) + c * o;
}

unsigned char Blend::softlight(unsigned char& aChar, unsigned char& bChar, double& o) {
    double a = static_cast<double>(aChar) / 255, b = static_cast<double>(bChar) / 255, c;
    c = softlight(a, b, o);
    return round(c * 255);
}

double Blend::hardlight(double& a, double& b, double& o) {
    double c;

    if(b < 0.5) {
        c = 2 * b * a;
    } else {
        c = 1 - 2 * (1 - b) * (1 - a);
    }

    return a * (1 - o) + c * o;
}

unsigned char Blend::hardlight(unsigned char& aChar, unsigned char& bChar, double& o) {
    double a = static_cast<double>(aChar) / 255, b = static_cast<double>(bChar) / 255, c;
    c = hardlight(a, b, o);
    return round(c * 255);
}

double Blend::vividlight(double& a, double& b, double& o) {
    double c;

    if(a <= 0.5) c = 1 - (1 - b) / (2 * a);
    if(a > 0.5) c = b / (2 * (1 - a));

    if(c < 0) c = 0;
    if(c > 1) c = 1;
    return a * (1 - o) + c * o;
}

unsigned char Blend::vividlight(unsigned char& aChar, unsigned char& bChar, double& o) {
    double a = static_cast<double>(aChar) / 255, b = static_cast<double>(bChar) / 255, c;
    c = vividlight(a, b, o);
    return round(c * 255);
}

double Blend::linearlight(double& a, double& b, double& o) {

    double c = b + 2 * a - 1;

    if(c < 0) c = 0;
    if(c > 1) c = 1;
    return a * (1 - o) + c * o;
}

unsigned char Blend::linearlight(unsigned char& aChar, unsigned char& bChar, double& o) {
    double a = static_cast<double>(aChar) / 255, b = static_cast<double>(bChar) / 255, c;
    c = linearlight(a, b, o);
    return round(c * 255);
}

double Blend::pinlight(double& a, double& b, double& o) {
    double c;

    if(b > 0.5) {
      if(a > 2 * (b - 0.5)) c = a;
      if(a < 2 * (b - 0.5)) c = 2 * (b - 0.5);
    } else {
      if(a < 2 * b) c = a;
      if(a > 2 * b) c = 2 * b;
    }

    return a * (1 - o) + c * o;
}

unsigned char Blend::pinlight(unsigned char& aChar, unsigned char& bChar, double& o) {
    double a = static_cast<double>(aChar) / 255, b = static_cast<double>(bChar) / 255, c;
    c = pinlight(a, b, o);
    return round(c * 255);
}

double Blend::hardmix(double& a, double& b, double& o) {
    double c;

    if(a < 1 - b) c = 0;
    if(a > 1 - b) c = 1;

    return a * (1 - o) + c * o;
}

unsigned char Blend::hardmix(unsigned char& aChar, unsigned char& bChar, double& o) {
    double a = static_cast<double>(aChar) / 255, b = static_cast<double>(bChar) / 255, c;
    c = hardmix(a, b, o);
    return round(c * 255);
}

double Blend::difference(double& a, double& b, double& o) {

    double c = fabs(a - b);

    return a * (1 - o) + c * o;
}

unsigned char Blend::difference(unsigned char& aChar, unsigned char& bChar, double& o) {
    double a = static_cast<double>(aChar) / 255, b = static_cast<double>(bChar) / 255, c;
    c = difference(a, b, o);
    return round(c * 255);
}

double Blend::exclusion(double& a, double& b, double& o) {

    double c = 0.5 - 2 * (a - 0.5) * (b - 0.5);

    return a * (1 - o) + c * o;
}

unsigned char Blend::exclusion(unsigned char& aChar, unsigned char& bChar, double& o) {
    double a = static_cast<double>(aChar) / 255, b = static_cast<double>(bChar) / 255, c;
    c = exclusion(a, b, o);
    return round(c * 255);
}

void Blend::frame(cv::Mat& image, cv::Mat& film, std::size_t frame_index) {
  std::size_t width = image.cols, height = image.rows;
  cv::Size size(width, height);

  cv::Mat mask = cv::Mat(size, CV_64FC1);

  std::size_t c;
  uchar* image_ptr, * film_ptr;
  double* mask_ptr;


  for (std::size_t i = m_masks.size(); i --> 0; ) {
    if (i == m_masks.size()) {
      mask = m_masks[i]->frame(mask, frame_index);
    } else {
      m_masks[i]->process(mask, frame_index);
    }
  }

  for (std::size_t  y = 0; y < height; y++) {

    image_ptr = image.ptr<uchar>(y);
    film_ptr = film.ptr<uchar>(y);
    mask_ptr = mask.ptr<double>(y);

    for (std::size_t  x = 0; x < width; x++) {
      c = x * 3;

      image_ptr[c + 2] = m_blendc(image_ptr[c + 2], film_ptr[c + 2], mask_ptr[x]);
      image_ptr[c + 1] = m_blendc(image_ptr[c + 1], film_ptr[c + 1], mask_ptr[x]);
      image_ptr[c]     = m_blendc(image_ptr[c],     film_ptr[c],     mask_ptr[x]);
    }
  }

}

// --- --- --- --- --- --- --- --- Public --- --- ---

nlohmann::json Blend::data() {

  return m_data;
}

nlohmann::json Blend::update(nlohmann::json data) {

  m_blendc  = get_blendc(data, "blend");
  m_blendf  = get_blendf(data, "blend");

  nlohmann::json new_array = nlohmann::json::array(), array = data[1];
  std::vector< Mask* > new_masks;

  std::string new_type, old_type, new_filter, none;
  std::size_t size = data::array_size(array);

  new_filter = data::get_string(array[size - 1], "filter");

  for (std::size_t i = 0; i < size; i++) {
    new_type = data::get_string(array[i], "type");
    old_type = data::get_string(m_array[i], "type");

    if (new_type == old_type) {
      new_masks.push_back(m_masks[i]);
      new_array.push_back(new_masks[i]->update(array[i]));

    } else {
      if (new_type == "sine") new_masks.push_back(new Gradient());
      if (new_type == "even") new_masks.push_back(new Gradient());
      new_array.push_back(new_masks[i]->init());
    }

    none = data::get_string(array[i], "filter");
    if (none == "none") {
      break;
    }

  }

  if (new_filter != "none") {
    new_masks.push_back(new Gradient());
    new_array.push_back(new_masks[size]->init());
  }

  m_array = new_array;
  m_masks = new_masks;

  m_data[0] = data[0];
  m_data[1] = m_array;

  return m_data;
}

void Blend::image_frame(cv::Mat& image, cv::Mat& film, std::size_t frame_index) {

  frame(image, film, frame_index);

}

void Blend::audio_frame(cv::Mat& audio, cv::Mat& film, std::size_t frame_index) {

  frame(audio, film, frame_index);

}



void Blend::process(std::vector<cv::Mat>& images, std::vector<cv::Mat>& fillings) {

  std::size_t width = images[0].cols, height = images[0].rows;
  cv::Size size(width, height);

  cv::Mat mask = cv::Mat(size, CV_64FC1);

  std::size_t c;
  uchar* image_ptr, * fill_ptr;
  double* mask_ptr;

  for (std::size_t f = 0; f < images.size(); f++) {

    for (std::size_t i = m_masks.size(); i --> 0; ) {
      if (i == m_masks.size()) {
        mask = m_masks[i]->frame(mask, f);
      } else {
        m_masks[i]->process(mask, f);
      }
    }

    for (std::size_t  y = 0; y < height; y++) {

      image_ptr = images[f].ptr<uchar>(y);
      fill_ptr = fillings[f].ptr<uchar>(y);
      mask_ptr = mask.ptr<double>(y);

      for (std::size_t  x = 0; x < width; x++) {
        c = x * 3;

        image_ptr[c + 2] = m_blendc(image_ptr[c + 2], fill_ptr[c + 2], mask_ptr[x]);
        image_ptr[c + 1] = m_blendc(image_ptr[c + 1], fill_ptr[c + 1], mask_ptr[x]);
        image_ptr[c]     = m_blendc(image_ptr[c],     fill_ptr[c],     mask_ptr[x]);
      }
    }

  }

}

void Blend::process(stk::StkFrames& audio, stk::StkFrames& filling) {

  std::size_t width = 2, length = audio.frames();

  cv::Size size(width, length);

  cv::Mat mask = cv::Mat(size, CV_64FC1);

  double* mask_ptr;

  stk::StkFrames framesL(length, 1), framesR(length, 1), fillingL(length, 1), fillingR(length, 1);
  framesL.setChannel(0, audio, 0);
  framesR.setChannel(0, audio, 1);
  fillingL.setChannel(0, filling, 0);
  fillingR.setChannel(0, filling, 1);


  for (std::size_t i = m_masks.size(); i --> 0; ) {
    if (i == m_masks.size()) {
      mask = m_masks[i]->frame(mask, 0);
    } else {
      m_masks[i]->process(mask, 0);
    }
  }

  for (std::size_t  y = 0; y < length; y++) {

    mask_ptr = mask.ptr<double>(y);

    for (std::size_t  x = 0; x < width; x++) {

      if (x == 0)
        framesL[y] = m_blendf(framesL[y], fillingL[y], mask_ptr[x]);
      else
        framesR[y] = m_blendf(framesR[y], fillingR[y], mask_ptr[x]);

    }
  }

  audio.setChannel(0, framesL, 0);
  audio.setChannel(1, framesR, 0);

}
