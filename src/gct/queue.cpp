#include <algorithm>
#include <numeric>
#include <vector>
#include <cstdint>
#include <functional>
#include <gct/queue.hpp>
#include <gct/surface.hpp>
#include <gct/command_pool.hpp>
#include <gct/present_info.hpp>
#include <gct/physical_device_properties.hpp>
#include <gct/to_json.hpp>
#include <vulkan2json/QueueFlags.hpp>
#include <vulkan2json/Extent3D.hpp>
#include <vulkan2json/QueueGlobalPriorityEXT.hpp>

namespace gct {

  void to_json( nlohmann::json &root, const queue_requirement_t &v ) {
    root = nlohmann::json::object();
    root[ "flags" ] = v.flags;
    root[ "timestamp_valid_bits" ] = v.timestamp_valid_bits;
    root[ "image_transfer_granularity" ] = v.image_transfer_granularity;
#ifdef VK_EXT_GLOBAL_PRIORITY_EXTENSION_NAME
    if( v.global_priority ) {
      root[ "global_priority" ] = *v.global_priority;
    }
#endif
  }
  
  void to_json( nlohmann::json &root, const activated_queue_family_count_t &v ) {
    root = nlohmann::json::object();
    root[ "available_queue_family_index" ] = v.available_queue_family_index;
    root[ "activated_queue_family_index" ] = v.activated_queue_family_index;
#ifdef VK_EXT_GLOBAL_PRIORITY_EXTENSION_NAME
    if( v.global_priority ) {
      root[ "global_priority" ] = *v.global_priority;
    }
#endif
    root[ "count" ] = v.count;
  }
  
  void to_json( nlohmann::json &root, const activated_queue_mapping_t &v ) {
    root = nlohmann::json::object();
    root[ "req" ] = v.req;
    root[ "order" ] = v.order;
    root[ "rarity" ] = v.rarity;
    root[ "available_queue_family_index" ] = v.available_queue_family_index;
    root[ "activated_queue_family_index" ] = v.activated_queue_family_index;
    root[ "queue_count_index" ] = v.queue_count_index;
  }
  
  bool is_capable(
    const vk::PhysicalDevice &pdev,
    const queue_family_properties_t &qf,
    const queue_requirement_t &req,
    std::uint32_t family_index
  ) {
    if( ( qf.get_basic().queueFlags & req.flags ) != req.flags ) return false;
    if( ( qf.get_basic().timestampValidBits & req.timestamp_valid_bits ) != req.timestamp_valid_bits ) return false;
    if( req.image_transfer_granularity.width && qf.get_basic().minImageTransferGranularity.width > req.image_transfer_granularity.width ) return false;
    if( req.image_transfer_granularity.height && qf.get_basic().minImageTransferGranularity.height > req.image_transfer_granularity.height ) return false;
    if( req.image_transfer_granularity.depth && qf.get_basic().minImageTransferGranularity.depth > req.image_transfer_granularity.depth ) return false;
    for( auto &s: req.surface )
      if( !pdev.getSurfaceSupportKHR( family_index, s ) ) return false;
    return true;
  }


