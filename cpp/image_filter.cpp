#include "image_filter.h"

ImageFilter::ImageFilter() {

  m_default["process"] = "filter";
  m_default["type"] = "color_scale";
  m_default["color"] = { 0, 0, 0 };

}

nlohmann::json ImageFilter::get_default() {
  return m_default;
}

void ImageFilter::color_scale(cv::Mat& image, nlohmann::json data) {

  cv::Mat table(1, 256, CV_8UC3), temp;
  uchar rTemp, gTemp, bTemp;
  double v;

  for (int i = 0; i < 256; ++i) {
    v = static_cast<double>(i) / 255;

    rTemp = wtc::scale::projectChar(data["color"][0], data["color"][3], v);
    gTemp = wtc::scale::projectChar(data["color"][1], data["color"][4], v);
    bTemp = wtc::scale::projectChar(data["color"][2], data["color"][5], v);

    table.at<cv::Vec3b>(0,i) = cv::Vec3b(bTemp, gTemp, rTemp);
  }

  cv::LUT(image, table, temp);

  image = temp;
} // color scale

void ImageFilter::color_strech(cv::Mat& image, nlohmann::json data) {

  cv::Mat table(1, 256, CV_8UC3), temp;
  uchar rA, rB, gA, gB, bA, bB, tempR, tempG, tempB;
  double value, valueR, valueG, valueB;

  rA = data["color"][0];
  gA = data["color"][1];
  bA = data["color"][2];
  rB = data["color"][3];
  gB = data["color"][4];
  bB = data["color"][5];

  for (int i = 0; i < 256; ++i) {
    value = static_cast<double>(i) / 255;

    valueR = (rA - rB) * value;
    valueG = (gA - gB) * value;
    valueB = (bA - bB) * value;

    tempR = wtc::scale::projectChar(rA, rB, valueR);
    tempG = wtc::scale::projectChar(gA, gB, valueG);
    tempB = wtc::scale::projectChar(bA, bB, valueB);

    table.at<cv::Vec3b>(0,i) = cv::Vec3b(tempB, tempG, tempR);
  }

  cv::LUT(image, table, temp);

  image = temp;
} // color strech

void ImageFilter::color_jump(cv::Mat& image, nlohmann::json data) {

  cv::Mat table(1, 256, CV_8UC3), temp;
  uchar rA, rB, gA, gB, bA, bB, tempR, tempG, tempB;
  double value, valueR, valueG, valueB;

  rA = data["color"][0];
  gA = data["color"][1];
  bA = data["color"][2];
  rB = data["color"][3];
  gB = data["color"][4];
  bB = data["color"][5];

  for (int i = 0; i < 256; ++i) {
    value = static_cast<double>(i) / 255;

    valueR = (rA - rB) * value;
    valueG = (gA - gB) * value;
    valueB = (bA - bB) * value;

    tempR = wtc::scale::projectChar(rA, rB, valueR);
    tempG = wtc::scale::projectChar(gA, gB, valueG);
    tempB = wtc::scale::projectChar(bA, bB, valueB);

    table.at<cv::Vec3b>(0,i) = cv::Vec3b(tempB, tempG, tempR);
  }

  cv::LUT(image, table, temp);

  image = temp;
} // color jump

void ImageFilter::gamma(cv::Mat& image, nlohmann::json data) {

  cv::Mat table(1, 256, CV_8U), temp;
  uchar color;
  uchar* p = table.ptr();

  for(int i = 0; i < 256; ++i) {
    color = round( pow(static_cast<double>(i) / 255.0, data["gamma"]) * 255 );
    p[i] = color;
  }

  cv::LUT(image, table, temp);

  image = temp;
} // color
