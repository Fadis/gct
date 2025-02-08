#ifndef LIBGCT_INCLUDE_GCT_REMOVE_CHAIN_HPP
#define LIBGCT_INCLUDE_GCT_REMOVE_CHAIN_HPP

#include <nlohmann/json.hpp>
namespace gct {
[[nodiscard]] nlohmann::json remove_chain( const nlohmann::json &src );
}

#endif

