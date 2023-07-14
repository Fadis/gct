#include <gct/device.hpp>
#include <gct/shader_module.hpp>
#include <gct/allocator.hpp>
#include <gct/dnn/linear.hpp>
#include <gct/dnn/convolution.hpp>
#include <gct/dnn/nnef.hpp>

namespace gct::dnn {

const nnef_data_t &get_buffer(
  const nnef::Operation &op,
  const std::unordered_map< std::string, nnef_data_t > &bufs,
  const std::string &identifier
) {
  const auto input = std::find_if(
    op.inputs.begin(), op.inputs.end(),
    [&]( const auto &v ) {
      return v.first == identifier;
    }
  );
  if( input == op.inputs.end() ) {
    throw -1;
  }
  const auto input_buf = bufs.find( input->second.identifier() );
  if( input_buf == bufs.end() ) {
    throw -1;
  }
  return input_buf->second;
}

const nnef_data_t &get_buffer_maybe(
  const nnef::Operation &op,
  const std::unordered_map< std::string, nnef_data_t > &bufs,
  const std::string &identifier
) {
  static const nnef_data_t null_buf;
  const auto input = std::find_if(
    op.inputs.begin(), op.inputs.end(),
    [&]( const auto &v ) {
      return v.first == identifier;
    }
  );
  if( input == op.inputs.end() ) {
    return null_buf;
  }
  if( input->second.kind() != nnef::Value::Kind::Identifier ) {
    return null_buf;
  }
  const auto input_buf = bufs.find( input->second.identifier() );
  if( input_buf == bufs.end() ) {
    return null_buf;
  }
  return input_buf->second;
}

std::string numeric_type_to_suffix( const numeric_type_t &type ) {
  if( type.component == numeric_component_type_t::bool_ ) {
    return "b8";
  }
  else if( type.component == numeric_component_type_t::int_ ) {
    if( type.attr == integer_attribute_t::asis ) {
      if( type.sign ) {
        return "s" + std::to_string( type.depth );
      }
      else {
        return "u" + std::to_string( type.depth );
      }
    }
    else if( type.attr == integer_attribute_t::normalized ) {
      if( type.sign ) {
        return "p" + std::to_string( type.depth );
      }
      else {
        return "q" + std::to_string( type.depth );
      }
    }
    else if( type.attr == integer_attribute_t::scaled ) {
      if( type.sign ) {
        return "s" + std::to_string( type.depth );
      }
      else {
        return "t" + std::to_string( type.depth );
      }
    }
  }
  else if( type.component == numeric_component_type_t::float_ ) {
    return "f" + std::to_string( type.depth );
  }
  throw -1;
}

std::unordered_map< std::string, std::shared_ptr< shader_module_t > > load_shaders(
  const std::shared_ptr< device_t > &device,
  const std::filesystem::path &dir
) {
  if( !std::filesystem::is_directory( dir ) ) {
    throw -1;
  }
  std::unordered_map< std::string, std::shared_ptr< shader_module_t > > shaders;
  for( const auto &d: std::filesystem::directory_iterator( dir ) ) {
    if( std::filesystem::is_regular_file( d.path() ) ) {
      const auto filename = d.path().filename().string();
      if( filename.size() >= 9u && filename.find( ".comp.spv" ) == filename.size() - 9u ) {
        auto shader = device->get_shader_module(
          std::filesystem::absolute( d.path() ).string()
        );
        shaders.insert(
          std::make_pair(
            filename.substr( 0, filename.size() - 9u ),
            shader
          )
        );
      }
    }
  }
  return shaders;
}


}

