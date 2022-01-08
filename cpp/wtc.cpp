#include "wtc.h"

namespace wtc {

  unsigned int width(SIZE s) {
    unsigned int result = W;
    if(s == A) result = W;
    if(s == B) result = H;
    if(s == C) result = W * 2;
    return result;
  }

  unsigned int height(SIZE s) {
    unsigned int result = H;
    if(s == A) result = H;
    if(s == B) result = W * 2;
    if(s == C) result = H * 2;
    return result;
  }

  unsigned int length(SIZE s) {
    unsigned int w, h;
    w = width(s);
    h = height(s);
    return round(pow(static_cast<double>(w), 2.0) / (h / static_cast<double>(w)) / 4.0 / 2.0);
  }

  unsigned int bands(SIZE s) {
    return width(s) / 2;
  }

  TYPE getType(std::string& filename) {
    TYPE type = NONE;
    std::vector<std::string> imageType{".jpg", ".jpeg", ".png", ".tif", ".tiff"}, audioType{".aif", ".aiff"};

    for(int i = 0; i < static_cast<int>(imageType.size()); ++i) {
      if (filename.find(imageType[i]) != std::string::npos) {
        type = IMAGE;
        break;
      }
    }

    for(int i = 0; i < static_cast<int>(audioType.size()); ++i) {
      if (filename.find(audioType[i]) != std::string::npos) {
        type = AUDIO;
        break;
      }
    }
    return type;
  }

    namespace scale { // ------------------------------------------------------------ scale ----

        double normalize(double min, double max, double v) {                      // from range to normalized
            return (v - min) / (max - min);
        }
        double normalizeInt(unsigned int min, unsigned int max, unsigned int v) { // from range int to normalized
            return (static_cast<double>(v) - static_cast<double>(min)) / (static_cast<double>(max) - static_cast<double>(min));
        }
        double project(double min, double max, double v) {                        // from normalized to range
            return (v * (max - min)) + min;
        }
        unsigned char projectChar(unsigned char min, unsigned char max, double v) { // from normalized to range
            return round((v * (static_cast<double>(max) - static_cast<double>(min))) + static_cast<double>(min));
        }
        // void normalizeLayer(int layer, cimg_library::CImg<unsigned char>& image) {
        //     std::vector<unsigned int> check, range;
        //     for (unsigned int y = 0; y < static_cast<unsigned int>(image.height()); ++y) {
        //         for (unsigned int x = 0; x < static_cast<unsigned int>(image.width()); ++x) {
        //             check.push_back(image(x,y,0,layer));
        //         }
        //     }
        //     range = v1d::minMaxInt(check);
        //
        //     for (unsigned int y = 0; y < static_cast<unsigned int>(image.height()); ++y) {
        //         for (unsigned int x = 0; x < static_cast<unsigned int>(image.width()); ++x) {
        //             image(x,y,0,layer) = round(normalizeInt(range[0], range[1], image(x,y,0,layer)) * 255);
        //         }
        //     }
        // }

        std::vector< std::vector<double> > normalize2dInt(std::vector< std::vector<unsigned int> >& v) {
            std::vector<unsigned int> check, range;
            for (unsigned int i = 0; i < v.size(); ++i) {
                for (unsigned int j = 0; j < v[0].size(); ++j) {
                    check.push_back(v[i][j]);
                }
            }
            range = v1d::minMaxInt(check);

            std::vector< std::vector<double> > vector;
            for (unsigned int i = 0; i < v.size(); ++i) {
                std::vector<double> temp;
                for (unsigned int j = 0; j < v[0].size(); ++j) {
                    temp.push_back(normalizeInt(range[0], range[1], v[i][j]));
                }
                vector.push_back(temp);
            }
            return vector;
        }

    } // ---------------------------------------------------------------------------- scale ---- end

    namespace merge { // ------------------------------------------------------------ merge ----
                      // values a, b, result c, fade f, opacity of b = o, quick index = i
                      // values, fade, opacity 0.0 - 1.0, index = 0 - (mean + blend - 1)
                      // ------------------------------------------------------------ merge ----

