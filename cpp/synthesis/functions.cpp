
#include "functions.h"

double func::blend::normal(double& a, double& b, double& o) {
    return  a * (1 - o) + b * o;
}
unsigned char func::blend::normal(unsigned char& aChar, unsigned char& bChar, double& o) {
    double a = static_cast<double>(aChar) / 255, b = static_cast<double>(bChar) / 255, c;
    c = normal(a, b, o);
    return round(c * 255);
}

// mean

double func::blend::arithmetic(double& a, double& b, double& o) {

    double c = (a + b) / 2;

    return  a * (1 - o) + c * o;
}
unsigned char func::blend::arithmetic(unsigned char& aChar, unsigned char& bChar, double& o) {
    double a = static_cast<double>(aChar) / 255, b = static_cast<double>(bChar) / 255, c;
    c = arithmetic(a, b, o);
    return round(c * 255);
}

double func::blend::geometric(double& a, double& b, double& o) {

    bool test = false;

    double temp = a * b;

    if(temp < 0) test = true;

    double c = sqrt(fabs(temp));

    if (test) c *= -1;

    return a * (1 - o) + c * o;
}
unsigned char func::blend::geometric(unsigned char& aChar, unsigned char& bChar, double& o) {
    double a = static_cast<double>(aChar) / 255, b = static_cast<double>(bChar) / 255, c;
    c = geometric(a, b, o);
    return round(c * 255);
}

double func::blend::harmonic(double& a, double& b, double& o) {

    double c = 2 / ((1 / a) + (1 / b));

    if(c > 1) c = 1;
    if(c < -1) c = -1;
    return a * (1 - o) + c * o;
}
unsigned char func::blend::harmonic(unsigned char& aChar, unsigned char& bChar, double& o) {
    double a = static_cast<double>(aChar) / 255, b = static_cast<double>(bChar) / 255, c;
    c = harmonic(a, b, o);
    return round(c * 255);
}

// blend

double func::blend::darken(double& a, double& b, double& o) {
    double c;

    if(a < b) c = a;
    if(a > b) c = b;

    return a * (1 - o) + c * o;
}
unsigned char func::blend::darken(unsigned char& aChar, unsigned char& bChar, double& o) {
    double a = static_cast<double>(aChar) / 255, b = static_cast<double>(bChar) / 255, c;
    c = darken(a, b, o);
    return round(c * 255);
}

double func::blend::multiply(double& a, double& b, double& o) {
    double c = a * b;
    return a * (1 - o) + c * o;
}
unsigned char func::blend::multiply(unsigned char& aChar, unsigned char& bChar, double& o) {
    double a = static_cast<double>(aChar) / 255, b = static_cast<double>(bChar) / 255, c;
    c = multiply(a, b, o);
    return round(c * 255);
}

double func::blend::colorburn(double& a, double& b, double& o) {
    double c = 1 - (1 - b) / a;

    if(c < 0) c = 0;
    return a * (1 - o) + c * o;
}
unsigned char func::blend::colorburn(unsigned char& aChar, unsigned char& bChar, double& o) {
    double a = static_cast<double>(aChar) / 255, b = static_cast<double>(bChar) / 255, c;
    c = colorburn(a, b, o);
    return round(c * 255);
}

double func::blend::linearburn(double& a, double& b, double& o) {
    double c = a + b - 1;

    if(c < 0) c = 0;
    return a * (1 - o) + c * o;
}
unsigned char func::blend::linearburn(unsigned char& aChar, unsigned char& bChar, double& o) {
    double a = static_cast<double>(aChar) / 255, b = static_cast<double>(bChar) / 255, c;
    c = linearburn(a, b, o);
    return round(c * 255);
}


double func::blend::lighten(double& a, double& b, double& o) {
    double c;

    if(b < a) c = a;
    if(b > a) c = b;

    return a * (1 - o) + c * o;
}
unsigned char func::blend::lighten(unsigned char& aChar, unsigned char& bChar, double& o) {
    double a = static_cast<double>(aChar) / 255, b = static_cast<double>(bChar) / 255, c;
    c = lighten(a, b, o);
    return round(c * 255);
}

double func::blend::screen(double& a, double& b, double& o) {

    double c = 1 - (1 - a) * (1 - b);

    return a * (1 - o) + c * o;
}
unsigned char func::blend::screen(unsigned char& aChar, unsigned char& bChar, double& o) {
    double a = static_cast<double>(aChar) / 255, b = static_cast<double>(bChar) / 255, c;
    c = screen(a, b, o);
    return round(c * 255);
}

