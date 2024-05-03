#include <iostream>
#include <nlohmann/json.hpp>
#include <gct/pipeline_layout.hpp>
#include <gct/command_buffer_recorder.hpp>
#include <gct/instance_list.hpp>
#include <gct/query_pool.hpp>

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
) : props( ci ), resource( graph.get_resource() ) {
  for( const auto &i: resource->inst.get_descriptor() ) {
    auto inst = resource->inst.get( i );
    draw_list.push_back(
      resource_pair()
        .set_inst( i )
        .set_prim( inst->descriptor.prim )
    );
  }
  if( !resource->push_constant_mp ) {
    throw -1;
  }
}
void instance_list::operator()(
  command_buffer_recorder_t &rec,
  const compiled_scene_graph &compiled
) const {
  std::vector< std::uint8_t > push_constant( resource->push_constant_mp->get_aligned_size(), 0u );
  const auto &prim = compiled.get_primitive();
  rec.bind_descriptor_set(
    vk::PipelineBindPoint::eGraphics,
    resource->descriptor_set_id,
    resource->pipeline_layout,
    resource->descriptor_set
  );
  rec.bind_descriptor_set(
    vk::PipelineBindPoint::eGraphics,
    resource->texture_descriptor_set_id,
    resource->pipeline_layout,
    resource->texture_descriptor_set
  );
  for( const auto &i: draw_list ) {
    auto p = prim.find( i.prim );
    if( p != prim.end() ) {
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
  }
}
void instance_list::operator()(
  command_buffer_recorder_t &rec,
  const compiled_aabb_scene_graph &compiled,
  const std::shared_ptr< query_pool_t > &query_pool
) const {
  std::vector< std::uint8_t > push_constant( resource->push_constant_mp->get_aligned_size(), 0u );
  const auto &prim = compiled.get_primitive();
  rec.bind_descriptor_set(
    vk::PipelineBindPoint::eGraphics,
    resource->descriptor_set_id,
    resource->pipeline_layout,
    resource->descriptor_set
  );
  rec.bind_descriptor_set(
    vk::PipelineBindPoint::eGraphics,
    resource->texture_descriptor_set_id,
    resource->pipeline_layout,
    resource->texture_descriptor_set
  );
  compiled( rec );
  for( const auto &i: draw_list ) {
    const auto inst = resource->inst.get( i.inst );
    auto p = prim.find( i.prim );
    if( inst && p != prim.end() ) {
      const auto query_token = rec.begin(
        query_pool,
        *inst->descriptor.visibility,
        vk::QueryControlFlags( 0 )
      );
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
  }
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

}