        double quick(double a, double b, int i, double o) {
            if(i == 0) {
                return arithmetic(a, b, o);
            } else if (i == 1) {
                return geometric(a, b, o);
            } else if (i == 2) {
                return harmonic(a, b, o);
            } else if (i == 3) {
                return multiply(a, b, o);
            } else if (i == 4) {
                return screen(a, b, o);
            } else if (i == 5) {
                return overlay(a, b, o);
            } else if (i == 6) {
                return hardlight(a, b, o);
            } else {
                return softlight(a, b, o);
            }
        }

        unsigned char fade(unsigned char aChar, unsigned char bChar, double o) {
            double a = static_cast<double>(aChar) / 255, b = static_cast<double>(bChar) / 255, d;
            d = a * (1 - o) + b * o;
            return round(d * 255);
        }

        // mean
        unsigned char arithmetic(unsigned char aChar, unsigned char bChar, double o) { // 0
            double a = static_cast<double>(aChar) / 255, b = static_cast<double>(bChar) / 255, c, d;
            c = (a + b) / 2;
            d = a * (1 - o) + c * o;
            return round(d * 255);
        }
        unsigned char geometric(unsigned char aChar, unsigned char bChar, double o) {  // 1
            double a = static_cast<double>(aChar) / 255, b = static_cast<double>(bChar) / 255, c, d;
            c = sqrt(a * b);
            d = a * (1 - o) + c * o;
            return round(d * 255);
        }
        unsigned char harmonic(unsigned char aChar, unsigned char bChar, double o) {   // 2
            double a = static_cast<double>(aChar) / 255, b = static_cast<double>(bChar) / 255, c, d;
            c = 2 / ((1 / a) + (1 / b));
            d = a * (1 - o) + c * o;
            return round(d * 255);
        }

        // blend
        unsigned char multiply(unsigned char aChar, unsigned char bChar, double o) {   // 3
            double a = static_cast<double>(aChar) / 255, b = static_cast<double>(bChar) / 255, c, d;
            c = a * b;
            d = a * (1 - o) + c * o;
            return round(d * 255);
        }
        unsigned char screen(unsigned char aChar, unsigned char bChar, double o) {     // 4
            double a = static_cast<double>(aChar) / 255, b = static_cast<double>(bChar) / 255, c, d;
            c = 1 - (1 - a) * (1 - b);
            d = a * (1 - o) + c * o;
            return round(d * 255);
        }
        unsigned char overlay(unsigned char aChar, unsigned char bChar, double o) {    // 5
            double a = static_cast<double>(aChar) / 255, b = static_cast<double>(bChar) / 255, c, d;
            if(a < 0.5) {
                c = 2 * a * b;
            } else {
                c = 1 - 2 * (1 - a) * (1 - b);
            }
            d = a * (1 - o) + c * o;
            return round(d * 255);
        }
        unsigned char hardlight(unsigned char aChar, unsigned char bChar, double o) {  // 6 not quiet right!
            double a = static_cast<double>(aChar) / 255, b = static_cast<double>(bChar) / 255, c, d;
            if(b < 0.5) {
                c = 2 * b * a;
            } else {
                c = 1 - 2 * (1 - b) * (1 - a);
            }
            d = a * (1 - o) + c * o;
            return round(d * 255);
        }
        unsigned char softlight(unsigned char aChar, unsigned char bChar, double o) {  // 7
            double a = static_cast<double>(aChar) / 255, b = static_cast<double>(bChar) / 255, c, d;
            if(b < 0.5) {
                c = 2 * a * b + (pow(a, 2) * (1 - 2 * b));
            } else {
                c = 2 * a * (1 - b) + (sqrt(a) * (2 * b - 1));
            }
            d = a * (1 - o) + c * o;
            return round(d * 255);
        }
    } // ---------------------------------------------------------------------------- merge ----


    namespace fs { // --------------------------------------------------------------- fs ---

        std::string home() {
            return getenv("HOME");
        }

        std::string getFile(std::string& filename){
            return home() + "/Desktop/in/" + filename;
        }

        bool fileExists(const std::string& file) {
            struct stat buffer;
            return (stat(file.c_str(), &buffer) == 0);
        }

