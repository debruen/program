#include "image_synth.h"

ImageSynth::ImageSynth() {

}

blendchar ImageSynth::get_blend(std::string blend) {

  blendchar function = who::blend::normal;

  if(blend == "normal") function = who::blend::normal;
  if(blend == "arithmetic") function = who::blend::arithmetic;
  if(blend == "geometric") function = who::blend::geometric;
  if(blend == "harmonic") function = who::blend::harmonic;
  if(blend == "darken") function = who::blend::darken;
  if(blend == "multiply") function = who::blend::multiply;
  if(blend == "colom_color_durn") function = who::blend::colorburn;
  if(blend == "lineam_color_durn") function = who::blend::linearburn;
  if(blend == "lighten") function = who::blend::lighten;
  if(blend == "screen") function = who::blend::screen;
  if(blend == "colordodge") function = who::blend::colordodge;
  if(blend == "lineardodge") function = who::blend::lineardodge;
  if(blend == "overlay") function = who::blend::overlay;
  if(blend == "softlight") function = who::blend::softlight;
  if(blend == "hardlight") function = who::blend::hardlight;
  if(blend == "vividlight") function = who::blend::vividlight;
  if(blend == "linearlight") function = who::blend::linearlight;
  if(blend == "pinlight") function = who::blend::pinlight;
  if(blend == "hardmix") function = who::blend::hardmix;
  if(blend == "difference") function = who::blend::difference;
  if(blend == "exclusion") function = who::blend::exclusion;

  return function;
} // get_blend

void ImageSynth::set_data(nlohmann::json data) {

  m_opacity = data["opacity"];

  m_blend = get_blend(data["blend"]);

  m_color_a = data["color"][0];
  m_color_b = data["color"][1];
  m_color_c = data["color"][2];
  m_color_d = data["color"][3];
  m_color_e = data["color"][4];
  m_color_f = data["color"][5];

  m_frequency = data["gradient"][0];
  m_phase = data["gradient"][1];
  m_tilt = data["gradient"][2];

  m_width = data["rectangle"][0];
  m_height = data["rectangle"][1];
  m_center = data["rectangle"][2];

}

double ImageSynth::point_gradient(unsigned int y, unsigned int x, unsigned int width, unsigned int height, double frequency, double phase, double tilt) {

    double r, p, a, w, h, rp, ra, ya, xa, ry, yv, xv, degrees, sinus;

    w = static_cast<double>(width) - 1;
    h = static_cast<double>(height) - 1;

    r = frequency * 360;
    p  = phase * 360;
    a = 1 - tilt;

    if(a <= 0.25) {
        ya = 1 - a * 4;
        xa = a * 4;
        rp = 0;
    } else if (a <= 0.5) {
        ra = a - 0.25;
        ya = ra * 4;
        xa = 1 - ra * 4;
        rp = 0;
    } else if (a <= 0.75) {
        ra = a - 0.5;
        ya = 1 - ra * 4;
        xa = ra * 4;
        rp = 180;
    } else {
        ra = a - 0.75;
        ya = ra * 4;
        xa = 1 - ra * 4;
        rp = 180;
    }

    if(a <= 0.25) {
        ry = y;          // √
    } else if (a <= 0.5) {
        ry = h - y;
    } else if (a <= 0.75) {
        ry = y;
    } else {
        ry = h - y;
    }

    yv = (ry / h * ya);
    xv = (x / w * xa);

    degrees = ((xv + yv) * r) + p + rp;
    sinus = sin(wtc::number::radian(degrees));

    return wtc::number::normDouble(-1, 1, sinus);
} // point_gradient

void ImageSynth::fill(cv::Mat& image, nlohmann::json data) {
  set_data(data);

  uchar r, g, b, value;

  cv::Mat table(1, 256, CV_8UC3), temp;

  for (int i = 0; i < 256; ++i) {
    value = static_cast<uchar>(i);

    r = m_blend(value, m_color_a, m_opacity);
    g = m_blend(value, m_color_b, m_opacity);
    b = m_blend(value, m_color_c, m_opacity);

    table.at<cv::Vec3b>(0,i) = cv::Vec3b(b, g, r);
  }

  cv::LUT(image, table, temp);

  image = temp;
} // fill openCV

void ImageSynth::noise(cv::Mat& image, nlohmann::json data) {
  set_data(data);

  unsigned int width, height;
  uchar r, g, b, value;

  std::mt19937 generator(8);
  std::uniform_int_distribution<unsigned char> distribution(0,255);

  width = static_cast<unsigned int>(image.cols);
  height = static_cast<unsigned int>(image.rows);

  unsigned int c;
  uchar* ptr;
  for (unsigned int y = 0; y < height; ++y) {

    ptr = image.ptr<uchar>(y);
    for (unsigned int x = 0; x < width; ++x) {
      c = x * 3;

      value = distribution(generator);

      r = wtc::scale::projectChar(m_color_a, m_color_d, value);
      g = wtc::scale::projectChar(m_color_b, m_color_e, value);
      b = wtc::scale::projectChar(m_color_c, m_color_f, value);

      ptr[c + 2] = m_blend(ptr[c + 2], r, m_opacity);
      ptr[c + 1] = m_blend(ptr[c + 1], g, m_opacity);
      ptr[c] = m_blend(ptr[c], b, m_opacity);
    }
  }

} // noise

