#ifndef GCT_FUTURE_HPP
#define GCT_FUTURE_HPP
#include <tuple>
#include <atomic>
#include <variant>
#include <optional>
#include <exception>
#include <gct/non_copyable_function.hpp>
#include <gct/type_traits/merge_sequence.hpp>
#include <gct/type_traits/is_lifted.hpp>
#include <gct/type_traits/sequence_size.hpp>
#include <gct/type_traits/nth_type.hpp>
#include <gct/type_traits/type_sequence.hpp>
#include <gct/type_traits/lift.hpp>
#include <gct/type_traits/filter_sequence.hpp>
#include <gct/type_traits/if.hpp>
#include <gct/type_traits/for_each.hpp>
#include <gct/type_traits/count_sequence.hpp>
#include <gct/tuplise.hpp>

namespace gct {
  struct basic_future_shared_resource {
    using cancel_type = non_copyable_function< void() >;
    basic_future_shared_resource() : cancel_( [](){} ), count( 0 ) {};
    void set_cancel( cancel_type &&f ) {
      cancel_ = [f=std::move( f ),g=std::move( cancel_ )]() mutable {
        g();
        f();
      };
    }
    cancel_type cancel_;
    std::atomic< int > count;
  };
  template< typename T >
  struct future_shared_resource final : public basic_future_shared_resource {
    using then_type = non_copyable_function< void( std::variant< std::exception_ptr, T >&& ) >;
    future_shared_resource() = default;
    ~future_shared_resource() {
      try {
        cancel();
      }
      catch( ... ) {}
    }
    void set_value( T &&v ) {
      value = std::move( v );
      if( count.fetch_add( 1 ) == 1 ) {
        then( std::move( value ) );
        clear();
      }
    }
    void set_exception( std::exception_ptr p ) {
      value = p;
      if( count.fetch_add( 1 ) == 1 ) {
        then( std::move( value ) );
        clear();
      }
    }
    void set_then( then_type &&f ) {
      then = std::move( f );
      if( count.fetch_add( 1 ) == 1 ) {
        then( std::move( value ) );
        clear();
      }
    }
    void cancel() {
      int expected = count.load();
      while( expected < 2 ) {
        if( count.compare_exchange_strong( expected, 2 ) ) {
          cancel_();
          clear();
        }
      }
    }
    void clear() {
      cancel_ = cancel_type();
      then = then_type();
// NOLINTBEGIN(bugprone-throw-keyword-missing)
      value = std::exception_ptr();
// NOLINTEND(bugprone-throw-keyword-missing)
    }
    std::variant< std::exception_ptr, T > value;
    then_type then;
  };
  template<>
  struct future_shared_resource< void > final : public basic_future_shared_resource {
    using then_type = non_copyable_function< void( std::exception_ptr ) >;
    future_shared_resource() {};
    ~future_shared_resource() {
      try {
        cancel();
      }
// NOLINTBEGIN(bugprone-empty-catch)
      catch( ... ) {}
// NOLINTEND(bugprone-empty-catch)
    }
    void set_value() {
      if( count.fetch_add( 1 ) == 1 ) {
        then( value );
        clear();
      }
    }
    void set_exception( std::exception_ptr p ) {
      value = p;
      if( count.fetch_add( 1 ) == 1 ) {
        then( value );
        clear();
      }
    }
    void set_then( then_type &&f ) {
      then = std::move( f );
      if( count.fetch_add( 1 ) == 1 ) {
        then( std::move( value ) );
        clear();
      }
    }
    void cancel() {
      int expected = count.load();
      while( expected < 2 ) {
        if( count.compare_exchange_strong( expected, 2 ) ) {
          cancel_();
          clear();
        }
      }
    }
    void clear() {
      cancel_ = cancel_type();
      then = then_type();
      value = std::exception_ptr();
    }
    std::exception_ptr value;
    then_type then;
  };
  
  template< typename T >
  class promise;
  template< typename T >
  class future;

  template< typename T >
  struct is_future : public std::false_type {};
  template< typename T >
  struct is_future< future< T > > : public std::true_type {};
  template< typename T >
  constexpr bool is_future_v = is_future< std::remove_cvref_t< T > >::value;

