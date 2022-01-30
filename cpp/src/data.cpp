
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

// -------------
double data::normalize(double min, double max, double value) {
  return (value - min) / (max - min);
}

double data::project(double min, double max, double value) {
    return (value * (max - min)) + min;
}

nlohmann::json data::data_int(std::string name, int min, int max, int value, bool select) {

  nlohmann::json data;

  data["name"] = name;
  data["form"] = "range";
  data["select"] = select;
  data["type"] = "int";
  data["min"] = min;
  data["max"] = max;
  data["value"] = value;

  return data;
}

nlohmann::json data::data_time(std::string name, int min, int max, int value, bool select) {

  nlohmann::json data;

  data["name"] = name;
  data["form"] = "range";
  data["select"] = select;
  data["type"] = "time";
  data["min"] = min;
  data["max"] = max;
  data["value"] = value;

  return data;
}

nlohmann::json data::data_float(std::string name, double min, double max, double value, bool select) {

  nlohmann::json data;

  data["name"] = name;
  data["form"] = "range";
  data["select"] = select;
  data["type"] = "float";
  data["min"] = min;
  data["max"] = max;
  data["value"] = value;

  return data;
}

nlohmann::json data::data_value(std::string name, double value) {

  nlohmann::json data;

  data["name"]  = name;
  data["form"]  = "data";
  data["type"]  = "value";
  data["value"] = value;

  return data;
}

nlohmann::json data::data_path(std::string name, std::string value) {

  nlohmann::json data;

  data["name"]  = name;
  data["form"]  = "data";
  data["type"]  = "string";
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

nlohmann::json data::get_data(nlohmann::json& data, std::string name) {

  nlohmann::json select;

  std::size_t size = array_size(data);

  for (std::size_t i = 0; i < size; i++) {
    if (data[i]["name"] == name) {
      select = data[i];
      break;
    }
  }

  return select;
}

nlohmann::json data::get_array(nlohmann::json& data, std::string name) {

  nlohmann::json select;

  std::size_t size = array_size(data);

  for (std::size_t i = 0; i < size; i++) {
    if (data[i]["name"] == name) {
      select = data[i];
      break;
    }
  }

  nlohmann::json value = nlohmann::json::array();

  if(select["type"] == "array") value = select["options"];

  return value;
}

void data::set_array(nlohmann::json& data, std::string name, nlohmann::json array) {

  std::size_t size = array_size(data);

  for (std::size_t i = 0; i < size; i++) {
    if (data[i]["name"] == name) {
      data[i]["options"] = array;
      break;
    }
  }

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

std::string data::get_string(nlohmann::json& data, std::string name) {

  std::size_t index = 0, size = array_size(data);

  for (std::size_t i = 0; i < size; i++) {
    if (data[i]["name"] == name) {
      index = i;
      break;
    }
  }

  // std::string value = "";
  //
  // if(data[index]["type"] == "string") value = data[index]["value"];

  return data[index]["value"];
}

unsigned int data::get_width(nlohmann::json& data, std::string name) {

  std::size_t index{0}, size = array_size(data);

  for (std::size_t i = 0; i < size; i++) {
    if (data[i]["name"] == name) {
      index = i;
      break;
    }
  }

  return data[index]["width"];
}

unsigned int data::get_height(nlohmann::json& data, std::string name) {

  std::size_t index{0}, size = array_size(data);

  for (std::size_t i = 0; i < size; i++) {
    if (data[i]["name"] == name) {
      index = i;
      break;
    }
  }

  return data[index]["height"];
}

void data::compute_size(nlohmann::json& data, std::string name, std::string area, double ratio) {

  std::size_t index{0}, size = array_size(data);

  for (std::size_t i = 0; i < size; i++) {
    if (data[i]["name"] == name) {
      index = i;
      break;
    }
  }

  // base area = A4 @ 300 dpi
  int width, height, base_area{2480 * 3508};

  if (area == "A6") base_area = base_area / 4;
  if (area == "A5") base_area = base_area / 2;
  // if (area == "A4") base_area = base_area;
  if (area == "A3") base_area = base_area * 2;
  if (area == "A2") base_area = base_area * 4;

  height = round(sqrt(base_area / ratio));
  width = round(height * ratio);

  data[index]["width"] = width;
  data[index]["height"] = height;

} // data_int END

nlohmann::json data::data_object(std::string name, std::string type, std::vector<std::string> options, std::string value, bool select) {

  nlohmann::json data;

  data["name"] = name;
  data["form"] = "object";
  data["select"] = select;
  data["type"] = type;
  data["options"] = options;
  data["value"] = value;
  data["objects"] = nlohmann::json::array();

  return data;
} // data_mask END

nlohmann::json data::data_array(std::string name, bool select) {

  nlohmann::json data;

  data["name"] = name;
  data["form"] = "array";
  data["select"] = select;
  data["type"] = "array";
  data["options"] = nlohmann::json::array();

  return data;
} // data_array END

nlohmann::json data::data_bool(std::string name, bool select, bool value) {

  nlohmann::json data;

  data["name"] = name;
  data["form"] = "bool";
  data["select"] = select;
  data["type"] = "bool";
  data["value"] = value;

  return data;
} // data_bool END

nlohmann::json data::data_button(std::string name, bool select, bool value) {

  nlohmann::json data;

  data["name"] = name;
  data["form"] = "button";
  data["select"] = select;
  data["type"] = "bool";
  data["value"] = value;

  return data;
} // data_bool END

nlohmann::json data::data_string(std::string name, bool select, std::vector<std::string> options, std::string value) {

  nlohmann::json data;

  if(value == "") value = name;

  data["name"] = name;
  data["form"] = "select";
  data["select"] = select;
  data["type"] = "string";
  data["options"] = options;
  data["value"] = value;

  return data;
} // data_string END

nlohmann::json data::data_size(std::string name, bool select, int width, int height) {

  nlohmann::json data;

  data["name"] = name;
  data["form"] = "size";
  data["select"] = select;
  data["type"] = "size";
  data["width"] = width;
  data["height"] = height;

  return data;
} // data_size END

nlohmann::json data::data_mask(std::string name, std::string type, bool select) {

  nlohmann::json data;

  data["name"] = name;
  data["form"] = "mask";
  data["select"] = select;
  data["type"] = type;
  data["options"] = nlohmann::json::array();

  return data;
} // data_mask END


nlohmann::json data::data_data(std::string name, double min, double max) {

  nlohmann::json data;

  data["name"] = name;
  data["form"] = "data";
  data["type"] = "data";
  data["min"] = min;
  data["max"] = max;

  return data;
}

nlohmann::json data::data_sine(std::string name, bool select) {

  nlohmann::json data;

  data["name"] = name;
  data["form"] = "sine";
  data["select"] = select;
  data["type"] = "sine";
  data["options"] = nlohmann::json::array();

  data["options"].push_back(data_float("frequency", 16, 21000, 440));
  data["options"].push_back(data_float("amplitude", 0, 1, 0.5));
  data["options"].push_back(data_float("phase", -1, +1, 0.0));

  return data;
} // data_sine END