double func::blend::colordodge(double& a, double& b, double& o) {

    double c = a / (1 - b);

    if(c > 1) c = 1;
    return a * (1 - o) + c * o;
}
unsigned char func::blend::colordodge(unsigned char& aChar, unsigned char& bChar, double& o) {
    double a = static_cast<double>(aChar) / 255, b = static_cast<double>(bChar) / 255, c;
    c = colordodge(a, b, o);
    return round(c * 255);
}

double func::blend::lineardodge(double& a, double& b, double& o) {

    double c = a + b;

    if(c > 1) c = 1;
    return a * (1 - o) + c * o;
}
unsigned char func::blend::lineardodge(unsigned char& aChar, unsigned char& bChar, double& o) {
    double a = static_cast<double>(aChar) / 255, b = static_cast<double>(bChar) / 255, c;
    c = lineardodge(a, b, o);
    return round(c * 255);
}

double func::blend::overlay(double& a, double& b, double& o) {
    double c;

    if(a < 0.5) {
        c = 2 * a * b;
    } else {
        c = 1 - 2 * (1 - a) * (1 - b);
    }

    return a * (1 - o) + c * o;
}
unsigned char func::blend::overlay(unsigned char& aChar, unsigned char& bChar, double& o) {
    double a = static_cast<double>(aChar) / 255, b = static_cast<double>(bChar) / 255, c;
    c = overlay(a, b, o);
    return round(c * 255);
}

double func::blend::softlight(double& a, double& b, double& o) {
    double c;

    if(b < 0.5) {
        c = 2 * a * b + (pow(a, 2) * (1 - 2 * b));
    } else {
        c = 2 * a * (1 - b) + (sqrt(a) * (2 * b - 1));
    }

    return a * (1 - o) + c * o;
}
unsigned char func::blend::softlight(unsigned char& aChar, unsigned char& bChar, double& o) {
    double a = static_cast<double>(aChar) / 255, b = static_cast<double>(bChar) / 255, c;
    c = softlight(a, b, o);
    return round(c * 255);
}

double func::blend::hardlight(double& a, double& b, double& o) {
    double c;

    if(b < 0.5) {
        c = 2 * b * a;
    } else {
        c = 1 - 2 * (1 - b) * (1 - a);
    }

    return a * (1 - o) + c * o;
}
unsigned char func::blend::hardlight(unsigned char& aChar, unsigned char& bChar, double& o) {
    double a = static_cast<double>(aChar) / 255, b = static_cast<double>(bChar) / 255, c;
    c = hardlight(a, b, o);
    return round(c * 255);
}

double func::blend::vividlight(double& a, double& b, double& o) {
    double c;

    if(a <= 0.5) c = 1 - (1 - b) / (2 * a);
    if(a > 0.5) c = b / (2 * (1 - a));

    if(c < 0) c = 0;
    if(c > 1) c = 1;
    return a * (1 - o) + c * o;
}
unsigned char func::blend::vividlight(unsigned char& aChar, unsigned char& bChar, double& o) {
    double a = static_cast<double>(aChar) / 255, b = static_cast<double>(bChar) / 255, c;
    c = vividlight(a, b, o);
    return round(c * 255);
}

double func::blend::linearlight(double& a, double& b, double& o) {

    double c = b + 2 * a - 1;

    if(c < 0) c = 0;
    if(c > 1) c = 1;
    return a * (1 - o) + c * o;
}
unsigned char func::blend::linearlight(unsigned char& aChar, unsigned char& bChar, double& o) {
    double a = static_cast<double>(aChar) / 255, b = static_cast<double>(bChar) / 255, c;
    c = linearlight(a, b, o);
    return round(c * 255);
}