  template< typename T >
  struct unwrap {};
  template< template< typename ... > typename L, typename T, typename ...Args >
  struct unwrap< L< T, Args... > > {
    using type = T;
  };
  template< typename T >
  using unwrap_t = typename unwrap< std::remove_cvref_t< T > >::type;

  template< typename T >
  class future {
  private:
    template< typename U > friend class future;
    using shared_type = future_shared_resource< T >;
    using then_type = typename shared_type::then_type;
  public:
    future() : raw( nullptr ) {}
// NOLINTBEGIN(cppcoreguidelines-prefer-member-initializer)
    future( const std::shared_ptr< shared_type > &s ) : shared( s ) {
      raw = shared.get();
    }
    future( const future& ) = delete;
    future( future &&src ) noexcept : shared( std::move( src.shared ) )  {
      raw = shared.get();
      src.shared.reset();
      src.raw = nullptr;
    }
// NOLINTEND(cppcoreguidelines-prefer-member-initializer)
    future &operator=( const future& ) = delete;
    future &operator=( future &&src ) noexcept {
      shared = std::move( src.shared );
      raw = shared.get();
      src.shared.reset();
      src.raw = nullptr;
      return *this;
    }
    template< typename F >
    auto then( F &&then_ ) {
      if constexpr ( std::is_same_v< T, void > ) {
        if constexpr ( std::is_invocable_v< F > ) {
          using next_type = std::remove_cvref_t< decltype( then_() ) >;
          if constexpr ( is_future_v< next_type > ) {
            using next_type = unwrap_t< next_type >;
            promise< next_type > p;
            p.link_weak( shared );
            auto f = p.get_future();
            if( raw ) raw->set_then(
              [then_=std::forward<F>( then_ ),p=std::move( p )] ( std::exception_ptr v ) mutable {
                if( v ) {
                  p.set_exception( v );
                }
                else {
                  try {
                    auto f2 = then_();
                    p.link_weak( f2.shared );
                    if constexpr ( std::is_same_v< next_type, void > ) {
                      f2.set_then(
                        [p=std::move( p )]( std::exception_ptr v ) mutable {
                          if( v ) {
                            p.set_exception( v );
                          }
                          else {
                            p.set_value();
                          }
                        }
                      );
                    }
                    else {
                      f2.set_then(
                        [p=std::move( p )]( std::variant< std::exception_ptr, next_type > &&v ) mutable {
                          if( v.index() == 0u ) {
                            p.set_exception( std::get< std::exception_ptr >( v ) );
                          }
                          else {
                            p.set_value( std::get< next_type >( v ) );
                          }
                        }
                      );
                    }
                  }
                  catch( ... ) {
                    p.set_exception( std::current_exception() );
                  }
                }
              }
            );
            return f;
          }
          else {
            promise< next_type > p;
            p.link_weak( shared );
            auto f = p.get_future();
            if( raw ) raw->set_then(
              [then_=std::forward<F>( then_ ),p=std::move( p )] ( std::exception_ptr v ) mutable {
                if( v ) {
                  p.set_exception( v );
                }
                else {
                  try {
                    if constexpr ( std::is_same_v< next_type, void > ) {
                      then_();
                      p.set_value();
                    }
                    else {
                      p.set_value( then_() );
                    }
                  }
                  catch( ... ) {
                    p.set_exception( std::current_exception() );
                  }
                }
              }
            );
            return f;
          }
        }
        else if constexpr ( std::is_invocable_v< F, std::exception_ptr > ) {
          using next_type = std::remove_cvref_t< decltype( then_( std::exception_ptr() ) ) >;
          if constexpr ( is_future_v< next_type > ) {
            using next_type = unwrap_t< next_type >;
            promise< next_type > p;
            p.link_weak( shared );
            auto f = p.get_future();
            if( raw ) raw->set_then(
              [then_=std::forward<F>( then_ ),p=std::move( p )] ( std::exception_ptr v ) mutable {
                try {
                  auto f2 = then_( v );
                  p.link_weak( f2.shared );
                  if constexpr ( std::is_same_v< next_type, void > ) {
                    f2.set_then(
                      [p=std::move( p )]( std::exception_ptr v ) mutable {
                        if( v ) {
                          p.set_exception( v );
                        }
                        else {
                          p.set_value();
                        }
                      }
                    );
                  }
                  else {
                    f2.set_then(
                      [p=std::move( p )]( std::variant< std::exception_ptr, next_type > &&v ) mutable {
                        if( v.index() == 0u ) {
                          p.set_exception( std::get< std::exception_ptr >( v ) );
                        }
                        else {
                          p.set_value( std::get< next_type >( v ) );
                        }
                      }
                    );
                  }
                }
                catch( ... ) {
                  p.set_exception( std::current_exception() );
                }
              }
            );
            return f;
          }
          else {
            promise< next_type > p;
            p.link_weak( shared );
            auto f = p.get_future();
            if( raw ) raw->set_then(
              [then_=std::forward<F>( then_ ),p=std::move( p )] ( std::exception_ptr v ) mutable {
                try {
                  if constexpr ( std::is_same_v< next_type, void > ) {
                    then_( v );
                    p.set_value();
                  }
                  else {
                    p.set_value( then_( v ) );
                  }
                }
                catch( ... ) {
                  p.set_exception( std::current_exception() );
                }
              }
            );
            return f;
          }
        }
        else {
          static_assert( std::is_invocable_v< F >, "future::then : function is not invocable in supported forms." );
        }
      }
      else {
        if constexpr ( std::is_invocable_v< F, T&& > ) {
          using next_type = std::remove_cvref_t< decltype( then_( std::declval< T&& >() ) ) >;
          if constexpr ( is_future_v< next_type > ) {
            using next_type = unwrap_t< next_type >;
            promise< next_type > p;
            auto f = p.get_future();
            p.link_weak( shared );
            if( raw ) raw->set_then(
              [then_=std::forward<F>( then_ ),p=std::move( p )] ( std::variant< std::exception_ptr, T > &&v ) mutable {
                if( v.index() == 0u ) {
                  p.set_exception( std::get< std::exception_ptr >( v ) );
                }
                else {
                  try {
                    auto f2 = then_( std::move( std::get< T >( v ) ) );
                    p.link_weak( f2.shared );
                    if constexpr ( std::is_same_v< next_type, void > ) {
                      f2.set_then(
                        [p=std::move( p )]( std::exception_ptr v ) mutable {
                          if( v ) {
                            p.set_exception( v );
                          }
                          else {
                            p.set_value();
                          }
                        }
                      );
                    }
                    else {
                      f2.set_then(
                        [p=std::move( p )]( std::variant< std::exception_ptr, next_type > &&v ) mutable {
                          if( v.index() == 0u ) {
                            p.set_exception( std::get< std::exception_ptr >( v ) );
                          }
                          else {
                            p.set_value( std::move( std::get< next_type >( v ) ) );
                          }
                        }
                      );
                    }
                  }
                  catch( ... ) {
                    p.set_exception( std::current_exception() );
                  }
                }
              }
            );
            return f;
          }
          else {
            promise< next_type > p;
            p.link_weak( shared );
            auto f = p.get_future();
            if( raw ) raw->set_then(
              [then_=std::forward<F>( then_ ),p=std::move( p )] ( std::variant< std::exception_ptr, T > &&v ) mutable {
                if( v.index() == 0u ) {
                  p.set_exception( std::get< std::exception_ptr >( v ) );
                }
                else {
                  try {
                    if constexpr ( std::is_same_v< next_type, void > ) {
                      then_( std::move( std::get< T >( v ) ) );
                      p.set_value();
                    }
                    else {
                      p.set_value( then_( std::move( std::get< T >( v ) ) ) );
                    }
                  }
                  catch( ... ) {
                    p.set_exception( std::current_exception() );
                  }
                }
              }
            );
            return f;
          }
        }
        else if constexpr ( std::is_invocable_v< F, std::variant< std::exception_ptr, T >&& > ) {
          using next_type = std::remove_cvref_t< decltype( then_( std::declval< std::variant< std::exception_ptr, T >&& >() ) ) >;
          if constexpr ( is_future_v< next_type > ) {
            using next_type = unwrap_t< next_type >;
            promise< next_type > p;
            auto f = p.get_future();
            p.link_weak( shared );
            if( raw ) raw->set_then(
              [then_=std::forward<F>( then_ ),p=std::move( p )] ( std::variant< std::exception_ptr, T > &&v ) mutable {
                try {
                  auto f2 = then_( std::move( v ) );
                  p.link_weak( f2.shared );
                  if constexpr ( std::is_same_v< next_type, void > ) {
                    f2.set_then(
                      [p=std::move( p )]( std::exception_ptr v ) mutable {
                        if( v ) {
                          p.set_exception( v );
                        }
                        else {
                          p.set_value();
                        }
                      }
                    );
                  }
                  else {
                    f2.set_then(
                      [p=std::move( p )]( std::variant< std::exception_ptr, next_type > &&v ) mutable {
                        if( v.index() == 0u ) {
                          p.set_exception( std::get< std::exception_ptr >( v ) );
                        }
                        else {
                          p.set_value( std::move( std::get< next_type >( v ) ) );
                        }
                      }
                    );
                  }
                }
                catch( ... ) {
                  p.set_exception( std::current_exception() );
                }
              }
            );
            return f;
          }
          else {
            promise< next_type > p;
            p.link_weak( shared );
            auto f = p.get_future();
            if( raw ) raw->set_then(
              [then_=std::forward<F>( then_ ),p=std::move( p )] ( std::variant< std::exception_ptr, T > &&v ) mutable {
                try {
                  if constexpr ( std::is_same_v< next_type, void > ) {
                    then_( std::move( v ) );
                    p.set_value();
                  }
                  else {
                    p.set_value( then_( std::move( v ) ) );
                  }
                }
                catch( ... ) {
                  p.set_exception( std::current_exception() );
                }
              }
            );
            return f;
          }
        }
        else {
          static_assert( std::is_invocable_v< F >, "future::then : function is not invocable in supported forms." );
        }
      }
    }
    template< typename F >
    auto rescue( F &&rescue_ ) {
      using next_type = T;
      promise< next_type > p;
      p.link_weak( shared );
      auto f = p.get_future();
      if( raw ) {
        if constexpr ( std::is_same_v< T, void > ) {
          raw->set_then(
            [rescue_=std::forward<F>( rescue_ ),p=std::move( p )] ( std::exception_ptr v ) mutable {
              if( v ) {
                try {
                  rescue_( v );
                  p.set_value();
                }
                catch( ... ) {
                  p.set_exception( std::current_exception() );
                }
              }
              else {
                p.set_value();
              }
            }
          );
        }
        else {
          raw->set_then(
            [rescue_=std::forward<F>( rescue_ ),p=std::move( p )] ( std::variant< std::exception_ptr, T > &&v ) mutable {
              if( v.index() == 0u ) {
                try {
                  p.set_value( rescue_( std::get< std::exception_ptr >( v ) ) );
                }
                catch( ... ) {
                  p.set_exception( std::current_exception() );
                }
              }
              else {
                p.set_value( std::move( std::get< T >( v ) ) );
              }
            }
          );
        }
      }
      return f;
    }
    void cancel() {
      if( raw ) raw->cancel();
    }
    template< typename F >
    void set_cancel( F &&f ) {
      raw->set_cancel( [f=std::forward< F >( f )]() mutable { f(); } );
    }
  private:
    template< typename F >
    void set_then( F &&f ) {
      if( raw ) raw->set_then( std::forward< F >( f ) );
    }
    std::shared_ptr< shared_type > shared;
    shared_type *raw;
  };

