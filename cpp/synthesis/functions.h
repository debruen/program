
#ifndef functions_h
#define functions_h

#include <iostream>
#include <string>
#include <vector>
#include <cmath>
#include <random>

#include "nlohmann/json.hpp"

#include "stk/SineWave.h"

#include "opencv2/core.hpp"
#include "opencv2/highgui.hpp"
#include "opencv2/imgproc.hpp"


enum typ {IMAG, AUDI};

typedef uchar (*blendimage)(uchar&, uchar&, double&);
typedef double (*blendaudio)(double&, double&, double&);

namespace func {

  namespace blend {

    double normal(double& a, double& b, double& o);
    unsigned char normal(unsigned char& aChar, unsigned char& bChar, double& o);

    // mean
    double arithmetic(double& a, double& b, double& o);
    unsigned char arithmetic(unsigned char& aChar, unsigned char& bChar, double& o);

    double geometric(double& a, double& b, double& o);
    unsigned char geometric(unsigned char& aChar, unsigned char& bChar, double& o);

    double harmonic(double& a, double& b, double& o);
    unsigned char harmonic(unsigned char& aChar, unsigned char& bChar, double& o);

    // blend
    double darken(double& a, double& b, double& o);
    unsigned char darken(unsigned char& aChar, unsigned char& bChar, double& o);

    double multiply(double& a, double& b, double& o);
    unsigned char multiply(unsigned char& aChar, unsigned char& bChar, double& o);

    double colorburn(double& a, double& b, double& o);
    unsigned char colorburn(unsigned char& aChar, unsigned char& bChar, double& o);

    double linearburn(double& a, double& b, double& o);
    unsigned char linearburn(unsigned char& aChar, unsigned char& bChar, double& o);


    double lighten(double& a, double& b, double& o);
    unsigned char lighten(unsigned char& aChar, unsigned char& bChar, double& o);

    double screen(double& a, double& b, double& o);
    unsigned char screen(unsigned char& aChar, unsigned char& bChar, double& o);

    double colordodge(double& a, double& b, double& o);
    unsigned char colordodge(unsigned char& aChar, unsigned char& bChar, double& o);

    double lineardodge(double& a, double& b, double& o);
    unsigned char lineardodge(unsigned char& aChar, unsigned char& bChar, double& o);


    double overlay(double& a, double& b, double& o);
    unsigned char overlay(unsigned char& aChar, unsigned char& bChar, double& o);

    double softlight(double& a, double& b, double& o);
    unsigned char softlight(unsigned char& aChar, unsigned char& bChar, double& o);

    double hardlight(double& a, double& b, double& o);
    unsigned char hardlight(unsigned char& aChar, unsigned char& bChar, double& o);

    double vividlight(double& a, double& b, double& o);
    unsigned char vividlight(unsigned char& aChar, unsigned char& bChar, double& o);

    double linearlight(double& a, double& b, double& o);
    unsigned char linearlight(unsigned char& aChar, unsigned char& bChar, double& o);

    double pinlight(double& a, double& b, double& o);
    unsigned char pinlight(unsigned char& aChar, unsigned char& bChar, double& o);

    double hardmix(double& a, double& b, double& o);
    unsigned char hardmix(unsigned char& aChar, unsigned char& bChar, double& o);

    double difference(double& a, double& b, double& o);
    unsigned char difference(unsigned char& aChar, unsigned char& bChar, double& o);

    double exclusion(double& a, double& b, double& o);
    unsigned char exclusion(unsigned char& aChar, unsigned char& bChar, double& o);

  } // blend

  namespace scale { // scaling functions
    unsigned char projectChar(unsigned char min, unsigned char max, double v);
  }

  namespace number { // functions returning numbers
    double normDouble(double min, double max, double value); // returns 0.0 - 1.0 from value out of range min max
    double circle(double min, double max, double value);
    double radian(double degree);
  }

  void get_blend(blendimage& function, std::string blend = "normal");
  void get_blend(blendaudio& function, std::string blend = "normal");

  double point_gradient(unsigned int y, unsigned int x, unsigned int width, unsigned int height, double frequency, double phase, double tilt);

} // func

#endif /* functions_h */
