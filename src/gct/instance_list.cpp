#include <iostream>
#include <nlohmann/json.hpp>
#include <gct/pipeline_layout.hpp>
#include <gct/command_buffer_recorder.hpp>
#include <gct/instance_list.hpp>

namespace gct::scene_graph {
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
  push_constant.resize( resource->push_constant_mp->get_aligned_size(), 0u );

}
void instance_list::operator()(
  command_buffer_recorder_t &rec,
  const compiled_scene_graph &compiled
) const {
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
  //std::cout << "instance_list::operator()" << std::endl;
  for( const auto &i: draw_list ) {
    //std::cout << "prim " << *i.prim << std::endl;
    auto p = prim.find( i.prim );
    if( p != prim.end() ) {
      //std::cout << "found " << *p->first << " " << nlohmann::json( p->second ) << std::endl;
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

