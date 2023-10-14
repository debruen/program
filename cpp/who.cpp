#include "who.h"

// WHO :: blend functions

double who::blend::normal(double& a, double& b, double& o) {
    return  a * (1 - o) + b * o;
}
unsigned char who::blend::normal(unsigned char& aChar, unsigned char& bChar, double& o) {
    double a = static_cast<double>(aChar) / 255, b = static_cast<double>(bChar) / 255, c;
    c = normal(a, b, o);
    return round(c * 255);
}

// mean

double who::blend::arithmetic(double& a, double& b, double& o) {

    double c = (a + b) / 2;

    return  a * (1 - o) + c * o;
}
unsigned char who::blend::arithmetic(unsigned char& aChar, unsigned char& bChar, double& o) {
    double a = static_cast<double>(aChar) / 255, b = static_cast<double>(bChar) / 255, c;
    c = arithmetic(a, b, o);
    return round(c * 255);
}

double who::blend::geometric(double& a, double& b, double& o) {

    bool test = false;

    double temp = a * b;

    if(temp < 0) test = true;

    double c = sqrt(fabs(temp));

    if (test) c *= -1;

    return a * (1 - o) + c * o;
}
unsigned char who::blend::geometric(unsigned char& aChar, unsigned char& bChar, double& o) {
    double a = static_cast<double>(aChar) / 255, b = static_cast<double>(bChar) / 255, c;
    c = geometric(a, b, o);
    return round(c * 255);
}

double who::blend::harmonic(double& a, double& b, double& o) {

    double c = 2 / ((1 / a) + (1 / b));

    if(c > 1) c = 1;
    if(c < -1) c = -1;
    return a * (1 - o) + c * o;
}
unsigned char who::blend::harmonic(unsigned char& aChar, unsigned char& bChar, double& o) {
    double a = static_cast<double>(aChar) / 255, b = static_cast<double>(bChar) / 255, c;
    c = harmonic(a, b, o);
    return round(c * 255);
}

// blend

double who::blend::darken(double& a, double& b, double& o) {
    double c;

    if(a < b) c = a;
    if(a > b) c = b;

    return a * (1 - o) + c * o;
}
unsigned char who::blend::darken(unsigned char& aChar, unsigned char& bChar, double& o) {
    double a = static_cast<double>(aChar) / 255, b = static_cast<double>(bChar) / 255, c;
    c = darken(a, b, o);
    return round(c * 255);
}

double who::blend::multiply(double& a, double& b, double& o) {
    double c = a * b;
    return a * (1 - o) + c * o;
}
unsigned char who::blend::multiply(unsigned char& aChar, unsigned char& bChar, double& o) {
    double a = static_cast<double>(aChar) / 255, b = static_cast<double>(bChar) / 255, c;
    c = multiply(a, b, o);
    return round(c * 255);
}

double who::blend::colorburn(double& a, double& b, double& o) {
    double c = 1 - (1 - b) / a;

    if(c < 0) c = 0;
    return a * (1 - o) + c * o;
}
unsigned char who::blend::colorburn(unsigned char& aChar, unsigned char& bChar, double& o) {
    double a = static_cast<double>(aChar) / 255, b = static_cast<double>(bChar) / 255, c;
    c = colorburn(a, b, o);
    return round(c * 255);
}

double who::blend::linearburn(double& a, double& b, double& o) {
    double c = a + b - 1;

    if(c < 0) c = 0;
    return a * (1 - o) + c * o;
}
unsigned char who::blend::linearburn(unsigned char& aChar, unsigned char& bChar, double& o) {
    double a = static_cast<double>(aChar) / 255, b = static_cast<double>(bChar) / 255, c;
    c = linearburn(a, b, o);
    return round(c * 255);
}


double who::blend::lighten(double& a, double& b, double& o) {
    double c;

    if(b < a) c = a;
    if(b > a) c = b;

    return a * (1 - o) + c * o;
}
unsigned char who::blend::lighten(unsigned char& aChar, unsigned char& bChar, double& o) {
    double a = static_cast<double>(aChar) / 255, b = static_cast<double>(bChar) / 255, c;
    c = lighten(a, b, o);
    return round(c * 255);
}

double who::blend::screen(double& a, double& b, double& o) {

    double c = 1 - (1 - a) * (1 - b);

    return a * (1 - o) + c * o;
}
unsigned char who::blend::screen(unsigned char& aChar, unsigned char& bChar, double& o) {
    double a = static_cast<double>(aChar) / 255, b = static_cast<double>(bChar) / 255, c;
    c = screen(a, b, o);
    return round(c * 255);
}

double who::blend::colordodge(double& a, double& b, double& o) {

    double c = a / (1 - b);

    if(c > 1) c = 1;
    return a * (1 - o) + c * o;
}
unsigned char who::blend::colordodge(unsigned char& aChar, unsigned char& bChar, double& o) {
    double a = static_cast<double>(aChar) / 255, b = static_cast<double>(bChar) / 255, c;
    c = colordodge(a, b, o);
    return round(c * 255);
}

