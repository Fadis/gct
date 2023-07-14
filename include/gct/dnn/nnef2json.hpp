#include <nnef.h>
#include <nlohmann/json_fwd.hpp>

namespace nnef {

void to_json( nlohmann::json &dest, const nnef::Value &src );
void to_json( nlohmann::json &dest, const nnef::ValueDict &src );
void to_json( nlohmann::json &dest, const nnef::Tensor &src );
void to_json( nlohmann::json &dest, const nnef::Operation &src );
void to_json( nlohmann::json &dest, const nnef::Graph &src );

}

