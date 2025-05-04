#include <nlohmann/json.hpp>
#include <gct/allocator.hpp>
#include <gct/mappable_buffer.hpp>
#include <gct/pipeline_layout.hpp>
#include <gct/command_buffer_recorder.hpp>
#include <gct/instance_list.hpp>
#include <gct/query_pool.hpp>
#include <gct/aabb.hpp>
#include <gct/kdtree.hpp>
#include <gct/allocator.hpp>
#include <gct/device.hpp>
#include <gct/render_pass.hpp>
#include <gct/conditional_rendering_begin_info.hpp>
#include <gct/get_device.hpp>

namespace gct::scene_graph {

bool operator==( const resource_pair &l, const resource_pair &r ) {
  return l.inst == r.inst && l.prim == r.prim;
}

bool operator!=( const resource_pair &l, const resource_pair &r ) {
  return l.inst != r.inst || l.prim != r.prim;
}

instance_list::instance_list(
  const instance_list_create_info &ci,
  const scene_graph &graph
) : property_type( ci ), resource( graph.get_resource() ) {
  for( const auto &i: resource->inst.get_descriptor() ) {
    const auto inst = resource->inst.get( i );
    draw_list.push_back(
      resource_pair()
        .set_inst( i )
        .set_prim( inst->descriptor.prim )
    );
    const auto prim = resource->prim.get( inst->descriptor.prim );
    max_primitive_count = std::max( max_primitive_count,  prim->count / 3u );
  }
  if( !resource->push_constant_mp ) {
    throw -1;
  }
  const auto &device = get_device( *graph.get_props().allocator_set.allocator );
#ifdef VK_EXT_CONDITIONAL_RENDERING_EXTENSION_NAME
  enable_conditional = device.get_activated_extensions().find( VK_EXT_CONDITIONAL_RENDERING_EXTENSION_NAME ) != device.get_activated_extensions().end();
#else
  enable_conditional = false;
#endif
  update_device_side_list();
}
void instance_list::operator()(
  command_buffer_recorder_t &rec,
  const compiled_scene_graph &compiled,
  bool conditional
) const {
  return (*this)( rec, compiled, std::nullopt, syncable(), conditional );
}
void instance_list::operator()(
  command_buffer_recorder_t &rec,
  const compiled_scene_graph &compiled,
  const std::optional< render_pass_begin_info_t > &rp,
  const syncable &barrier_target,
  bool conditional
) const {
  std::vector< std::uint8_t > push_constant( resource->push_constant_mp->get_aligned_size(), 0u );
  const auto &prim = compiled.get_primitive();
  rec.bind_descriptor_set(
    vk::PipelineBindPoint::eGraphics,
    resource->descriptor_set_id,
    resource->pipeline_layout,
    resource->descriptor_set
  );
  if( resource->texture_descriptor_set ) {
    rec.bind_descriptor_set(
      vk::PipelineBindPoint::eGraphics,
      resource->texture_descriptor_set_id,
      resource->pipeline_layout,
      resource->texture_descriptor_set
    );
  }
  if( resource->image_descriptor_set ) {
    rec.bind_descriptor_set(
      vk::PipelineBindPoint::eGraphics,
      resource->image_descriptor_set_id,
      resource->pipeline_layout,
      resource->image_descriptor_set
    );
  }
  const bool enable_barrier = !barrier_target.empty();
  for( const auto &v: draw_list ) {
    auto p = prim.find( v.prim );
    auto i = resource->inst.get( v.inst );
    if( p != prim.end() && i ) {
      push_constant.data() ->* (*resource->push_constant_mp)[ "instance" ] = *v.inst;
      push_constant.data() ->* (*resource->push_constant_mp)[ "primitive" ] = *v.prim;
      rec->pushConstants(
        **resource->pipeline_layout,
        resource->pipeline_layout->get_props().get_push_constant_range()[ 0 ].stageFlags,
        resource->push_constant_mp->get_offset(),
        push_constant.size(),
        push_constant.data()
      );
      if( rp ) {
        {
          if( conditional && enable_conditional ) {
#ifdef VK_EXT_CONDITIONAL_RENDERING_EXTENSION_NAME
            auto token = rec.begin(
              conditional_rendering_begin_info_t()
                .set_buffer( resource->last_visibility->get_buffer() )
                .set_basic(
                  vk::ConditionalRenderingBeginInfoEXT()
                    .setOffset( *i->descriptor.visibility * sizeof( std::uint32_t ) )
                )
            );
#endif
            auto render_pass_token = rec.begin_render_pass(
              *rp,
              vk::SubpassContents::eInline
            );
            (p->second)( rec );
            if( enable_barrier ) {
              rec.barrier( barrier_target );
            }
          }else {
            (p->second)( rec );
            if( enable_barrier ) {
              rec.barrier( barrier_target );
            }
          }
        }
      }
      else {
        if( conditional && enable_conditional ) {
#ifdef VK_EXT_CONDITIONAL_RENDERING_EXTENSION_NAME
          auto token = rec.begin(
            conditional_rendering_begin_info_t()
              .set_buffer( resource->last_visibility->get_buffer() )
              .set_basic(
                vk::ConditionalRenderingBeginInfoEXT()
                  .setOffset( *i->descriptor.visibility * sizeof( std::uint32_t ) )
              )
          );
#endif
          (p->second)( rec );
        }else {
          (p->second)( rec );
        }
      }
    }
  }
}
void instance_list::update_device_side_list() {
  device_side_list = resource->resource_pair->allocate( draw_list.size() );
  const auto mp = resource->resource_pair->get_member_pointer();
  std::vector< std::uint8_t > temp( mp.get_aligned_size(), 0u );
  for( std::uint32_t i = 0u; i != draw_list.size(); ++i ) {
    if( mp.has( "instance" ) ) {
      temp.data()->*mp[ "instance" ] = *draw_list[ i ].inst;
    }
    else if( mp.has( "inst" ) ) {
      temp.data()->*mp[ "inst" ] = *draw_list[ i ].inst;
    }
    if( mp.has( "primitive" ) ) {
      temp.data()->*mp[ "primitive" ] = *draw_list[ i ].prim;
    }
    else if( mp.has( "prim" ) ) {
      temp.data()->*mp[ "prim" ] = *draw_list[ i ].prim;
    }
    resource->resource_pair->set( device_side_list, i, temp.data(), std::next( temp.data(), temp.size() ) );
  }
}

void instance_list::setup_resource_pair_buffer(
  command_buffer_recorder_t &rec
) const {
  std::vector< std::uint8_t > push_constant( resource->push_constant_mp->get_aligned_size(), 0u );
  if( resource->push_constant_mp->has( "offset" ) ) {
    push_constant.data() ->* (*resource->push_constant_mp)[ "offset" ] = *device_side_list;
  }
  else if( resource->push_constant_mp->has( "instance" ) ) {
    push_constant.data() ->* (*resource->push_constant_mp)[ "instance" ] = *device_side_list;
  }
  if( resource->push_constant_mp->has( "count" ) ) {
    push_constant.data() ->* (*resource->push_constant_mp)[ "count" ] = std::uint32_t( draw_list.size() );
  }
  else if( resource->push_constant_mp->has( "primitive" ) ) {
    push_constant.data() ->* (*resource->push_constant_mp)[ "primitive" ] = std::uint32_t( draw_list.size() );
  }
  rec->pushConstants(
    **resource->pipeline_layout,
    resource->pipeline_layout->get_props().get_push_constant_range()[ 0 ].stageFlags,
    resource->push_constant_mp->get_offset(),
    push_constant.size(),
    push_constant.data()
  );
}
void instance_list::operator()(
  command_buffer_recorder_t &rec,
  const compiled_aabb_scene_graph &compiled
) const {
  rec.bind_descriptor_set(
    vk::PipelineBindPoint::eGraphics,
    resource->descriptor_set_id,
    resource->pipeline_layout,
    resource->descriptor_set
  );
  if( resource->texture_descriptor_set ) {
    rec.bind_descriptor_set(
      vk::PipelineBindPoint::eGraphics,
      resource->texture_descriptor_set_id,
      resource->pipeline_layout,
      resource->texture_descriptor_set
    );
  }
  if( resource->image_descriptor_set ) {
    rec.bind_descriptor_set(
      vk::PipelineBindPoint::eGraphics,
      resource->image_descriptor_set_id,
      resource->pipeline_layout,
      resource->image_descriptor_set
    );
  }
  compiled( rec, draw_list.size() );
  /*for( const auto &i: draw_list ) {
    const auto inst = resource->inst.get( i.inst );
    auto p = prim.find( i.prim );
    if( inst && p != prim.end() ) {
      push_constant.data() ->* (*resource->push_constant_mp)[ "instance" ] = *i.inst;
      push_constant.data() ->* (*resource->push_constant_mp)[ "primitive" ] = *i.prim;
      rec->pushConstants(
        **resource->pipeline_layout,
        resource->pipeline_layout->get_props().get_push_constant_range()[ 0 ].stageFlags,
        resource->push_constant_mp->get_offset(),
        push_constant.size(),
        push_constant.data()
      );
      (p->second)( rec );
    }
  }*/
}
void instance_list::operator()(
  command_buffer_recorder_t &rec,
  const graphics &compiled
) const {
  setup_resource_pair_buffer( rec );
  if( props.parallel_mode ) {
    compiled( rec, 0u, 32u, draw_list.size(), 1u );
  }
  else {
    compiled( rec, 0u, draw_list.size(), 1u, 1u );
  }
}
std::vector< resource_pair > instance_list::get_last_visible_list() const {
  std::unordered_set< std::uint32_t > visible_instance_ids;
  std::vector< std::uint32_t > visibility;
  {
    const auto mapped = resource->last_visibility->map< std::uint32_t >();
    visibility.assign( mapped.begin(), mapped.end() );
  }
  std::uint32_t instance_id = 0u;
  for( const auto &v: visibility ) {
    if( v ) {
      visible_instance_ids.insert( instance_id );
    }
    ++instance_id;
  }
  std::vector< resource_pair > visible_instance;
  for( const auto &v: draw_list ) {
    const auto inst = resource->inst.get( v.inst );
    if( inst ) {
      if( visible_instance_ids.find( *inst->descriptor.visibility ) != visible_instance_ids.end() ) {
        visible_instance.push_back( v );
      }
    }
  }
  return visible_instance;
}


void append(
  const scene_graph_resource &resource,
  const std::vector< resource_pair > &l,
  kdtree< resource_pair > &kd
) {
  for( const auto &v: l ) {
    const auto inst = resource.inst.get( v.inst );
    if( inst ) {
      kd.insert( inst->initial_world_aabb, v );
    }
  }
}

void to_json( nlohmann::json &dest, const resource_pair &src ) {
  dest = nlohmann::json::object();
  if( src.inst ) {
    dest[ "instance" ] = *src.inst;
  }
  if( src.prim ) {
    dest[ "primitive" ] = *src.prim;
  }
}

void instance_list::to_json( nlohmann::json &dest ) const {
  dest = nlohmann::json::object();
  dest[ "draw_list" ] = nlohmann::json::array();
  for( const auto &v: draw_list ) {
    dest[ "draw_list" ].push_back( v );
  }
}

void to_json( nlohmann::json &dest, const instance_list &src ) {
  src.to_json( dest );
}

void sort_by_distance(
  std::vector< kdtree< resource_pair > > &dest,
  const glm::vec3 &eye_pos
) {
  std::vector< std::pair< float, kdtree< resource_pair > > > temp;
  temp.reserve( dest.size() );
  std::transform(
    dest.begin(),
    dest.end(),
    std::back_inserter( temp ),
    [&]( const auto &v ) {
      const auto distance = glm::distance( get_center( v.get_aabb() ), eye_pos );
      return std::make_pair( distance, v );
    }
  );
  std::sort( temp.begin(), temp.end(), []( const auto &l, const auto &r ) { return l.first < r.first; } );
  std::transform(
    temp.begin(),
    temp.end(),
    dest.begin(),
    [&]( const auto &v ) {
      return v.second;
    }
  );
}

}

