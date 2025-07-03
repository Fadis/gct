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
#include <gct/compute.hpp>

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
) : instance_list( ci, graph, graph.get_resource()->inst.get_descriptor() ) {}
instance_list::instance_list(
  const instance_list_create_info &ci,
  const scene_graph &graph,
  const std::vector< pool< std::shared_ptr< instance > >::descriptor > &l
) : property_type( ci ), resource( graph.get_resource() ) {
  std::unordered_set< pool< std::shared_ptr< primitive > >::descriptor > known_prim;
  for( const auto &i: l ) {
    const auto inst = resource->inst.get( i );
    if( inst->is_highest_lod || props.all_lods ) {
      if( props.unique_prim_list ) {
        if( known_prim.find( inst->descriptor.prim ) == known_prim.end() ) {
          known_prim.insert( inst->descriptor.prim );
          draw_list.push_back(
            resource_pair()
              .set_inst( i )
              .set_prim( inst->descriptor.prim )
          );
        }
      }
      else {
        draw_list.push_back(
          resource_pair()
            .set_inst( i )
            .set_prim( inst->descriptor.prim )
        );
      }
    }
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
  if( resource->resource_pair ) {
    const auto mp = resource->resource_pair->get_member_pointer();
    std::vector< std::uint8_t > temp( mp.get_aligned_size(), 0u );
    {
      device_side_list = resource->resource_pair->allocate( draw_list.size() );
      for( std::uint32_t i = 0u; i != draw_list.size(); ++i ) {
        const auto inst = resource->inst.get( draw_list[ i ].inst );
        if( !inst ) throw -1;
        const auto prim = resource->prim.get( draw_list[ i ].prim );
        if( !prim ) throw -1;
        if( mp.has( "instance" ) ) {
          temp.data()->*mp[ "instance" ] = std::uint32_t( *inst->descriptor.resource_index + inst->lod_id );
        }
        else if( mp.has( "inst" ) ) {
          temp.data()->*mp[ "inst" ] = std::uint32_t( *inst->descriptor.resource_index + inst->lod_id );
        }
        if( mp.has( "primitive" ) ) {
          temp.data()->*mp[ "primitive" ] = *prim->descriptor.resource_index;
        }
        else if( mp.has( "prim" ) ) {
          temp.data()->*mp[ "prim" ] = *prim->descriptor.resource_index;
        }
        if( mp.has( "offset" ) ) {
          temp.data()->*mp[ "offset" ] = 0u;
        }
        resource->resource_pair->set( device_side_list, i, temp.data(), std::next( temp.data(), temp.size() ) );
      }
    }
    if( props.parallel_mode3 ) {
      std::uint32_t total_task_count = 0u;
      std::unordered_map< pool< std::shared_ptr< primitive > >::descriptor, std::uint32_t > task_count;
      for( std::uint32_t i = 0u; i != draw_list.size(); ++i ) {
        const auto inst = resource->inst.get( draw_list[ i ].inst );
        const auto prim = resource->prim.get( draw_list[ i ].prim );
        if( !prim ) throw -1;
        const std::uint32_t tc = prim->count / ( 3u * 32u * 32u ) + (( prim->count % ( 3u * 32u * 32u ) ) ? 1u : 0u );
        inst->set_mesh_task_offset( total_task_count );
        inst->set_mesh_task_count( tc );
        task_count[ draw_list[ i ].prim ] = tc;
        total_task_count += tc;
      }
      meshlet_list = resource->resource_pair->allocate( total_task_count );
      std::uint32_t current_task_offset = 0u;
      for( std::uint32_t i = 0u; i != draw_list.size(); ++i ) {
        const auto inst = resource->inst.get( draw_list[ i ].inst );
        if( !inst ) throw -1;
        const auto prim = resource->prim.get( draw_list[ i ].prim );
        if( !prim ) throw -1;
        for( std::uint32_t task_index = 0u; task_index != task_count[ draw_list[ i ].prim ]; ++task_index ) {
          if( mp.has( "instance" ) ) {
            temp.data()->*mp[ "instance" ] = std::uint32_t( *inst->descriptor.resource_index + inst->lod_id );
          }
          else if( mp.has( "inst" ) ) {
            temp.data()->*mp[ "inst" ] = std::uint32_t( *inst->descriptor.resource_index + inst->lod_id );
          }
          if( mp.has( "primitive" ) ) {
            temp.data()->*mp[ "primitive" ] = *prim->descriptor.resource_index;
          }
          else if( mp.has( "prim" ) ) {
            temp.data()->*mp[ "prim" ] = *prim->descriptor.resource_index;
          }
          if( mp.has( "offset" ) ) {
            temp.data()->*mp[ "offset" ] = task_index;
          }
          resource->resource_pair->set(
            meshlet_list,
            current_task_offset + task_index,
            temp.data(), std::next( temp.data(), temp.size() )
          );
        }
        current_task_offset += task_count[ draw_list[ i ].prim ];
      }
      meshlet_list_size = total_task_count;
    }
  }
}

void instance_list::setup_resource_pair_buffer(
  command_buffer_recorder_t &rec
) const {
  const auto &pcmp = resource->push_constant_mp;
  if( pcmp ) {
    std::vector< std::uint8_t > push_constant( resource->push_constant_mp->get_aligned_size(), 0u );
    if( pcmp->has( "offset" ) ) {
      push_constant.data() ->* (*pcmp)[ "offset" ] = device_side_list ? *device_side_list : 0u;
    }
    else if( pcmp->has( "instance" ) ) {
      push_constant.data() ->* (*pcmp)[ "instance" ] = device_side_list ? *device_side_list : 0u;
    }
    if( pcmp->has( "count" ) ) {
      push_constant.data() ->* (*pcmp)[ "count" ] = std::uint32_t( draw_list.size() );
    }
    else if( pcmp->has( "primitive" ) ) {
      push_constant.data() ->* (*pcmp)[ "primitive" ] = std::uint32_t( draw_list.size() );
    }
    rec->pushConstants(
      **resource->pipeline_layout,
      resource->pipeline_layout->get_props().get_push_constant_range()[ 0 ].stageFlags,
      pcmp->get_offset(),
      push_constant.size(),
      push_constant.data()
    );
  }
}
void instance_list::setup_resource_pair_buffer(
  bool use_meshlet,
  const compute &compiled
) const {
  const auto &pcmp = compiled.get_push_constant_member_pointer();
  if( pcmp ) {
    std::vector< std::uint8_t > &push_constant = compiled.get_push_constant();
    if( pcmp->has( "offset" ) ) {
      push_constant.data() ->* (*pcmp)[ "offset" ] = use_meshlet ? ( meshlet_list ? *meshlet_list : 0u ) : ( device_side_list ? *device_side_list : 0u );
    }
    else if( pcmp->has( "instance" ) ) {
      push_constant.data() ->* (*pcmp)[ "instance" ] = use_meshlet ? ( meshlet_list ? *meshlet_list : 0u ) : ( device_side_list ? *device_side_list : 0u );
    }
    if( pcmp->has( "count" ) ) {
      push_constant.data() ->* (*pcmp)[ "count" ] = std::uint32_t( use_meshlet ? meshlet_list_size : draw_list.size() );
    }
    else if( pcmp->has( "primitive" ) ) {
      push_constant.data() ->* (*pcmp)[ "primitive" ] = std::uint32_t( use_meshlet ? meshlet_list_size : draw_list.size() );
    }
  }
}
void instance_list::setup_resource_pair_buffer( ///
  bool use_meshlet,
  const std::vector< resource_pair >::const_iterator &i,
  const compute &compiled
) const {
  const auto &pcmp = compiled.get_push_constant_member_pointer();
  if( pcmp ) {
    std::vector< std::uint8_t > &push_constant = compiled.get_push_constant();
    const auto inst = resource->inst.get( i->inst );
    const std::size_t offset = use_meshlet ? inst->mesh_task_offset : std::distance( draw_list.begin(), i );
    const std::uint32_t dli = std::distance( draw_list.begin(), i );
    if( pcmp->has( "offset" ) ) {
      push_constant.data() ->* (*pcmp)[ "offset" ] = std::uint32_t( use_meshlet ? ( *meshlet_list + offset ): ( device_side_list ? ( *device_side_list + offset ) : offset ) );
    }
    else if( pcmp->has( "instance" ) ) {
      push_constant.data() ->* (*pcmp)[ "instance" ] = std::uint32_t( use_meshlet ? ( *meshlet_list + offset ): ( device_side_list ? ( *device_side_list + offset ) : offset ) );
    }
    if( pcmp->has( "count" ) ) {
      push_constant.data() ->* (*pcmp)[ "count" ] = std::uint32_t( use_meshlet ? inst->mesh_task_count : 1u );
    }
    else if( pcmp->has( "primitive" ) ) {
      push_constant.data() ->* (*pcmp)[ "primitive" ] = std::uint32_t( use_meshlet ? inst->mesh_task_count : 1u );
    }
  }
}
void instance_list::setup_resource_pair_buffer(
  bool use_meshlet,
  const graphics &compiled
) const {
  const auto &pcmp = compiled.get_push_constant_member_pointer();
  if( pcmp ) {
    std::vector< std::uint8_t > &push_constant = compiled.get_push_constant();
    if( pcmp->has( "offset" ) ) {
      push_constant.data() ->* (*pcmp)[ "offset" ] = use_meshlet ? ( meshlet_list ? *meshlet_list : 0u ) : ( device_side_list ? *device_side_list : 0u );
    }
    else if( pcmp->has( "instance" ) ) {
      push_constant.data() ->* (*pcmp)[ "instance" ] = use_meshlet ? ( meshlet_list ? *meshlet_list : 0u ) : ( device_side_list ? *device_side_list : 0u );
    }
    if( pcmp->has( "count" ) ) {
      push_constant.data() ->* (*pcmp)[ "count" ] = std::uint32_t( use_meshlet ? meshlet_list_size : draw_list.size() );
    }
    else if( pcmp->has( "primitive" ) ) {
      push_constant.data() ->* (*pcmp)[ "primitive" ] = std::uint32_t( use_meshlet ? meshlet_list_size : draw_list.size() );
    }
  }
}
void instance_list::setup_resource_pair_buffer( ///
  bool use_meshlet,
  const std::vector< resource_pair >::const_iterator &i,
  const graphics &compiled
) const {
  const auto &pcmp = compiled.get_push_constant_member_pointer();
  if( pcmp ) {
    std::vector< std::uint8_t > &push_constant = compiled.get_push_constant();
    const auto inst = resource->inst.get( i->inst );
    const std::size_t offset = use_meshlet ? inst->mesh_task_offset : std::distance( draw_list.begin(), i );
    const std::uint32_t dli = std::distance( draw_list.begin(), i );
    if( pcmp->has( "offset" ) ) {
      push_constant.data() ->* (*pcmp)[ "offset" ] = std::uint32_t( use_meshlet ? ( meshlet_list ? ( *meshlet_list + offset ) : offset ) : ( device_side_list ? ( *device_side_list + offset ) : offset ) );
    }
    else if( pcmp->has( "instance" ) ) {
      push_constant.data() ->* (*pcmp)[ "instance" ] = std::uint32_t( use_meshlet ? ( meshlet_list ? ( *meshlet_list + offset ) : offset ) : ( device_side_list ? ( *device_side_list + offset ) : offset ) );
    }
    if( pcmp->has( "count" ) ) {
      push_constant.data() ->* (*pcmp)[ "count" ] = std::uint32_t( use_meshlet ? inst->mesh_task_count : 1u );
    }
    else if( pcmp->has( "primitive" ) ) {
      push_constant.data() ->* (*pcmp)[ "primitive" ] = std::uint32_t( use_meshlet ? inst->mesh_task_count : 1u );
    }
  }
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
  const auto pcmp = compiled.get_push_constant_member_pointer();
  if( props.parallel_mode3 ) {
    setup_resource_pair_buffer( true, compiled );
    compiled( rec, 0u, 32u, meshlet_list_size, 1u );
  }
  else if( props.parallel_mode2 ) {
    setup_resource_pair_buffer( false, compiled );
    compiled( rec, 0u, ( max_primitive_count / 32u ) + ( ( max_primitive_count % 32u ) ? 1u : 0u ), draw_list.size(), 1u );
  }
  else if( props.parallel_mode ) {
    setup_resource_pair_buffer( false, compiled );
    compiled( rec, 0u, 32u, draw_list.size(), 1u );
  }
  else {
    // push constantsにresource_pairの何要素目から読むべきかを書く
    setup_resource_pair_buffer( false, compiled );
    // 描画対象の数だけタスクシェーダーのローカルワークグループを実行
    compiled( rec, 0u, draw_list.size(), 1u, 1u );
  }
}
void instance_list::operator()(
  command_buffer_recorder_t &rec,
  const graphics &compiled,
  const std::vector< resource_pair >::const_iterator &i
) const {
  if( props.parallel_mode3 ) {
    const auto inst = resource->inst.get( i->inst );
    setup_resource_pair_buffer( true, i, compiled );
    compiled( rec, 0u, 32u, inst->mesh_task_count, 1u );
  }
  else if( props.parallel_mode2 ) {
    setup_resource_pair_buffer( false, i, compiled );
    compiled( rec, 0u, ( max_primitive_count / 32u ) + ( ( max_primitive_count % 32u ) ? 1u : 0u ), draw_list.size(), 1u );
  }
  else if( props.parallel_mode ) {
    setup_resource_pair_buffer( false, i, compiled );
    compiled( rec, 0u, 32u, 1u, 1u );
  }
  else {
    setup_resource_pair_buffer( false, i, compiled );
    compiled( rec, 0u, 1u, 1u, 1u );
  }
}
void instance_list::for_each_unique_vertex(
  command_buffer_recorder_t &rec,
  const compute &compiled,
  const std::vector< resource_pair >::const_iterator &i
) const {
  const auto pcmp = compiled.get_push_constant_member_pointer();
  const auto inst = resource->inst.get( i->inst );
  const auto prim = resource->prim.get( i->prim );
  setup_resource_pair_buffer( true, i, compiled );
  compiled( rec, 0u, prim->mesh.unique_vertex_count, 1u, 1u );
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

