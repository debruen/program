
#ifndef math_h
#define math_h

#include <iostream>
#include <string>
#include <vector>
#include <cmath>

namespace math {

  double normalize(const double& min, const double& max, const double& value);

  double project(const double& min, const double& max, const double& value);

  double circle(const double& min, const double& max, double& value);

  double radian(double& degree);

}

#endif /* math_h */