void ImageSynth::walker(cv::Mat& image, nlohmann::json data) {
  set_data(data);

  double value;
  unsigned int n, y, x, temp, width, height;
  uchar r, g, b;

    width = static_cast<unsigned int>(image.cols);
    height = static_cast<unsigned int>(image.rows);

    std::mt19937 generator(8);
    std::uniform_int_distribution<int> distribution(-1, 1);

    n = (height * width) / 4;

    y = (height - 1) / 2;
    x = (width - 1) / 2;

    std::vector< std::vector<uchar> > vector(height, std::vector<uchar> (width, 0));

    for (unsigned int i = 0; i < n; ++i) {
        if (i > 0){
            y = y + distribution(generator);
            x = x + distribution(generator);
        }

        if(y > height - 1) y = height - 1;
        if(y < 0) y = 0;

        if(x > width - 1) x = width - 1;
        if(x < 0) x = 0;

        temp = vector[y][x] + 2;
        if(temp > 255) temp = 255;
        vector[y][x] = temp;
    }

    unsigned int c;
    uchar* ptr;
    for (unsigned int y = 0; y < height; ++y) {

      ptr = image.ptr<uchar>(y);
      for (unsigned int x = 0; x < width; ++x) {
        c = x * 3;

        value = sqrt(static_cast<double>(vector[y][x]) / 255);
        r = wtc::scale::projectChar(m_color_a, m_color_d, value);
        g = wtc::scale::projectChar(m_color_b, m_color_e, value);
        b = wtc::scale::projectChar(m_color_c, m_color_f, value);

        ptr[c + 2] = m_blend(ptr[c + 2], r, m_opacity);
        ptr[c + 1] = m_blend(ptr[c + 1], g, m_opacity);
        ptr[c] = m_blend(ptr[c], b, m_opacity);
      }
    }

} // walker

void ImageSynth::gradient(cv::Mat& image, nlohmann::json data, int page) {
  set_data(data);

  double value, pm;
  unsigned int width, height;
  uchar r, g, b;

  m_tilt = wtc::number::circle(0, 1, m_tilt);

    for (int i = 0; i < page; i++) {
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

        value = point_gradient(y, x, width, height, m_frequency, m_phase, m_tilt);
        r = wtc::scale::projectChar(m_color_a, m_color_d, value);
        g = wtc::scale::projectChar(m_color_b, m_color_e, value);
        b = wtc::scale::projectChar(m_color_c, m_color_f, value);

        ptr[c + 2] = m_blend(ptr[c + 2], r, m_opacity);
        ptr[c + 1] = m_blend(ptr[c + 1], g, m_opacity);
        ptr[c] = m_blend(ptr[c], b, m_opacity);
      }
    }

} // gm_color_adient

void ImageSynth::rectangle(cv::Mat& image, nlohmann::json data, int page) {
  set_data(data);

  double value;
  unsigned int w, wr, left, right, width, height;
  uchar r, g, b;

  for (int i = 0; i < page; i++) {
    m_phase += m_frequency;
  }

  width = static_cast<unsigned int>(image.cols);
  height = static_cast<unsigned int>(image.rows);

  w = static_cast<double>(width) - 1;
  wr = w * m_width;
  left = round((w - wr) * m_center);
  right = round(wr + left);

  unsigned int c;
  uchar* ptr;
  for (unsigned int y = 0; y < height; y++) {

    ptr = image.ptr<uchar>(y);
    for (unsigned int x = 0; x < width; x++) {
      c = x * 3;

      value = point_gradient(y, x, width, height, m_frequency, m_phase, 0);
      if(value < m_height && x >= left && x <= right) {
          r = m_color_d;
          g = m_color_e;
          b = m_color_f;
      } else {
          r = m_color_a;
          g = m_color_b;
          b = m_color_c;
      }

      ptr[c + 2] = m_blend(ptr[c + 2], r, m_opacity);
      ptr[c + 1] = m_blend(ptr[c + 1], g, m_opacity);
      ptr[c]     = m_blend(ptr[c],     b, m_opacity);
    }
  }
} // rectangle

void ImageSynth::synthesis(cv::Mat& image, nlohmann::json data, int page) {

  if(data["type"] == "fill") fill(image, data);
  if(data["type"] == "noise") noise(image, data);
  if(data["type"] == "walker") walker(image, data);
  if(data["type"] == "gradient") gradient(image, data, page);
  if(data["type"] == "rectangle") rectangle(image, data, page);

} // synthesis
