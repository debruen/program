
#include "math.h"

/// takes value in the range min, max
/// returns value in the range of 0 to 1

double math::normalize(double& min, double& max, double value) {
  return (value - min) / (max - min);
}

/// takes value in the range 0 to 1
/// returns value in the range min, max

double math::project(double& min, double& max, double& value) {
  return (value * (max - min)) + min;
}

double math::circle(const double& min, const double& max, double& value) {

  if(value > max) {
    value = value - max;
  }
  if(value < min) {
    value = value + max;
  }
  if(value > max || value < min) {
    circle(min, max, value);
  }

  return value;
}

double math::radian(double& degree) {
//  return math::circle(0, 360, degree) * M_PI / 180;
  return degree * M_PI / 180;
}
