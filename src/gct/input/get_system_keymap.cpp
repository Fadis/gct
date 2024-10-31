#include <string>
#include <gct/dbus/dbus.hpp>
#include <gct/sched/wait.hpp>
#include <gct/sched/epoll_notifier.hpp>
#include <gct/input/get_system_keymap.hpp>

namespace gct::input {

namespace {
#ifdef GCT_SDBUSCPP_STRONG_TYPE
  using error_type = std::optional< sdbus::Error >;
#else
  using error_type = sdbus::Error*;
#endif
}

xkb_rule_names get_ssytem_keymap() {
#ifdef GCT_SDBUSCPP_STRONG_TYPE
  auto dbus_locale = sdbus::createProxy( sdbus::ServiceName("org.freedesktop.locale1"), sdbus::ObjectPath( "/org/freedesktop/locale1") );
#else
  auto dbus_locale = sdbus::createProxy( "org.freedesktop.locale1", "/org/freedesktop/locale1" );
#endif
  auto [layout,model,variant,options] = sched::wait(
    gct::dbus::call< error_type, sdbus::Variant >(
      dbus_locale->callMethodAsync( "Get" )
        .onInterface( "org.freedesktop.DBus.Properties" )
        .withArguments( "org.freedesktop.locale1", "X11Layout" )
    ) &
    gct::dbus::call< error_type, sdbus::Variant >(
      dbus_locale->callMethodAsync( "Get" )
        .onInterface( "org.freedesktop.DBus.Properties" )
        .withArguments( "org.freedesktop.locale1", "X11Model" )
    ) &
    gct::dbus::call< error_type, sdbus::Variant >(
      dbus_locale->callMethodAsync( "Get" )
        .onInterface( "org.freedesktop.DBus.Properties" )
        .withArguments( "org.freedesktop.locale1", "X11Variant" )
    ) &
    gct::dbus::call< error_type, sdbus::Variant >(
      dbus_locale->callMethodAsync( "Get" )
        .onInterface( "org.freedesktop.DBus.Properties" )
        .withArguments( "org.freedesktop.locale1", "X11Options" )
    )
  );
  const auto layout_str = layout.get< std::string >();
  const auto model_str = model.get< std::string >();
  const auto variant_str = variant.get< std::string >();
  const auto options_str = options.get< std::string >();
  xkb_rule_names names;
  names.rules = nullptr;
  names.model = model_str.c_str();
  names.layout = layout_str.c_str();
  names.variant = variant_str.c_str();
  names.options = options_str.c_str();
  return names;
}

}