        std::string getFreeFile(std::string& filename, std::string& path, TYPE t) {

            std::string index, imageTest, audioTest, textTest, file;

            // std::string path = home() + "/Desktop/out/";
            // path = home() + path;

            for(int i = 0; i < 10000; ++i) {
                if(i < 10 - 1) {
                    index = "000" + std::to_string(i + 1);
                } else if(i < 100 - 1) {
                    index = "00" + std::to_string(i + 1);
                } else if(i < 1000 - 1) {
                    index = "0" + std::to_string(i + 1);
                } else {
                    index = std::to_string(i + 1);
                }

                imageTest = path + filename + index + ".tiff";
                audioTest = path + filename + index + ".aiff";
                textTest = path + filename + index + ".txt";

                if(!fileExists(imageTest) && !fileExists(audioTest)) {
                    break;
                }
            }

            if(t == IMAGE) {
                file = imageTest;
            } else if(t == AUDIO) {
                file = audioTest;
            } else if(t == TEXT) {
                file = textTest;
            } else {
                file = path + filename + index;
            }

            return file;
        }

        std::string imageFile(std::string& filename, std::string& path) {
            return getFreeFile(filename, path, IMAGE);
        }

        std::string audioFile(std::string& filename, std::string& path) {
            return getFreeFile(filename, path, AUDIO);
        }

        std::string textFile(std::string& filename, std::string& path) {
            return getFreeFile(filename, path, TEXT);
        }

        std::string blankFile(std::string& filename, std::string& path) {
            return getFreeFile(filename, path, NONE);
        }
    } // ---------------------------------------------------------------------------- fs ---- end

    namespace number { // ----------------------------------------------------------- number ----

        double randomDouble(double min, double max) {
            std::random_device generator;
            std::uniform_real_distribution<double> distribution(min,max);

            return distribution(generator);
        }

        double randomFixed(double min, double max) {
            std::mt19937 generator(0);
            std::uniform_real_distribution<double> distribution(min,max);

            return distribution(generator);
        }

        int randomInt(int min, int max) {
            std::random_device generator;
            std::uniform_int_distribution<int> distribution(min,max);

            return distribution(generator);
        }

        unsigned char randomChar(unsigned char min, unsigned char max) {
            std::random_device generator;
            std::uniform_int_distribution<int> distribution(min,max);

            return distribution(generator);
        }

        unsigned char noiseChar(unsigned char value, unsigned char noise) {
            int max, min;
            max = value + noise;
            if(max > 255) max = 255;
            min = value - noise;
            if(min < 0) min = 0;

            return randomChar(max, min);
        }

        double normDouble(double min, double max, double value) {
            return (value - min) / (max - min);
        }

        double normToSignal(double value) {
            return value * 2 - 1;
        }

        double normSignal(double min, double max, double value) {
            return normToSignal(normDouble(min, max, value));
        }

        double circle(double min, double max, double value) {
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

        double radian(double degree) {
            degree = circle(0, 360, degree);
            return degree * M_PI / 180;
        }

        double curve(unsigned int i, double frequency, double phase, double volume) {
            if(frequency <= 0){
                std::cout << "frequency = " << frequency << " it has to be bigger than 0. i set it to 1 !" << '\n' << '\n';
                frequency = 1;
            }

            double n, d, p;

            n = 44100.0;
            d = 360.0 * frequency;
            p = round(phase * n / frequency);

            return sin( radian( ( ( static_cast<double>(i) + p ) / n ) * d ) ) * volume;
        }

        double hanning (int& i, int& n, double& value) {

            double multiplier = 0.5 * (1 - cos(2 * M_PI * i / (n-1)));

            return value * multiplier;
        }
    } // --------------------------------------------------------------------------- number ---- end

    namespace v1d { // ------------------------------------------------------------- v1d ----

        std::vector<double> minMax(std::vector<double>& v) {
            std::vector<double> temp;

            temp.push_back(*std::min_element(v.begin(), v.end()));
            temp.push_back(*std::max_element(v.begin(), v.end()));

            return temp;
        }