  template< typename T >
  class promise {
    template< typename U > friend class future;
    using shared_type = future_shared_resource< T >;
    using cancel_type = typename shared_type::cancel_type;
  public:
    promise() : shared( std::make_shared< shared_type >() ) {
      raw = shared.get();
    }
    [[nodiscard]] future< T > get_future() {
      return future< T >( shared );
    }
    void set_value( const T &v ) {
      raw->set_value( T( v ) );
    }
    void set_value( T &&v ) {
      raw->set_value( std::move( v ) );
    }
    void set_exception( std::exception_ptr p ) {
      raw->set_exception( p );
    }
    template< typename F >
    void set_cancel( F &&f ) {
      raw->set_cancel( [f=std::forward< F >( f )]() mutable { f(); } );
    }
  private:
    template< typename U >
    void link_weak( std::shared_ptr< U > &f ) {
      raw->set_cancel( [w=std::weak_ptr< U >( f )]() mutable {
        if( auto f = w.lock() )
          f->cancel();
      } );
    }
    template< typename U >
    void link( std::shared_ptr< U > &f ) {
      raw->set_cancel( [f=f]() mutable {
        f->cancel();
      } );
    }
    std::shared_ptr< shared_type > shared;
    shared_type *raw;
  };
  template<>
  class promise< void > {
    template< typename U > friend class future;
    using shared_type = future_shared_resource< void >;
    using cancel_type = typename shared_type::cancel_type;
  public:
    promise() : shared( std::make_shared< shared_type >() ) {
      raw = shared.get();
    }
    [[nodiscard]] future< void > get_future() {
      return future< void >( shared );
    }
    void set_value() {
      raw->set_value();
    }
    void set_exception( std::exception_ptr p ) {
      raw->set_exception( p );
    }
    template< typename F >
    void set_cancel( F &&f ) {
      raw->set_cancel( [f=std::forward< F >( f )]() mutable { f(); } );
    }
  private:
    template< typename U >
    void link_weak( std::shared_ptr< U > &f ) {
      raw->set_cancel( [w=std::weak_ptr< U >( f )]() mutable {
        if( auto f = w.lock() )
          f->cancel();
      } );
    }
    template< typename U >
    void link( std::shared_ptr< U > &f ) {
      raw->set_cancel( [f=f]() mutable {
        f->cancel();
      } );
    }
    std::shared_ptr< shared_type > shared;
    shared_type *raw;
  };

