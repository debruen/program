
#ifndef record_h
#define record_h

#include <fstream>
#include <sys/stat.h>

#include "stk/FileWvOut.h"

#include "data.h"

class Record {

  private:

    std::vector<frame>& m_buffer;

    std::string m_path, m_name, m_image, m_audio;

    stk::FileWvOut audio_out;

    std::size_t m_prev;

    bool exists(const std::string& file);
    std::string file(std::string& filename, std::string& path, std::string type);

    void save_image(cv::Mat& image, std::size_t frame_index);
    void save_audio(cv::Mat& audio);

  public:
    Record(std::vector<frame>& buffer);

    bool start();

    bool stop();

    void save(std::size_t frame_index);

};

#endif /* record_h */
