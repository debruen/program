
#ifndef math_h
#define math_h

#include <iostream>
#include <string>
#include <vector>
#include <cmath>

namespace math {

  double normalize(double& min, double& max, double value);

  double project(double& min, double& max, double& value);

  double circle(const double& min, const double& max, double& value);

  double radian(double& degree);

}

#endif /* math_h */