        std::vector<unsigned int> minMaxInt(std::vector<unsigned int>& v) {
            std::vector<unsigned int> temp;

            temp.push_back(*std::min_element(v.begin(), v.end()));
            temp.push_back(*std::max_element(v.begin(), v.end()));

            return temp;
        }

        std::vector<double> fft(std::vector<double> input) {
            fftw_complex *fftin, *fftout;
            fftw_plan fftplan;

            fftin = (fftw_complex*) fftw_malloc(sizeof(fftw_complex) * input.size()); // allocating memory
            fftout = (fftw_complex*) fftw_malloc(sizeof(fftw_complex) * input.size());
            fftplan = fftw_plan_dft_1d(input.size(), fftin, fftout, FFTW_FORWARD, FFTW_ESTIMATE); // Here we set which kind of transformation we want to perform

            for (int i = 0; i < static_cast<int>(input.size()); ++i) {
                fftin[i][0] = input[i];
                fftin[i][1] = 0.0;
            }

            fftw_execute(fftplan);

            std::vector<double> band;

            for (int i = 0; i < static_cast<int>(input.size()) / 2; ++i) {

                band.push_back(sqrt(fftout[i][0] * fftout[i][0] + fftout[i][1] * fftout[i][1]));
            }

            fftw_destroy_plan(fftplan);
            fftw_cleanup();

            return band;
        }

        std::vector<double> norm1d(std::vector<double> vector) {
            std::vector<double> range = minMax(vector);

            std::vector<double> result;

            for (unsigned int i = 0; i < vector.size(); ++i) {
                result.push_back(number::normDouble(range[0], range[1], vector[i]));
            }
            return result;
        }
    } // --------------------------------------------------------------------------- v1d ---- end

    namespace v2d { // ------------------------------------------------------------- v2d ----
        std::vector< std::vector<double> > sinus(unsigned int n) {

            std::vector< std::vector<double> > signal;
            std::vector<double> L, R;

            for (unsigned int i = 0; i < n; ++i) {
                L.push_back(number::curve(i));
                R.push_back(number::curve(i));
            }
            signal.push_back(L);
            signal.push_back(R);

            return signal;
        }

        std::vector< std::vector<double> > monochromeSignal(std::vector< std::vector< std::vector<double> > > vector) {

            double left, right, l, r, discrete;

            std::vector< std::vector<double> > signal;
            std::vector<double> L, R;

            for (unsigned int y = 0; y < vector.size(); ++y) {

                left = 0;
                right = 0;

                for (unsigned int x = 0; x < vector[0].size(); ++x) {

                    l = x / (static_cast<double>(vector[0].size()) - 1.0);
                    r = 1.0 - (x/(static_cast<double>(vector[0].size()) - 1.0));

                    discrete = number::normToSignal( vector[y][x][1] ) / (static_cast<double>(vector[0].size()) - 1.0) * 4;

                    left = left + discrete * l;
                    right = right + discrete * r;

                }

                L.push_back(left);
                R.push_back(right);
            }

            signal.push_back(L);
            signal.push_back(R);

            return signal;
        }

        std::vector< std::vector<double> > baseVolToneB(std::vector< std::vector< std::vector<double> > > vector) {

            double freq, base, left, right, l, r, hue, saturation, luminance, basefreq, tonefreq, discrete, xcorrection;

            std::vector< std::vector<double> > signal;
            std::vector<double> L, R, phase;

            freq = 16.3516;
            base = pow(2, 1/12.0);

            std::mt19937 generator(8);
            std::uniform_real_distribution<double> distribution(-1.0,1.0);

            for (unsigned int i = 0; i < vector[0].size(); ++i) {
                phase.push_back(distribution(generator));
            }

            xcorrection = 8.0 / (static_cast<double>(vector[0].size()));

            for (unsigned int y = 0; y < vector.size(); ++y) {

                left = 0;
                right = 0;

                for (unsigned int x = 0; x < vector[0].size(); ++x) {

                    l = x / (static_cast<double>(vector[0].size()) - 1.0);
                    r = 1.0 - (x / (static_cast<double>(vector[0].size()) - 1.0));

                    l = l * xcorrection;
                    r = r * xcorrection;

                    hue        = vector[y][x][0];
                    saturation = vector[y][x][1];
                    luminance  = vector[y][x][2];

                    basefreq = pow(base, hue * 12.0) * freq;
                    tonefreq = pow(base, luminance * 84.0) * basefreq;

                    if(y == 0) {
                        phase.push_back(wtc::number::randomDouble(-1.0, 1.0));
                    }
                    discrete = number::curve(y, tonefreq, phase[x], saturation);

                    left = left + (discrete * l);
                    right = right + (discrete * r);
                }

                //            left = left * (4.0 / vector[0].size());
                //            right = right * (4.0 / vector[0].size());

                L.push_back(left);
                R.push_back(right);
            }

            signal.push_back(L);
            signal.push_back(R);

            return signal;
        }

