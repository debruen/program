
#ifndef record_h
#define record_h

#include <thread>
#include <mutex>

#include <fstream>
#include <sys/stat.h>

#include "stk/FileWvOut.h"

#include "../functionality/data.h"

class Record {

  private:

    std::vector<frame>& m_buffer;
    std::mutex& m_buffer_mutex;

    info& m_info;
    std::mutex& m_info_mutex;

    std::string m_path, m_name, m_image, m_audio;

    // bool m_quit = false;
    //
    // void thread();
    // std::thread m_thread;

    // bool m_recording{false}, m_saved{false};

    std::size_t m_frames{1};

    stk::FileWvOut audio_out;

    // std::size_t m_prev{0};
    //
    // std::size_t m_count{0};

    bool exists(const std::string& file);
    std::string file(std::string& filename, std::string& path, std::string type);

    bool frame_exists(std::size_t& frame_index);

    void save_image(cv::Mat& image, std::size_t frame_index);
    void save_audio(cv::Mat& audio);

    void start();

    bool stop();

    void save(std::size_t frame_index);

  public:
    Record(std::vector<frame>& buffer, std::mutex& buffer_mutex, info& info, std::mutex& info_mutex);

    void init(nlohmann::json& data);

    // void data(nlohmann::json& data);

    bool record();

    // bool quit();
};

#endif /* record_h */