  std::tuple<
    activated_queue_mappings_t,
    activated_queue_family_counts_t
  >
  generate_queue_map(
    const vk::PhysicalDevice &pdev,
    const physical_device_properties_t &props,
    const queue_requirements_t &reqs_
  ) {
    available_queue_family_counts_t left;
    {
      available_queue_family_index_t index = 0u;
      std::transform(
        props.get_queue_family().begin(),
        props.get_queue_family().end(),
        std::inserter( left, left.end() ),
        [&]( const queue_family_properties_t &p ) {
          return std::make_pair( index++, p.get_basic().queueCount );
        }
      );
    }
    activated_queue_mappings_t activated_queue_mappings;
    {
      logical_queue_index_t index = 0u;
      std::transform(
        reqs_.begin(),
        reqs_.end(),
        std::back_inserter( activated_queue_mappings ),
        [&]( const queue_requirement_t &r ) {
          available_queue_family_index_t family_index = 0u;
          const auto count = std::accumulate(
            props.get_queue_family().begin(),
            props.get_queue_family().end(),
            std::uint32_t( 0 ),
            [&]( auto sum, const auto &qf ) {
              if( !is_capable( pdev, qf, r, family_index++ ) ) return sum;
              return sum + qf.get_basic().queueCount;
            }
          );
          return activated_queue_mapping_t{
            r,
            index++,
            count,
            0u,
            0u
          };
        }
      );
    }
    std::sort(
      activated_queue_mappings.begin(),
      activated_queue_mappings.end(),
      []( const auto &l, const auto &r ) {
        return ( l.rarity == r.rarity && l.order ) || l.rarity < r.rarity;
      }
    );
    const bool use_priority = props.get_activated_extensions().find( "VK_EXT_global_priority" ) != props.get_activated_extensions().end();
    activated_queue_family_counts_t activated_queue_family_counts;
    for( auto &mapping: activated_queue_mappings ) {
      bool mapped = false;
      for(
        std::uint32_t available_queue_family_index = 0u;
        available_queue_family_index != props.get_queue_family().size();
        ++available_queue_family_index
      ) {
        if( left[ available_queue_family_index ] != 0u ) {
          auto &available_queue_family = props.get_queue_family()[ available_queue_family_index ];
          if( is_capable( pdev, available_queue_family, mapping.req, available_queue_family_index ) ) {
            const auto existing = std::find_if(
              activated_queue_family_counts.begin(),
              activated_queue_family_counts.end(),
              [&]( const auto &v ) {
                return
                  v.available_queue_family_index == available_queue_family_index &&
#ifdef VK_EXT_GLOBAL_PRIORITY_EXTENSION_NAME
                  ( ( use_priority && mapping.req.global_priority != vk::QueueGlobalPriorityEXT() ) ? v.global_priority == mapping.req.global_priority : true ) &&
#endif
                  v.command_pool_create_flags == mapping.req.command_pool_create_flags;
              }
            );
            std::uint32_t activated_queue_family_index = 0u;
            std::uint32_t queue_count_index = 0u;
            if( existing == activated_queue_family_counts.end() ) {
              activated_queue_family_index = activated_queue_family_counts.size();
              queue_count_index = 0u;
              activated_queue_family_counts.push_back(
                activated_queue_family_count_t{
                  available_queue_family_index,
                  activated_queue_family_index,
#ifdef VK_EXT_GLOBAL_PRIORITY_EXTENSION_NAME
                  use_priority ? mapping.req.global_priority : std::nullopt,
#endif
                  1u,
                  std::vector< float >(),
                  mapping.req.command_pool_create_flags
                }
              );
            }
            else {
              activated_queue_family_index = std::distance( activated_queue_family_counts.begin(), existing );
              queue_count_index = existing->count;
              ++existing->count;
            }
            mapping.available_queue_family_index = available_queue_family_index;
            mapping.activated_queue_family_index = activated_queue_family_index;
            mapping.queue_count_index = queue_count_index;
            --left[ available_queue_family_index ];
            mapped = true;
            break;
          }
        }
      }
      if( !mapped ) {
        throw -1;
      }
    }
    std::sort(
      activated_queue_mappings.begin(),
      activated_queue_mappings.end(),
      []( const auto &l, const auto &r ) {
        return l.order < r.order;
      }
    );
    for( auto &f: activated_queue_family_counts )
      f.local_priority = std::vector< float >( f.count, 0.5f );
    return std::make_tuple(
      activated_queue_mappings,
      activated_queue_family_counts
    );
  }

  queue_t::queue_t(
    const std::shared_ptr< device_t > &d,
    vk::Queue q,
    const std::shared_ptr< command_pool_t > &p,
    std::uint32_t available_queue_family_index_
  ) :
    created_from< device_t >( d ),
    queue( q ),
    command_pool( p ),
    available_queue_family_index( available_queue_family_index_ ) {}
  std::shared_ptr< bound_command_pool_t > queue_t::get_command_pool() {
    return std::shared_ptr< bound_command_pool_t >(
      new bound_command_pool_t(
        shared_from_this(),
        command_pool
      )
    );
  }
#ifdef VK_KHR_SWAPCHAIN_EXTENSION_NAME
  void queue_t::present( const present_info_t &info_ ) {
    auto info = info_;
    info.rebuild_chain();
    auto present_result = queue.presentKHR( &info.get_basic() );
    if( present_result != vk::Result::eSuccess )
      vk::throwResultException( present_result, "presentKHR failed" );
  }
#endif
}