        std::vector< std::vector<double> > baseVolToneA(std::vector< std::vector< std::vector<double> > > vector) {

            double freq, base, left, right, l, r, xcorrection, hue, saturation, luminance, basefreq, tonefreq, discreteL, discreteR;

            std::vector< std::vector<double> > signal;
            std::vector<double> L, R, phase;

            stk::SineWave sineL[vector[0].size()], sineR[vector[0].size()];

            freq = 16.3516;
            base = pow(2, 1/12.0);

            std::mt19937 generator(8);
            std::uniform_real_distribution<double> distribution(-1.0,1.0);

            for (unsigned int i = 0; i < vector[0].size(); ++i) {
                phase.push_back(distribution(generator));
            }

            xcorrection = 8.0 / (static_cast<double>(vector[0].size()));

            for (unsigned int y = 0; y < vector.size(); ++y) {

                left = 0;
                right = 0;

                // ycorrection = pow(y / (static_cast<double>(vector.size()) - 1.0), 0.125);

                for (unsigned int x = 0; x < vector[0].size(); ++x) {

                    l = x / (static_cast<double>(vector[0].size()) - 1.0);
                    r = 1.0 - (x / (static_cast<double>(vector[0].size()) - 1.0));

                    l = l * xcorrection;
                    r = r * xcorrection;

                    hue        = vector[y][x][0];
                    saturation = vector[y][x][1];
                    luminance  = vector[y][x][2];

                    basefreq = pow(base, hue * 12.0) * freq;
                    tonefreq = pow(base, luminance * 84.0) * basefreq;

                    sineL[x].setFrequency(tonefreq);
                    sineR[x].setFrequency(tonefreq);

                    if(y == 0) {
                        sineL[x].addPhase(phase[x]);
                        sineR[x].addPhase(phase[(vector[0].size() - 1) - x]);
                    }

                    discreteL = sineL[x].tick() * saturation;
                    discreteR = sineR[x].tick() * saturation;
                    // tone = (tone + (tone * correction)) / 2;
                    //                if(y == 0){
                    //                    std::cout << "tone: " << tone << '\n';
                    //                    std::cout << "l: " << l << '\n';
                    //                    std::cout << "r: " << r << '\n';
                    //                }
                    //
                    left = left + (discreteL * l);
                    right = right + (discreteR * r);
                }
//                if(y < 100 || y > vector.size() - 101){
//                    std::cout << "// ----- " << y << '\n';
//                    std::cout << "l: " << left << '\n';
//                    std::cout << "r: " << right << '\n';
//                }
                L.push_back(left);
                R.push_back(right);
            }

            signal.push_back(L);
            signal.push_back(R);

            return signal;
        }

