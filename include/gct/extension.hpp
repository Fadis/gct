#ifndef GCT_EXTENSION_HPP
#define GCT_EXTENSION_HPP

#include <vulkan/vulkan.hpp>
#include <gct/deep_copy.hpp>

#define LIBGCT_BASIC_SETTER_INDIRECT( member ) \
    self_type &set_basic ( const basic_t &v ) { \
      basic. member = v; \
      return *this; \
    } \
    self_type &set_basic ( basic_t &&v ) { \
      basic. member = std::move( v ); \
      return *this; \
    } \
    self_type &operator=( const basic_t &v ) { \
      basic. member = v; \
      return *this; \
    } \
    self_type &operator=( basic_t &&v ) { \
      basic. member = std::move( v ); \
      return *this; \
    } \
    self_type &operator=( const basic2_t &v ) { \
      basic = v; \
      chained = false; \
      return *this; \
    } \
    self_type &operator=( basic2_t &&v ) { \
      basic = std::move( v ); \
      chained = false; \
      return *this; \
    } \
    const basic2_t &get_basic2() const { \
      return basic; \
    } \
    const basic_t &get_basic () const { \
      return basic. member ; \
    }

#define LIBGCT_BASIC_SETTER_DIRECT \
    self_type &set_basic ( const basic_t &v ) { \
      basic = v; \
      return *this; \
    } \
    self_type &set_basic ( basic_t &&v ) { \
      basic = std::move( v ); \
      return *this; \
    } \
    self_type &operator=( const basic_t &v ) { \
      basic = v; \
      return *this; \
    } \
    self_type &operator=( basic_t &&v ) { \
      basic = std::move( v ); \
      return *this; \
    } \
    const basic_t &get_basic () const { \
      return basic; \
    }

#define LIBGCT_BASIC_SETTER( type ) \
private: \
    using basic_t = type ;\
    basic_t basic; \
public: \
    LIBGCT_BASIC_SETTER_DIRECT

#define LIBGCT_EXTENSION_SETTER( type, name ) \
private: \
    using name ## _t = type; \
    deep_copy_unique_ptr< type > name ; \
public: \
    self_type &set_ ## name ( const type &v ) { \
      name .reset( new type ( v ) ); \
      chained = false; \
      return *this; \
    } \
    self_type &set_ ## name ( type &&v ) { \
      name .reset( new type ( std::move( v ) ) ); \
      chained = false; \
      return *this; \
    } \
    self_type &operator=( const type &v ) { \
      name .reset( new type ( v ) ); \
      chained = false; \
      return *this; \
    } \
    self_type &operator=( type &&v ) { \
      name .reset( new type ( std::move( v ) ) ); \
      chained = false; \
      return *this; \
    } \
    self_type &remove_ ## name () { \
      name .reset(); \
      chained = false; \
      return *this; \
    } \
    bool has_ ## name () const { \
      return bool( name ); \
    } \
    const type &get_ ## name () const { \
      return * name ; \
    }

#define LIBGCT_EXTENSION_REBUILD_CHAIN( name ) \
    if( name ) { \
      *prev = &* name ; \
      prev = const_cast< void** >( & name ->pNext ); \
    }

#define LIBGCT_EXTENSION_CREATE_IF_EXT( name, extname ) \
    if( ext.find( extname ) != ext.end() ) { \
      name .reset( new name ## _t() ); \
      chained = false; \
    }

#define LIBGCT_EXTENSION_CREATE_IF_VERSION( name, major, minor, patch ) \
    if( instance.get_api_version() >= VK_MAKE_VERSION( major, minor, patch ) ) { \
      name .reset( new name ## _t() ); \
      chained = false; \
    }

#define LIBGCT_EXTENSION_CREATE_IF_VERSION_OR_EXT( name, major, minor, patch, extname ) \
    LIBGCT_EXTENSION_CREATE_IF_VERSION( name, major, minor, patch ) \
    else \
    LIBGCT_EXTENSION_CREATE_IF_EXT( name, extname )

#define LIBGCT_EXTENSION_TO_JSON( name ) \
    if( v.has_ ## name () ) { \
      root[ # name ] = to_json( v.get_ ## name () ); \
    }

#define LIBGCT_EXTENSION_BEGIN_REBUILD_CHAIN \
    if( chained ) return *this; \
    head = &basic; \
    void **prev = const_cast< void** >( &basic.pNext );

#define LIBGCT_EXTENSION_END_REBUILD_CHAIN \
    *prev = nullptr; \
    chained = true; \
    return *this;

#define LIBGCT_EXTENSION_REBUILD_CHAIN_DEF \
private: \
    void *head = nullptr; \
    bool chained = true; \
public: \
    const void *get_head() const { \
      return head; \
    } \
    self_type &rebuild_chain(); \
    self_type copy() const { \
      return *this; \
    }

struct chainable_t {
  vk::StructureType sType;
  void* pNext;
};

namespace gct {
  void **get_chain_tail( const void *v );
}
 

#endif
