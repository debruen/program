
#include "data.h"

/// internal functions

std::size_t data::array_size(nlohmann::json& data) {
  std::size_t size = 0;
  if (data.size())
    size = data.size();
  return size;
}

// data string functions

nlohmann::json data::init_str(std::string name, std::vector<std::string> options, std::string value) {

  nlohmann::json data;

  data["name"] = name;
  data["form"] = "select";
  data["type"] = "string";
  data["options"] = options;
  data["value"] = value;

  return data;
}

nlohmann::json data::init_str(std::string name, std::string value) {

  nlohmann::json data;

  data["name"] = name;
  data["form"] = "value";
  data["type"] = "string";
  data["value"] = value;

  return data;
}

std::string data::get_str(nlohmann::json& data, std::string name) {

  std::size_t index = 0, size = array_size(data);

  for (std::size_t i = 0; i < size; i++) {
    if (data[i]["name"] == name) {
      index = i;
      break;
    }
  }

  std::string value = "";

  if(data[index]["type"] == "string")
    value = data[index]["value"];

  return value;
}

// data int functions

nlohmann::json data::init_int(std::string name, std::size_t min, std::size_t max, std::size_t value) {

  nlohmann::json data;

  data["name"] = name;
  data["form"] = "range";
  data["type"] = "int";
  data["min"] = min;
  data["max"] = max;
  data["value"] = value;

  return data;
}

std::size_t data::get_int(nlohmann::json& data, std::string name) {

  nlohmann::json select;

  std::size_t size = array_size(data);

  for (std::size_t i = 0; i < size; i++) {
    if (data[i]["name"] == name) {
      select = data[i];
      break;
    }
  }

  std::size_t value{0};

  if(select["type"] == "int" || select["type"] == "time") value = select["value"];

  return value;
}
// data time functions

nlohmann::json data::init_time(std::string name, std::size_t min, std::size_t max, std::size_t value) {

  nlohmann::json data;

  data["name"] = name;
  data["form"] = "range";
  data["type"] = "time";
  data["min"] = min;
  data["max"] = max;
  data["value"] = value;

  return data;
}

// data float

nlohmann::json data::init_float(std::string name, double min, double max, double value) {

  nlohmann::json data;

  data["name"] = name;
  data["form"] = "range";
  data["type"] = "float";
  data["min"] = min;
  data["max"] = max;
  data["value"] = value;

  return data;
}

double data::get_float(nlohmann::json& data, std::string name) {

  nlohmann::json select;

  std::size_t size = array_size(data);

  for (std::size_t i = 0; i < size; i++) {
    if (data[i]["name"] == name) {
      select = data[i];
      break;
    }
  }

  double value = 0.0;

  if(select["type"] == "float") value = select["value"];

  return value;
}

nlohmann::json data::init_bool(std::string name, bool value) {

  nlohmann::json data;

  data["name"] = name;
  data["form"] = "bool";
  data["type"] = "bool";
  data["value"] = value;

  return data;
}

bool data::get_bool(nlohmann::json& data, std::string name) {

  nlohmann::json select;

  std::size_t size = array_size(data);

  for (std::size_t i = 0; i < size; i++) {
    if (data[i]["name"] == name) {
      select = data[i];
      break;
    }
  }

  bool value = false;

  if(select["type"] == "bool") value = select["value"];

  return value;
}

nlohmann::json data::init_button(std::string name, bool value) {

  nlohmann::json data;

  data["name"] = name;
  data["form"] = "button";
  // data["select"] = select;
  data["type"] = "bool";
  data["value"] = value;

  return data;
} // data_bool END

nlohmann::json data::init_value(std::string name, double value) {

  nlohmann::json data;

  data["name"]  = name;
  data["form"]  = "data";
  data["type"]  = "value";
  data["value"] = value;

  return data;
}

double data::get_value(nlohmann::json& data, std::string name) {

  nlohmann::json select;

  std::size_t size = array_size(data);

  for (std::size_t i = 0; i < size; i++) {
    if (data[i]["name"] == name) {
      select = data[i];
      break;
    }
  }

  double value = 0.0;

  if(select["type"] == "float") value = select["value"];

  return value;
}

nlohmann::json data::init_min_max(std::string name, double min, double max) {

  nlohmann::json data;

  data["name"] = name;
  data["form"] = "data";
  data["type"] = "data";
  data["min"] = min;
  data["max"] = max;

  return data;
}

// json data functions

nlohmann::json data::init_1d_float_data(std::string name, std::vector<double> values) {

  nlohmann::json data;

  data["name"] = name;
  data["form"] = "data";
  data["values"] = values;

  return data;
}

nlohmann::json data::init_2d_uchar_data(std::string name, std::vector< std::vector<unsigned char> > values) {

  nlohmann::json data;

  data["name"] = name;
  data["form"] = "data";
  data["values"] = values;

  return data;
}