double func::blend::pinlight(double& a, double& b, double& o) {
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
unsigned char func::blend::pinlight(unsigned char& aChar, unsigned char& bChar, double& o) {
    double a = static_cast<double>(aChar) / 255, b = static_cast<double>(bChar) / 255, c;
    c = pinlight(a, b, o);
    return round(c * 255);
}

double func::blend::hardmix(double& a, double& b, double& o) {
    double c;

    if(a < 1 - b) c = 0;
    if(a > 1 - b) c = 1;

    return a * (1 - o) + c * o;
}
unsigned char func::blend::hardmix(unsigned char& aChar, unsigned char& bChar, double& o) {
    double a = static_cast<double>(aChar) / 255, b = static_cast<double>(bChar) / 255, c;
    c = hardmix(a, b, o);
    return round(c * 255);
}

double func::blend::difference(double& a, double& b, double& o) {

    double c = fabs(a - b);

    return a * (1 - o) + c * o;
}
unsigned char func::blend::difference(unsigned char& aChar, unsigned char& bChar, double& o) {
    double a = static_cast<double>(aChar) / 255, b = static_cast<double>(bChar) / 255, c;
    c = difference(a, b, o);
    return round(c * 255);
}

double func::blend::exclusion(double& a, double& b, double& o) {

    double c = 0.5 - 2 * (a - 0.5) * (b - 0.5);

    return a * (1 - o) + c * o;
}
unsigned char func::blend::exclusion(unsigned char& aChar, unsigned char& bChar, double& o) {
    double a = static_cast<double>(aChar) / 255, b = static_cast<double>(bChar) / 255, c;
    c = exclusion(a, b, o);
    return round(c * 255);
}

unsigned char func::scale::projectChar(unsigned char min, unsigned char max, double v) { // from normalized to range
    return round((v * (static_cast<double>(max) - static_cast<double>(min))) + static_cast<double>(min));
}

double func::number::normDouble(double min, double max, double value) {
    return (value - min) / (max - min);
}

double func::number::circle(double min, double max, double value) {
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

double func::number::radian(double degree) {
    degree = circle(0, 360, degree);
    return degree * M_PI / 180;
}

void func::get_blend(blendimage& function, std::string blend) {

  if(blend == "normal") function = func::blend::normal;
  if(blend == "arithmetic") function = func::blend::arithmetic;
  if(blend == "geometric") function = func::blend::geometric;
  if(blend == "harmonic") function = func::blend::harmonic;
  if(blend == "darken") function = func::blend::darken;
  if(blend == "multiply") function = func::blend::multiply;
  if(blend == "colom_temp_color_durn") function = func::blend::colorburn;
  if(blend == "lineam_temp_color_durn") function = func::blend::linearburn;
  if(blend == "lighten") function = func::blend::lighten;
  if(blend == "screen") function = func::blend::screen;
  if(blend == "colordodge") function = func::blend::colordodge;
  if(blend == "lineardodge") function = func::blend::lineardodge;
  if(blend == "overlay") function = func::blend::overlay;
  if(blend == "softlight") function = func::blend::softlight;
  if(blend == "hardlight") function = func::blend::hardlight;
  if(blend == "vividlight") function = func::blend::vividlight;
  if(blend == "linearlight") function = func::blend::linearlight;
  if(blend == "pinlight") function = func::blend::pinlight;
  if(blend == "hardmix") function = func::blend::hardmix;
  if(blend == "difference") function = func::blend::difference;
  if(blend == "exclusion") function = func::blend::exclusion;

} // get_blend

void func::get_blend(blendaudio& function, std::string blend) {

  if(blend == "normal") function = func::blend::normal;
  if(blend == "arithmetic") function = func::blend::arithmetic;
  if(blend == "geometric") function = func::blend::geometric;
  if(blend == "harmonic") function = func::blend::harmonic;
  if(blend == "darken") function = func::blend::darken;
  if(blend == "multiply") function = func::blend::multiply;
  if(blend == "colom_temp_color_durn") function = func::blend::colorburn;
  if(blend == "lineam_temp_color_durn") function = func::blend::linearburn;
  if(blend == "lighten") function = func::blend::lighten;
  if(blend == "screen") function = func::blend::screen;
  if(blend == "colordodge") function = func::blend::colordodge;
  if(blend == "lineardodge") function = func::blend::lineardodge;
  if(blend == "overlay") function = func::blend::overlay;
  if(blend == "softlight") function = func::blend::softlight;
  if(blend == "hardlight") function = func::blend::hardlight;
  if(blend == "vividlight") function = func::blend::vividlight;
  if(blend == "linearlight") function = func::blend::linearlight;
  if(blend == "pinlight") function = func::blend::pinlight;
  if(blend == "hardmix") function = func::blend::hardmix;
  if(blend == "difference") function = func::blend::difference;
  if(blend == "exclusion") function = func::blend::exclusion;

} // get_blend

double func::point_gradient(unsigned int y, unsigned int x, unsigned int width, unsigned int height, double frequency, double phase, double tilt) {

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
    sinus = sin(number::radian(degrees));

    return number::normDouble(-1, 1, sinus);
} // point_gradient