        // std::vector< std::vector<double> > gradient(unsigned int width, unsigned int height, double rot, double off, double ang) {
        //
        //     double rotation, offset, rof, a, yAngle, xAngle, yValue, xValue;
        //
        //     off = wtc::number::circle(0, 1, off);
        //     ang = wtc::number::circle(0, 1, ang);
        //
        //     rotation = rot * 180;
        //     offset  = off * 360 + 90;
        //     ang = 1 - ang;
        //
        //     if(ang <= 0.25) {
        //         yAngle = 1 - ang * 4;
        //         xAngle = ang * 4;
        //         rof = 0;
        //
        //     } else if (ang <= 0.5) {
        //         a = ang - 0.25;
        //         yAngle = a * 4;
        //         xAngle = 1 - a * 4;
        //         rof = 0;
        //
        //     } else if (ang <= 0.75) {
        //         a = ang - 0.5;
        //         yAngle = 1 - a * 4;
        //         xAngle = a * 4;
        //         rof = 180;
        //
        //     } else {
        //         a = ang - 0.75;
        //         yAngle = a * 4;
        //         xAngle = 1 - a * 4;
        //         rof = 180;
        //
        //     }
        //
        //     std::vector< std::vector<double> > vector;
        //     for (unsigned int y = 0; y < height; ++y) {
        //         double h = static_cast<double>(height) - 1;
        //         double yv;
        //
        //         if(ang <= 0.25) {
        //             yv = y;          // √
        //         } else if (ang <= 0.5) {
        //             yv = h - y;
        //         } else if (ang <= 0.75) {
        //             yv = y;
        //         } else {
        //             yv = h - y;
        //         }
        //
        //         yValue = (yv / h * yAngle);
        //
        //         std::vector<double> temp;
        //         for (unsigned int x = 0; x < width; ++x) {
        //             double w = static_cast<double>(width) - 1;
        //
        //             xValue = (x / w * xAngle);
        //
        //             double degrees = ( ( xValue + yValue ) * rotation ) + offset + rof;
        //             double sinus = sin(wtc::number::radian(degrees));
        //
        //             temp.push_back(wtc::number::normDouble(-1, 1, sinus));
        //         }
        //         vector.push_back(temp);
        //     }
        //
        //     return vector;
        // }

        // std::vector< std::vector< unsigned char > > walker(unsigned int width, unsigned int height) {
        //
        //     unsigned int n;
        //     long int y, x;
        //
        //     y = wtc::number::randomInt(0, height - 1);
        //     x = wtc::number::randomInt(0, width - 1);
        //
        //     n = height * width;
        //
        //     std::vector< std::vector<unsigned char> > vector(height, std::vector<unsigned char>(width, 255));
        //
        //     for (unsigned int i = 0; i < n / 2; ++i) {
        //         if (i != 0){
        //             y = y + wtc::number::randomInt(-1,1);
        //             x = x + wtc::number::randomInt(-1,1);
        //         }
        //
        //         if(y > height - 1) y = round((height - 1) * 2 - y);
        //         if(y < 0) y *= -1;
        //
        //         if(x > width - 1) x = round((width - 1) * 2 - x);
        //         if(x < 0) x *= -1;
        //
        //         // std::cout << height << ": " << y << " " << width << ": " << x << '\n';
        //
        //         int value = vector[y][x] - 10;
        //         if(value < 0) value = 0;
        //
        //         vector[y][x] = value;
        //     }
        //
        //     return vector;
        // }

        void signalStrech(std::vector< std::vector<double> >& vector) {

            std::vector<double> rangeL, rangeR;
            rangeL = v1d::minMax(vector[0]);
            rangeR = v1d::minMax(vector[1]);

            double min, max;
            max = fabs(rangeL[0]);
            if(max < fabs(rangeL[1])) max = fabs(rangeL[1]);
            if(max < fabs(rangeR[0])) max = fabs(rangeR[0]);
            if(max < fabs(rangeR[1])) max = fabs(rangeR[1]);

            min = max * -1;

            for (unsigned int i = 0; i < vector[0].size(); ++i) {
                vector[0][i] = number::normSignal(min, max, vector[0][i]);
                vector[1][i] = number::normSignal(min, max, vector[1][i]);
            }

        }
    } // --------------------------------------------------------------------------- v2d ---- end

