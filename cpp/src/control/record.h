
#ifndef record_h
#define record_h

#include <thread>
#include <mutex>

#include <fstream>
#include <sys/stat.h>

#include "stk/FileWvOut.h"

#include "../data.h"

class Record {

  private:

    std::vector<frame>& m_buffer;
    std::mutex& m_buffer_mutex;


    std::string m_path, m_name, m_image, m_audio;


    bool m_thread = true;

    void recorder();
    std::thread m_recorder_thread;

    bool m_recording{false};
    std::size_t m_frames{1};

    stk::FileWvOut audio_out;

    std::size_t m_prev{0};

    std::size_t m_count{0};

    bool exists(const std::string& file);
    std::string file(std::string& filename, std::string& path, std::string type);

    void save_image(cv::Mat& image, std::size_t frame_index);
    void save_audio(cv::Mat& audio);

    void start();

    bool stop();

    void save(std::size_t frame_index);

  public:
    Record(std::vector<frame>& buffer, std::mutex& mutex);

    void record(std::size_t frames);

    bool status();
};

#endif /* record_h */
