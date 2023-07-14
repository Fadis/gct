#ifndef GCT_DNN_SYNSET_MAPPING_HPP
#define GCT_DNN_SYNSET_MAPPING_HPP
#include <vector>
#include <string>
#include <filesystem>

namespace gct::dnn {

std::vector< std::string > load_synset_mapping(
  const std::filesystem::path &filename
);

}

#endif