double who::blend::lineardodge(double& a, double& b, double& o) {

    double c = a + b;

    if(c > 1) c = 1;
    return a * (1 - o) + c * o;
}
unsigned char who::blend::lineardodge(unsigned char& aChar, unsigned char& bChar, double& o) {
    double a = static_cast<double>(aChar) / 255, b = static_cast<double>(bChar) / 255, c;
    c = lineardodge(a, b, o);
    return round(c * 255);
}

double who::blend::overlay(double& a, double& b, double& o) {
    double c;

    if(a < 0.5) {
        c = 2 * a * b;
    } else {
        c = 1 - 2 * (1 - a) * (1 - b);
    }

    return a * (1 - o) + c * o;
}
unsigned char who::blend::overlay(unsigned char& aChar, unsigned char& bChar, double& o) {
    double a = static_cast<double>(aChar) / 255, b = static_cast<double>(bChar) / 255, c;
    c = overlay(a, b, o);
    return round(c * 255);
}

double who::blend::softlight(double& a, double& b, double& o) {
    double c;

    if(b < 0.5) {
        c = 2 * a * b + (pow(a, 2) * (1 - 2 * b));
    } else {
        c = 2 * a * (1 - b) + (sqrt(a) * (2 * b - 1));
    }

    return a * (1 - o) + c * o;
}
unsigned char who::blend::softlight(unsigned char& aChar, unsigned char& bChar, double& o) {
    double a = static_cast<double>(aChar) / 255, b = static_cast<double>(bChar) / 255, c;
    c = softlight(a, b, o);
    return round(c * 255);
}

double who::blend::hardlight(double& a, double& b, double& o) {
    double c;

    if(b < 0.5) {
        c = 2 * b * a;
    } else {
        c = 1 - 2 * (1 - b) * (1 - a);
    }

    return a * (1 - o) + c * o;
}
unsigned char who::blend::hardlight(unsigned char& aChar, unsigned char& bChar, double& o) {
    double a = static_cast<double>(aChar) / 255, b = static_cast<double>(bChar) / 255, c;
    c = hardlight(a, b, o);
    return round(c * 255);
}

double who::blend::vividlight(double& a, double& b, double& o) {
    double c;

    if(a <= 0.5) c = 1 - (1 - b) / (2 * a);
    if(a > 0.5) c = b / (2 * (1 - a));

    if(c < 0) c = 0;
    if(c > 1) c = 1;
    return a * (1 - o) + c * o;
}
unsigned char who::blend::vividlight(unsigned char& aChar, unsigned char& bChar, double& o) {
    double a = static_cast<double>(aChar) / 255, b = static_cast<double>(bChar) / 255, c;
    c = vividlight(a, b, o);
    return round(c * 255);
}

double who::blend::linearlight(double& a, double& b, double& o) {

    double c = b + 2 * a - 1;

    if(c < 0) c = 0;
    if(c > 1) c = 1;
    return a * (1 - o) + c * o;
}
unsigned char who::blend::linearlight(unsigned char& aChar, unsigned char& bChar, double& o) {
    double a = static_cast<double>(aChar) / 255, b = static_cast<double>(bChar) / 255, c;
    c = linearlight(a, b, o);
    return round(c * 255);
}

double who::blend::pinlight(double& a, double& b, double& o) {
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
unsigned char who::blend::pinlight(unsigned char& aChar, unsigned char& bChar, double& o) {
    double a = static_cast<double>(aChar) / 255, b = static_cast<double>(bChar) / 255, c;
    c = pinlight(a, b, o);
    return round(c * 255);
}

double who::blend::hardmix(double& a, double& b, double& o) {
    double c;

    if(a < 1 - b) c = 0;
    if(a > 1 - b) c = 1;

    return a * (1 - o) + c * o;
}
unsigned char who::blend::hardmix(unsigned char& aChar, unsigned char& bChar, double& o) {
    double a = static_cast<double>(aChar) / 255, b = static_cast<double>(bChar) / 255, c;
    c = hardmix(a, b, o);
    return round(c * 255);
}

double who::blend::difference(double& a, double& b, double& o) {

    double c = fabs(a - b);

    return a * (1 - o) + c * o;
}
unsigned char who::blend::difference(unsigned char& aChar, unsigned char& bChar, double& o) {
    double a = static_cast<double>(aChar) / 255, b = static_cast<double>(bChar) / 255, c;
    c = difference(a, b, o);
    return round(c * 255);
}

double who::blend::exclusion(double& a, double& b, double& o) {

    double c = 0.5 - 2 * (a - 0.5) * (b - 0.5);

    return a * (1 - o) + c * o;
}
unsigned char who::blend::exclusion(unsigned char& aChar, unsigned char& bChar, double& o) {
    double a = static_cast<double>(aChar) / 255, b = static_cast<double>(bChar) / 255, c;
    c = exclusion(a, b, o);
    return round(c * 255);
}
