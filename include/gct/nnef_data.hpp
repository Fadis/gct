#ifndef GCT_NNEF_DATA_HPP
#define GCT_NNEF_DATA_HPP
#include <cstdint>
#include <vector>
#include <memory>
#include <nlohmann/json_fwd.hpp>
#include <gct/numeric_types.hpp>

namespace gct {
class buffer_t;
struct nnef_data_t {
  numeric_type_t type;
  std::vector< std::uint32_t > dim;
  std::shared_ptr< buffer_t > buffer;
};

void to_json( nlohmann::json &, const nnef_data_t& );

}

#endif

