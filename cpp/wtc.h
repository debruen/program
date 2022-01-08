#ifndef wtc_h
#define wtc_h

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <cmath>
#include <complex>
#include <random>
#include <sys/stat.h>

#include <fftw3.h>

#define cimg_use_jpeg
#define cimg_use_png
#define cimg_use_tiff

// #include "CImg.h"

#include "stk/SineWave.h"

enum TYPE {
    AUDIO,
    IMAGE,
    TEXT,
    NONE
};

enum SIZE {
    A,
    B,
    C
};

namespace wtc {

    const unsigned int W = 1024;
    const unsigned int H = 1448;

    unsigned int width(SIZE size = A);
    unsigned int height(SIZE size = A);
    unsigned int length(SIZE ssize = A);
    unsigned int bands(SIZE ssize = A);

    TYPE getType(std::string& filename);

    namespace scale { // scaling functions
        double normalize(double min, double max, double v);
        double project(double min, double max, double v);
        unsigned char projectChar(unsigned char min, unsigned char max, double v);

        // 2d to normalized by min max
        // void normalizeLayer(int layer, cimg_library::CImg<unsigned char>& image);

        std::vector< std::vector<double> > normalize2dInt(std::vector< std::vector<unsigned int> >& v);
    }

    namespace merge { // merge functions
        double quick(double a, double b, int i = 0, double o = 1.0);

        unsigned char fade(unsigned char a, unsigned char b, double o = 0.5);

        // mean
        unsigned char arithmetic(unsigned char a, unsigned char b, double o = 1.0);
        unsigned char geometric(unsigned char a, unsigned char b, double o = 1.0);
        unsigned char harmonic(unsigned char a, unsigned char b, double o = 1.0);

        // blend
        unsigned char multiply(unsigned char a, unsigned char b, double o = 1.0);
        unsigned char screen(unsigned char a, unsigned char b, double o = 1.0);
        unsigned char overlay(unsigned char a, unsigned char b, double o = 1.0);
        unsigned char hardlight(unsigned char a, unsigned char b, double o = 1.0);
        unsigned char softlight(unsigned char a, unsigned char b, double o = 1.0);
    }

    namespace fs { // filesystem functions
        std::string home();
        std::string getFile(std::string& filename);
        bool fileExists(const std::string& file);
        std::string getFreeFile(std::string& filename, std::string& path, TYPE t);
        std::string imageFile(std::string& filename, std::string& path);
        std::string audioFile(std::string& filename, std::string& path);
        std::string textFile(std::string& filename, std::string& path);
        std::string blankFile(std::string& filename, std::string& path);

        double hanning (int& i, int& n, double& value);
    }

    namespace number { // functions returning numbers
        double randomDouble(double min = 0.0, double max = 1.0);
        double randomFixed(double min = 0.0, double max = 1.0);
        int randomInt(int min = 0, int max = 255);
        unsigned char randomChar(unsigned char min = 0, unsigned char max = 255);
        unsigned char noiseChar(unsigned char value, unsigned char noise = 10);
        double normDouble(double min, double max, double value); // returns 0.0 - 1.0 from value out of range min max
        double normSignal(double min, double max, double value); // returns -1.0 - 1.0 from value out of range min max
        double normToSignal(double value);                       // returns -1.0 - 1.0 from value out of range 0.0 to 1.0
        double circle(double min, double max, double value);
        double radian(double degree);
        double curve(unsigned int i, double frequency = 440.0, double phase = 0.0, double volume = 0.66);
    }

    namespace v1d { // 1d vector calculations
        std::vector<double> minMax(std::vector<double>& v);
        std::vector<unsigned int> minMaxInt(std::vector<unsigned int>& v);

        std::vector<double> fft(std::vector<double> input);
        std::vector<double> norm1d(std::vector<double> vector);
    }

    namespace v2d { // 1d vector calculations
        std::vector< std::vector<double> > sinus(unsigned int n);
        std::vector< std::vector<double> > monochromeSignal(std::vector< std::vector< std::vector<double> > > vector);
        std::vector< std::vector<double> > baseVolToneA(std::vector< std::vector< std::vector<double> > > vector);
        std::vector< std::vector<double> > baseVolToneB(std::vector< std::vector< std::vector<double> > > vector);
        // std::vector< std::vector<double> > gradient(unsigned int width, unsigned int height, double rot = 1.0, double off = 0.0, double ang = 0.0);
        // std::vector< std::vector< unsigned char > > walker(unsigned int width, unsigned int height);

        void signalStrech(std::vector< std::vector<double> >& vector);
    }

    namespace v3d { // 1d vector calculations
        std::vector< std::vector< std::vector<double> > > fourier(std::vector< std::vector<double> >& signal);

        void norm3dDouble(std::vector< std::vector< std::vector<double> > >& vector);
    }

    namespace color {
        std::vector<double> LabRange();
        std::vector<double> HslRange();

        std::vector<double> rgbToHsl(unsigned char& R, unsigned char& G, unsigned char& B);
        std::vector<unsigned char> hslToRgb(std::vector<double> hsl);
    }

}

#endif /* wtc_h */
