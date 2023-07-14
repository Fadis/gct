#include <gct/compute_chain.hpp>

namespace gct {
  class device_t;
  class allocator_t;
  class buffer_t;
  class image_view_t;
  class descriptor_set_t;
  class compute_pipeline_t;
  class compute_chain : public created_from< device_t >, public std::enable_shared_from_this< compute_chain > {
    struct pass_resources {
      std::shared_ptr< descriptor_set_t > descriptor_set;
      std::shared_ptr< compute_pipeline_t > pipeline;
    };
  public:
    compute_chain(
      const std::shared_ptr< pipeine_cache_t > &pipeline_cache,
      const std::shared_ptr< allocator_t > &allocator,
      const std::filesystem::path &config_path
    ) {
      if( !std::filesystem::exists( config_path ) ) {
        throw gct::exception::invalid_argument( "compute_chain : file not found", __FILE__, __LINE__ );
      }
      if( !std::filesystem::is_regular_file( config_path ) ) {
        throw gct::exception::invalid_argument( "compute_chain : not a regular file", __FILE__, __LINE__ );
      }
      std::ifstream fd( config_path.string() );
      if( fd.fail() ) {
        throw gct::exception::invalid_argument( "compute_chain : unable to open the file", __FILE__, __LINE__ );
      }
      config = nlohmann::json::parse( fd );
      if( !config.is_object() ) {
        throw gct::exception::invalid_argument( "compute_chain : root element must be an object", __FILE__, __LINE__ );
      }
      if( !config.find( "pass" ) == config.end() ) {
        throw gct::exception::invalid_argument( "compute_chain : required value \"pass\" is not set", __FILE__, __LINE__ );
      }
      if( !config.find( "buffer" ) == config.end() ) {
        if( !config[ "buffer" ].is_array() ) {
          throw gct::exception::invalid_argument( "compute_chain : buffer must be an array", __FILE__, __LINE__ );
        }
        for( const auto &b: config[ "buffer" ] ) {
          if( !b.is_object() ) {
            throw gct::exception::invalid_argument( "compute_chain : buffer element must be an object", __FILE__, __LINE__ );
          }
          if( b.find( "name" ) == b.end() ) {
            throw gct::exception::invalid_argument( "compute_chain : buffer without name is not allowed", __FILE__, __LINE__ );
          }
          if( !b[ "name" ].is_string() ) {
            throw gct::exception::invalid_argument( "compute_chain : buffer name must be a string", __FILE__, __LINE__ );
          }
          if( b.find( "size" ) == b.end() ) {
            throw gct::exception::invalid_argument( "compute_chain : buffer without size is not allowed", __FILE__, __LINE__ );
          }
          if( !b[ "size" ].is_number() ) {
            throw gct::exception::invalid_argument( "compute_chain : buffer size must be a number", __FILE__, __LINE__ );
          }
          if( b[ "size" ] <= 0 ) {
            throw gct::exception::invalid_argument( "compute_chain : buffer size must higher than zero", __FILE__, __LINE__ );
          }
          if( b.find( "data" ) == b.end() ) {
            
          }
          else {
          }
        }
      }
    }
    const std::unordered_map< std::string, std::shared_ptr< buffer_t > > &get_buffer() const {
      return buffer;
    }
    const std::unordered_map< std::string, std::shared_ptr< image_view_t > > &get_image() const {
      return image;
    }
    const nlohmann::json &get_config() const {
      return config;
    }
    void run( command_buffer_recorder_t& ) const;
  private:
    nlohmann::json config;
    std::unordered_map< std::string, std::shared_ptr< buffer_t > > buffer;
    std::unordered_map< std::string, std::shared_ptr< image_view_t > > image;
    std::vector < pass_resources > pass;
    bound_command_buffer_t command;
  }
  void to_json( nlohmann::json &root, const compute_chain &v );
}

#endif