  template< typename ...T >
  struct future_or_result_type {
  private:
    template< typename U >
    struct lift_variant :
      public gct::type_traits::lift< std::variant, U > {};
    template< typename U >
    struct unlift_future :
      public gct::type_traits::unlift< gct::future, U > {};
    constexpr static bool has_void =
      gct::type_traits::count_sequence_v<
        gct::type_traits::for_each_t<
          unlift_future,
          gct::type_traits::type_sequence< T... >
        >,
        void
      >;
    using lifted = gct::type_traits::for_each_t<
      lift_variant,
      gct::type_traits::filter_sequence_t<
        gct::type_traits::for_each_t<
          unlift_future,
          gct::type_traits::type_sequence< T... >
        >,
        void
      >
    >;
    using opt = gct::type_traits::if_t<
      has_void,
      gct::type_traits::concat_sequence_t<
        gct::type_traits::type_sequence< std::nullopt_t >,
        lifted
      >,
      lifted
    >;
  public:
    using type = gct::type_traits::if_t<
      gct::type_traits::sequence_size_v< lifted > == 0,
      void,
      gct::type_traits::unlift_t<
        std::variant,
        typename gct::type_traits::chlift_t<
          gct::type_traits::type_sequence,
          gct::type_traits::merge_sequence,
          opt
        >::type
      >
    >;
  };
  template< typename ...T >
  using future_or_result_type_t = typename future_or_result_type< T... >::type;

