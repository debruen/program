
#ifndef new_h
#define new_h

class Color {

  private:

    nlohmann::json m_data;

  public:
    Color();

    nlohmann::json data();

    nlohmann::json data(nlohmann::json data, std::string type);

    std::vector<unsigned char> color(nlohmann::json data, std::string name = "color");

};

#endif /* new_h */
