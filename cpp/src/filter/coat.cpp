
#include "coat.h"

Coat::Coat() {

  std::vector<std::string> type_options{"spectrum"};
  m_coat.push_back(data::init_str("type", type_options, m_type));

} // constructor END

nlohmann::json Coat::coat_data() {
  return m_coat;
}
