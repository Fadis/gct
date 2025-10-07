#ifndef GCT_PUSH_CONSTANT_STORAGE_HPP
#define GCT_PUSH_CONSTANT_STORAGE_HPP
#include <unordered_set>
#include <string>
#include <optional>
#include <vector>
#include <nlohmann/json_fwd.hpp>
#include <gct/exception.hpp>
#include <gct/compute_create_info.hpp>
#include <gct/property.hpp>
#include <gct/spv_member_pointer.hpp>
#include <gct/command_buffer_recorder.hpp>

namespace gct {

class push_constant_storage {
public:
  [[nodiscard]] const std::optional< spv_member_pointer > &get_push_constant_member_pointer() const {
    return push_constant_mp;
  }
  [[nodiscard]] std::vector< std::uint8_t > &get_push_constant() const {
    if( push_constant_mp ) {
      use_internal_push_constant = true;
      push_constant.resize( push_constant_mp->get_aligned_size(), 0u );
    }
    assigned_push_constant = std::nullopt;
    return push_constant;
  }
  template< typename T >
  void operator()(
    command_buffer_recorder_t &rec,
    T &pipeline
  ) const {
    if( push_constant_enabled() ) {
      if( !push_constant_storage_is_ready() ) {
        throw exception::runtime_error( "push_constant_storage::operator() : One or more push constants are not set", __FILE__, __LINE__ );
      }
      rec->pushConstants(
        **pipeline->get_props().get_layout(),
        pipeline->get_props().get_layout()->get_props().get_push_constant_range()[ 0 ].stageFlags,
        get_push_constant_member_pointer()->get_offset(),
        get_push_constant_internal().size(),
        get_push_constant_internal().data()
      );
    }
  }
protected:
  [[nodiscard]] std::vector< std::uint8_t > &get_push_constant_internal() const {
    return push_constant;
  }
  void enable_push_constant(
    const spv_member_pointer &mp
  ) {
    push_constant_mp = mp;
    assigned_push_constant = std::unordered_set< std::string >{};
  }
  void disable_push_constant(
    const spv_member_pointer &mp
  ) {
    push_constant_mp = std::nullopt;
    assigned_push_constant = std::nullopt;
    push_constant.clear();
    use_internal_push_constant = false;
  }
  bool push_constant_enabled() const {
    return bool( push_constant_mp ) && !push_constant.empty();
  }
  bool push_constant_storage_is_ready() const {
    return use_internal_push_constant && ( assigned_push_constant ? ( assigned_push_constant->size() == push_constant_mp->get_member_count() ) : true );
  }
  template< typename T >
  void set_push_constant(
    const std::string &name,
    const T &value
  ) const {
    auto pcmp = get_push_constant_member_pointer();
    if( !pcmp ) {
      throw exception::runtime_error( "push_constant_storage::set_push_constant : Push constant member pointer is not available", __FILE__, __LINE__ );
    }
    if( !pcmp->has( name ) ) {
      throw exception::invalid_argument( "push_constant_storage::set_push_constant : Push constant variable " + name + " does not exist" , __FILE__, __LINE__ );
    }
    if( assigned_push_constant ) {
      assigned_push_constant->insert( name );
    }
    if( push_constant_mp && !use_internal_push_constant ) {
      use_internal_push_constant = true;
      push_constant.resize( push_constant_mp->get_aligned_size(), 0u );
    }
    push_constant.data()->*((*pcmp)[ name ]) = value;
  }
private:
  mutable bool use_internal_push_constant = false;
  mutable std::vector< std::uint8_t > push_constant;
  std::optional< spv_member_pointer > push_constant_mp;
  mutable std::optional< std::unordered_set< std::string > > assigned_push_constant;
};

}

#endif