    namespace v3d { // ------------------------------------------------------------- v3d ----
        std::vector< std::vector< std::vector<double> > > fourier(std::vector< std::vector<double> >& signal) {

            unsigned int n = round(sqrt(signal[0].size() * sqrt(2)));
            unsigned int h = round(sqrt(signal[0].size() * (1 / sqrt(2))));

            std::vector< std::vector< std::vector<double> > > vector;
            std::vector< std::vector<double> > vectorL, vectorR;

            int c = 0;
            for (unsigned int y = 0; y < h; ++y) {

                std::vector<double> bandInL, bandInR;
                for (unsigned int i = 0; i < n; ++i) {

                    bandInL.push_back(signal[0][c]);
                    bandInR.push_back(signal[1][c]);

                    ++c;
                }

                vectorL.push_back(v1d::norm1d(v1d::fft(bandInL)));
                vectorR.push_back(v1d::norm1d(v1d::fft(bandInR)));
            }

            vector.push_back(vectorL);
            vector.push_back(vectorR);

            // norm3dDouble(vector);

            //        std::cout << "vector size: " << vector.size() << '\n';
            //        std::cout << "vector[0] size: " << vector[0].size() << '\n';
            //        std::cout << "vector[0][0] size: " << vector[0][0].size() << '\n' << '\n';
            //
            //        std::cout << "h: " << h << '\n';
            //        std::cout << "n: " << n << '\n';
            //        std::cout << "c: " << c << '\n' << '\n';

            return vector;
        }

        void norm3dDouble(std::vector< std::vector< std::vector<double> > >& vector) {

            std::vector<double> check, range;

            for (unsigned int i = 0; i < vector.size(); ++i) {
                for (unsigned int j = 0; j < vector[0].size(); ++j) {
                    for (unsigned int k = 0; k < vector[0][0].size(); ++k) {
                        check.push_back(vector[i][j][k]);
                    }
                }
            }
            range = v1d::minMax(check);
            std:: cout << "min: " << range[0] << "max: " << range[1] << '\n' << '\n';

            for (unsigned int i = 0; i < vector.size(); ++i) {
                for (unsigned int j = 0; j < vector[0].size(); ++j) {
                    for (unsigned int k = 0; k < vector[0][0].size(); ++k) {
                        vector[i][j][k] = number::normDouble(range[0], range[1], vector[i][j][k]);
                    }
                }
            }
        }
    } // --------------------------------------------------------------------------- v3d ---- end

    namespace color { // ----------------------------------------------------------- color ----

        // std::vector<double> LabRange() { // get Lab min, max
        //
        //     cimg_library::CImg<unsigned char> RGB(1,1,1,3);
        //     cimg_library::CImg<double> Lab(1,1,1,3);
        //
        //     double minL = 0;
        //     double mina = 0;
        //     double minb = 0;
        //     double maxL = 0;
        //     double maxa = 0;
        //     double maxb = 0;
        //
        //     for(int R = 0; R < 256; ++R) {
        //         for(int G = 0; G < 256; ++G) {
        //             for(int B = 0; B < 256; ++B) {
        //                 RGB(0,0,0,0) = R;
        //                 RGB(0,0,0,1) = G;
        //                 RGB(0,0,0,2) = B;
        //
        //                 Lab = RGB.get_RGBtoLab();
        //
        //                 double L = Lab(0,0,0,0);
        //                 double a = Lab(0,0,0,1);
        //                 double b = Lab(0,0,0,2);
        //
        //                 if(L < minL) minL = L;
        //                 if(L > maxL) maxL = L;
        //
        //                 if(a < mina) mina = a;
        //                 if(a > maxa) maxa = a;
        //
        //                 if(b < minb) minb = b;
        //                 if(b > maxb) maxb = b;
        //             }
        //         }
        //     }
        //
        //     std::vector<double> range;
        //
        //     range.push_back(minL);
        //     range.push_back(maxL);
        //     range.push_back(mina);
        //     range.push_back(maxa);
        //     range.push_back(minb);
        //     range.push_back(maxb);
        //
        //     return range;
        // }