  template< typename T, typename U >
  struct future_or_shared_state {
    future_or_shared_state(
      T &&l_,
      U &&r_
    ) : l( std::move( l_ ) ), r( std::move( r_ ) ), count( 0u ) {}
    T l;
    U r;
    std::atomic< unsigned int > count;
    promise< future_or_result_type_t< T, U > > p;
  };

  template< typename T, typename U >
  [[nodiscard]] future< future_or_result_type_t< future< T >, future< U > > >
  operator|( future< T > &&l, future< U > &&r ) {
    using result_t = future_or_result_type_t< future< T >, future< U > >;
    auto shared = std::make_shared< future_or_shared_state< future< T >, future< U > > >(
      std::move( l ),
      std::move( r )
    );
    if constexpr ( std::is_void_v< T > ) {
      shared->l.then(
        [shared=shared]( std::exception_ptr v ) {
          if( shared->count++ == 0u ) {
            if( v ) {
              shared->p.set_exception( v );
            }
            else {
              if constexpr ( std::is_void_v< result_t > ) {
                shared->p.set_value();
              }
              else {
                shared->p.set_value( std::nullopt );
              }
            }
            shared->r.cancel();
          }
        }
      );
    }
    else {
      shared->l.then(
        [shared=shared]( std::variant< std::exception_ptr, T > &&v ) {
          if( shared->count++ == 0u ) {
            if( v.index() == 0u ) {
              shared->p.set_exception( std::get< std::exception_ptr >( v ) );
            }
            else {
              shared->p.set_value( std::move( std::get< T >( v ) ) );
            }
            shared->r.cancel();
          }
        }
      );
    }
    if constexpr ( std::is_void_v< U > ) {
      shared->r.then(
        [shared=shared]( std::exception_ptr v ) {
          if( shared->count++ == 0u ) {
            if( v ) {
              shared->p.set_exception( v );
            }
            else {
              if constexpr ( std::is_void_v< result_t > ) {
                shared->p.set_value();
              }
              else {
                shared->p.set_value( std::nullopt );
              }
            }
            shared->l.cancel();
          }
        }
      );
    }
    else {
      shared->r.then(
        [shared=shared]( std::variant< std::exception_ptr, U > &&v ) {
          if( shared->count++ == 0u ) {
            if( v.index() == 0u ) {
              shared->p.set_exception( std::get< std::exception_ptr >( v ) );
            }
            else {
              shared->p.set_value( std::move( std::get< U >( v ) ) );
            }
            shared->l.cancel();
          }
        }
      );
    }
    shared->p.set_cancel( [shared=shared]() {
      shared->l.cancel();
      shared->r.cancel();
    } );
    auto f = shared->p.get_future();
    return f;
  }

