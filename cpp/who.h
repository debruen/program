#ifndef who_h
#define who_h

#include <cmath>

namespace who {

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

} // who

#endif /* who_h */