        // std::vector<double> HslRange() { // get Lab min, max
        //
        //     cimg_library::CImg<unsigned char> RGB(1,1,1,3);
        //     cimg_library::CImg<double> Lab(1,1,1,3);
        //
        //     double minH = 0;
        //     double mins = 0;
        //     double minl = 0;
        //     double maxH = 0;
        //     double maxs = 0;
        //     double maxl = 0;
        //
        //     for(int R = 0; R < 256; ++R) {
        //         for(int G = 0; G < 256; ++G) {
        //             for(int B = 0; B < 256; ++B) {
        //                 RGB(0,0,0,0) = R;
        //                 RGB(0,0,0,1) = G;
        //                 RGB(0,0,0,2) = B;
        //
        //                 Lab = RGB.get_RGBtoHSL();
        //
        //                 double L = Lab(0,0,0,0);
        //                 double a = Lab(0,0,0,1);
        //                 double b = Lab(0,0,0,2);
        //
        //                 if(L < minH) minH = L;
        //                 if(L > maxH) maxH = L;
        //
        //                 if(a < mins) mins = a;
        //                 if(a > maxs) maxs = a;
        //
        //                 if(b < minl) minl = b;
        //                 if(b > maxl) maxl = b;
        //             }
        //         }
        //     }
        //
        //     std::vector<double> range;
        //
        //     range.push_back(minH);
        //     range.push_back(maxH);
        //     range.push_back(mins);
        //     range.push_back(maxs);
        //     range.push_back(minl);
        //     range.push_back(maxl);
        //
        //     return range;
        // }

        std::vector<double> rgbToHsl(unsigned char& R, unsigned char& G, unsigned char& B) { // R, G, B = 0 - 255 | H, s, l = 0.0 - 1.0

            double r = (static_cast<double>(R) / 255);
            double g = (static_cast<double>(G) / 255);
            double b = (static_cast<double>(B) / 255);

            std::vector<double> rgbDouble{r, g, b};

            std::vector<double> minmax = v1d::minMax(rgbDouble);

            double min = minmax[0];   // RGB min
            double max = minmax[1];   // RGB max

            double delta = max - min; // Delta RGB

            double h, s, l;

            h = 0.0;
            s = 0.0;
            l = (max + min) / 2;

            if(delta == 0) { // no Chromatic data
                h = 0.0;
                s = 0.0;
            } else {         // Chromatic data
                if(l < 0.5) {
                    s = delta / (max + min);
                } else {
                    s = delta / (2 - max - min);
                }

                double deltaR = ( ( (max - r) / 6 ) + (delta / 2) ) / delta;
                double deltaG = ( ( (max - g) / 6 ) + (delta / 2) ) / delta;
                double deltaB = ( ( (max - b) / 6 ) + (delta / 2) ) / delta;

                if( r == max ) {
                    h = deltaB - deltaG;
                } else if(g == max) {
                    h = (1 / 3) + deltaR - deltaB;
                } else if(b == max) {
                    h = ( 2 / 3 ) + deltaG - deltaR;
                }

                if ( h < 0 ) h += 1;
                if ( h > 1 ) h -= 1;
            }
            std::vector<double> hsl{h, s, l};
            return hsl;
        }

        std::vector<unsigned char> hslToRgb(std::vector<double> hsl) {  // H, s, l = 0.0 - 1.0 | R, G, B = 0 - 255
            unsigned char R, G, B;
            double a, b;

            auto hueToRgb = [](double v1, double v2, double vH) -> double { // useb by hslToRgb
                if(vH < 0) vH += 1;
                if(vH > 1) vH -= 1;

                if((6 * vH) < 1) return v1 + (v2 - v1) * 6 * vH;
                if((2 * vH) < 1) return v2;
                if((3 * vH) < 2) return v1 + (v2 - v1) * ((2 / 3) - vH) * 6;
                return v1;
            };

            if(hsl[1] == 0) { // no Chromatic data
                R = round(hsl[2] * 255.0);
                G = round(hsl[2] * 255.0);
                B = round(hsl[2] * 255.0);
            } else {         // Chromatic data
                if (hsl[2] < 0.5) {
                    b = hsl[2] * (1 + hsl[1]);
                } else {
                    b = (hsl[2] + hsl[1]) - (hsl[1] * hsl[2]);
                }

                a = 2 * hsl[2] - b;

                R = round(255.0 * hueToRgb(a, b, hsl[0] + (1 / 3)));
                G = round(255.0 * hueToRgb(a, b, hsl[0]));
                B = round(255.0 * hueToRgb(a, b, hsl[0] - (1 / 3)));
            }

            std::vector<unsigned char> rgb{R, G, B};
            return rgb;
        } // hslToRgb END
    } // --------------------------------------------------------------------------- color ---- end

}