  template< typename ...T >
  struct future_and_result_type {
  private:
    template< typename U >
    struct unlift_future :
      public gct::type_traits::unlift< gct::future, U > {};
    template< typename U >
    struct lift_tuple :
      public gct::type_traits::lift< std::tuple, U > {};

    using lifted =
      typename gct::type_traits::for_each<
        lift_tuple,
        gct::type_traits::filter_sequence_t<
          gct::type_traits::for_each_t<
            unlift_future,
            gct::type_traits::type_sequence< T... >
          >,
          void
        >
      >::type;
  public:
    using type = gct::type_traits::if_t<
      gct::type_traits::sequence_size_v< lifted > == 0,
      void,
      gct::type_traits::unlift_t<
        std::tuple,
        typename gct::type_traits::chlift_t<
          gct::type_traits::type_sequence,
          gct::type_traits::concat_sequence,
          lifted
        >::type
      >
    >;
  };
  template< typename ...T >
  using future_and_result_type_t = typename future_and_result_type< T... >::type;

  template< typename T, typename U, typename Enable = void >
  struct future_and_state {};
  template< typename T, typename U >
  struct future_and_state<
    T, U, std::enable_if_t< !std::is_void_v< T > && !std::is_void_v< U > >
  > {
    using result_t = future_and_result_type_t< future< T >, future< U > >;
    future_and_state(
      future< T > &&lf_,
      future< U > &&rf_
    ) : count( 0 ), lf( std::move( lf_ ) ), rf( std::move( rf_ ) ) {}
    std::atomic< std::size_t > count;
    std::optional< T > lv;
    std::optional< U > rv;
    future< T > lf;
    future< U > rf;
    promise< result_t > p;
  };
  template< typename T, typename U >
  struct future_and_state<
    T, U, std::enable_if_t< !std::is_void_v< T > && std::is_void_v< U > >
  > {
    using result_t = future_and_result_type_t< future< T >, future< U > >;
    future_and_state(
      future< T > &&lf_,
      future< U > &&rf_
    ) : count( 0 ), lf( std::move( lf_ ) ), rf( std::move( rf_ ) ) {}
    std::atomic< std::size_t > count;
    std::optional< T > lv;
    future< T > lf;
    future< U > rf;
    promise< result_t > p;
  };
  template< typename T, typename U >
  struct future_and_state<
    T, U, std::enable_if_t< std::is_void_v< T > && !std::is_void_v< U > >
  > {
    using result_t = future_and_result_type_t< future< T >, future< U > >;
    future_and_state(
      future< T > &&lf_,
      future< U > &&rf_
    ) : count( 0 ), lf( std::move( lf_ ) ), rf( std::move( rf_ ) ) {}
    std::atomic< std::size_t > count;
    std::optional< U > rv;
    future< T > lf;
    future< U > rf;
    promise< result_t > p;
  };
  template< typename T, typename U >
  struct future_and_state<
    T, U, std::enable_if_t< std::is_void_v< T > && std::is_void_v< U > >
  > {
    using result_t = future_and_result_type_t< future< T >, future< U > >;
    future_and_state(
      future< T > &&lf_,
      future< U > &&rf_
    ) : count( 0 ), lf( std::move( lf_ ) ), rf( std::move( rf_ ) ) {}
    std::atomic< std::size_t > count;
    future< T > lf;
    future< U > rf;
    promise< result_t > p;
  };

