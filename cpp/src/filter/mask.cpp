
#include "mask.h"

/// Mask type: sine, distribution

Mask::Mask() {

  std::vector<std::string> type_options{"gradient"};
  m_mask.push_back(data::init_str("type", type_options, m_type));

}

nlohmann::json Mask::get_data() {
  return m_mask;
}
