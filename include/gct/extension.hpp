#ifndef GCT_EXTENSION_HPP
#define GCT_EXTENSION_HPP

#include <vulkan/vulkan.hpp>
#include <gct/deep_copy.hpp>
#include <gct/exception.hpp>

// NOLINTBEGIN(bugprone-macro-parentheses,bugprone-branch-clone)

#define LIBGCT_BASIC_SETTER_INDIRECT( member ) \
  private: \
    [[nodiscard]] basic2_t &get_writable_basic2() { \
      return basic ; \
    } \
    [[nodiscard]] basic_t &get_writable_basic() { \
      return basic. member ; \
    } \
  public: \
    self_type &set_basic () { \
      basic. member = basic_t(); \
      return *this; \
    } \
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
    self_type &operator=( basic_t &&v ) noexcept { \
      basic. member = std::move( v ); \
      return *this; \
    } \
    self_type &operator=( const basic2_t &v ) { \
      basic = v; \
      chained = false; \
      return *this; \
    } \
    self_type &operator=( basic2_t &&v ) noexcept { \
      basic = std::move( v ); \
      chained = false; \
      return *this; \
    } \
    [[nodiscard]] const basic2_t &get_basic2() const { \
      return basic; \
    } \
    [[nodiscard]] const basic_t &get_basic () const { \
      return basic. member ; \
    } \
    [[nodiscard]] constexpr bool has_basic() const { \
      return true; \
    } \
    [[nodiscard]] constexpr bool has_basic2() const { \
      return true; \
    }

#define LIBGCT_BASIC_SETTER_DIRECT \
  private: \
    [[nodiscard]] basic_t &get_writable_basic() { \
      return basic; \
    } \
  public: \
    self_type &set_basic () { \
      basic = basic_t(); \
      return *this; \
    } \
    self_type &set_basic ( const basic_t &v ) { \
      basic = v; \
      chained = false; \
      return *this; \
    } \
    self_type &set_basic ( basic_t &&v ) { \
      basic = std::move( v ); \
      chained = false; \
      return *this; \
    } \
    self_type &operator=( const basic_t &v ) { \
      basic = v; \
      chained = false; \
      return *this; \
    } \
    self_type &operator=( basic_t &&v ) { \
      basic = std::move( v ); \
      chained = false; \
      return *this; \
    } \
    [[nodiscard]] const basic_t &get_basic () const { \
      return basic; \
    } \
    [[nodiscard]] constexpr bool has_basic() const { \
      return true; \
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
    [[nodiscard]] type &get_writable_ ## name () { \
      return * name ; \
    } \
public: \
    self_type &set_ ## name () { \
      name .reset( new type () ); \
      chained = false; \
      return *this; \
    } \
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
    self_type &clear_ ## name () { \
      name .reset(); \
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
    [[nodiscard]] bool has_ ## name () const { \
      return bool( name ); \
    } \
    [[nodiscard]] const type &get_ ## name () const { \
      return * name ; \
    }

// NOLINTBEGIN(cppcoreguidelines-pro-type-const-cast)
#define LIBGCT_EXTENSION_REBUILD_CHAIN( name ) \
    if( name ) { \
      *prev = &* name ; \
      prev = const_cast< void** >( & name ->pNext ); \
    }
// NOLINTEND(cppcoreguidelines-pro-type-const-cast)

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
      root[ # name ] = v.get_ ## name (); \
    }
#define LIBGCT_EXTENSION_FROM_JSON( name ) \
    if( root.find( # name ) != root.end() ) { \
      v.set_ ## name ( root[ # name ] ); \
    }

// NOLINTBEGIN(cppcoreguidelines-pro-type-const-cast)

#define LIBGCT_EXTENSION_BEGIN_REBUILD_CHAIN \
    if( chained ) return *this; \
    head = &basic; \
    void **prev = const_cast< void** >( &basic.pNext );

// NOLINTEND(cppcoreguidelines-pro-type-const-cast)

#define LIBGCT_EXTENSION_END_REBUILD_CHAIN \
    *prev = nullptr; \
    chained = true; \
    return *this;

// NOLINTEND(bugprone-macro-parentheses,bugprone-branch-clone)

// NOLINTBEGIN(cppcoreguidelines-pro-type-const-cast)

#define LIBGCT_EXTENSION_REBUILD_CHAIN_DEF \
public: \
    self_type &rebuild_chain(); \
    self_type &rebuild_chain() const { \
      return const_cast< self_type* >( this )->rebuild_chain(); \
    }

/*
    self_type copy() const { \
      return *this; \
    }
*/

// NOLINTEND(cppcoreguidelines-pro-type-const-cast)

// NOLINTBEGIN(bugprone-macro-parentheses)

#define LIBGCT_EXTENSION_CHECK_SUBSET( name ) \
    if( sub. name && ! name ) { \
        throw gct::exception::runtime_error( "physical_device_features_t::check_subset : " #name " is not available.", __FILE__, __LINE__ ); \
    }

// NOLINTEND(bugprone-macro-parentheses)

class chained_t {
public:
  constexpr chained_t() noexcept = default;
  chained_t( const chained_t & ) {}
  chained_t( chained_t && ) noexcept {}
// NOLINTBEGIN(bugprone-unhandled-self-assignment,cert-oop54-cpp)
  chained_t &operator=( const chained_t & ) {
    head = nullptr;
    chained = false;
    return *this;
  }
// NOLINTEND(bugprone-unhandled-self-assignment,cert-oop54-cpp)
  chained_t &operator=( chained_t && ) noexcept {
    head = nullptr;
    chained = false;
    return *this;
  }
  [[nodiscard]] constexpr const void *get_head() const noexcept {
    return head;
  }
  [[nodiscard]] bool is_chained() const {
    return chained;
  }
// NOLINTBEGIN(cppcoreguidelines-non-private-member-variables-in-classes)
protected:
  void *head = nullptr;
  bool chained = false;
// NOLINTEND(cppcoreguidelines-non-private-member-variables-in-classes)
};

struct chainable_t {
  vk::StructureType sType;
  void* pNext;
};

namespace gct {
  [[nodiscard]] void **get_chain_tail( const void *v );
  LIBGCT_EXCEPTION( invalid_argument, incompatible_json, "incompatible JSON" )
}

#endif