  template< typename T, typename U >
  [[nodiscard]] auto operator&( future< T > &&l, future< U > &&r ) ->
    std::enable_if_t<
      !std::is_void_v< T > && !std::is_void_v< U >,
      future< future_and_result_type_t< future< T >, future< U > > >
    > {
    using result_t = future_and_result_type_t< future< T >, future< U > >;
    auto state = std::make_shared< future_and_state< T, U > >(
      std::move( l ), std::move( r )
    );
    if constexpr ( !std::is_void_v< T > ) {
      state->lf.then(
        [state=state]( std::variant< std::exception_ptr, T > &&v ) {
          if( v.index() == 0u ) {
            state->count = 3u;
            state->p.set_exception( std::get< std::exception_ptr >( v ) );
            state->rf.cancel();
            return;
          }
          state->lv = std::move( std::get< T >( v ) );
          if( ++state->count == 2u ) {
            if constexpr ( std::is_same_v< T, result_t > ) {
              state->p.set_value( std::move( *state->lv ) );
            }
            else {
              state->p.set_value( std::tuple_cat( tuplise( std::move( *state->lv ) ), tuplise( std::move( *state->rv ) ) ) );
            }
          }
        }
      );
    }
    else {
      state->lf.then(
        [state=state]( std::exception_ptr v ) {
          if( v ) {
            state->count = 3u;
            state->p.set_exception( v );
            state->rf.cancel();
            return;
          }
          if( ++state->count == 2u ) {
            if constexpr ( std::is_void_v< result_t > ) {
              state->p.set_value();
            }
            else if constexpr ( std::is_same_v< U, result_t > ) {
              state->p.set_value( std::move( *state->rv ) );
            }
          }
        }
      );
    }
    if constexpr ( !std::is_void_v< U > ) {
      state->rf.then(
        [state=state]( std::variant< std::exception_ptr, U > &&v ) {
          if( v.index() == 0u ) {
            state->count = 3u;
            state->p.set_exception( std::get< std::exception_ptr >( v ) );
            state->lf.cancel();
            return;
          }
          state->rv = std::move( std::get< U >( v ) );
          if( ++state->count == 2u ) {
            if constexpr ( std::is_same_v< U, result_t > ) {
              state->p.set_value( std::move( *state->rv ) );
            }
            else {
              state->p.set_value( std::tuple_cat( tuplise( std::move( *state->lv ) ), tuplise( std::move( *state->rv ) ) ) );
            }
          }
        }
      );
    }
    else {
      state->rf.then(
        [state=state]( std::exception_ptr v ) {
          if( v ) {
            state->count = 3u;
            state->p.set_exception( v );
            state->lf.cancel();
            return;
          }
          if( ++state->count == 2u ) {
            if constexpr ( std::is_void_v< result_t > ) {
              state->p.set_value();
            }
            else if constexpr ( std::is_same_v< T, result_t > ) {
              state->p.set_value( std::move( *state->lv ) );
            }
          }
        }
      );
    }
    state->p.set_cancel( [state=state]() {
      state->count = 3u;
      state->lf.cancel();
      state->rf.cancel();
    } );
    auto f = state->p.get_future();
    return f;
  }
}

#endif

