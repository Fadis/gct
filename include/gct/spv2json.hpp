#ifndef GCT_SPV2JSON_HPP
#define GCT_SPV2JSON_HPP
#include <nlohmann/json_fwd.hpp>
#include <gct/spirv_reflect.h>

void to_json( nlohmann::json &root, const SpvReflectGenerator &v );
void to_json( nlohmann::json &root, const SpvExecutionModel &v );
void to_json( nlohmann::json &root, const SpvReflectShaderStageFlagBits &v );
void to_json( nlohmann::json &root, const SpvStorageClass &v );
void to_json( nlohmann::json &root, const SpvBuiltIn &v );
void to_json( nlohmann::json &root, const SpvReflectNumericTraits &v );
void to_json( nlohmann::json &root, const SpvReflectArrayTraits &v );
void to_json( nlohmann::json &root, const SpvReflectFormat &v );
void to_json( nlohmann::json &root, const SpvReflectInterfaceVariable &v );
void to_json( nlohmann::json &root, const SpvReflectDescriptorSet &v );
void to_json( nlohmann::json &root, const SpvReflectEntryPoint &v );
void to_json( nlohmann::json &root, const SpvSourceLanguage &v );
void to_json( nlohmann::json &root, const SpvReflectDescriptorType &v );
void to_json( nlohmann::json &root, const SpvReflectResourceType &v );
void to_json( nlohmann::json &root, const SpvImageFormat &v );
void to_json( nlohmann::json &root, const SpvReflectImageTraits &v );
void to_json( nlohmann::json &root, const SpvOp &v );
void to_json( nlohmann::json &root, const SpvReflectTypeDescription &v );
void to_json( nlohmann::json &root, const SpvReflectBlockVariable &v );
void to_json( nlohmann::json &root, const SpvReflectDescriptorBinding &v );
void to_json( nlohmann::json &root, const SpvReflectShaderModule &v );
void to_json( nlohmann::json &root, const SpvReflectBindingArrayTraits &v );

#endif

