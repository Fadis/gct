#include <exception>
#include <memory>
#include <vector>
#include <iostream>
#include <utility>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <gct/input/libinput.hpp>
#include <gct/input/device_monitor.hpp>

namespace gct::input {

void device_monitor( std::vector< std::string > &dest, libinput_event *event ) {
  auto device = libinput_event_get_device( event );
  auto name = libinput_device_get_name( device );
  auto sname = libinput_device_get_sysname( device );
  auto seat = libinput_device_get_seat( device );
  auto pname = libinput_seat_get_physical_name( seat );
  auto lname = libinput_seat_get_logical_name( seat );
  auto is_keyboard = libinput_device_has_capability( device, LIBINPUT_DEVICE_CAP_KEYBOARD );
  auto is_mouse = libinput_device_has_capability( device, LIBINPUT_DEVICE_CAP_POINTER );
  auto is_touch = libinput_device_has_capability( device, LIBINPUT_DEVICE_CAP_TOUCH );
  auto is_tablet_tool = libinput_device_has_capability( device, LIBINPUT_DEVICE_CAP_TABLET_TOOL );
  auto is_tablet_pad = libinput_device_has_capability( device, LIBINPUT_DEVICE_CAP_TABLET_PAD );
  auto is_gesture = libinput_device_has_capability( device, LIBINPUT_DEVICE_CAP_GESTURE );
  auto is_switch = libinput_device_has_capability( device, LIBINPUT_DEVICE_CAP_SWITCH );
  const auto type = libinput_event_get_type( event );
  std::string temp;
  if( is_keyboard ) {
    temp += "keyboard";
  }
  if( is_mouse ) {
    temp += "mouse";
  }
  if( is_touch ) {
    temp += "touch panel";
  }
  if( is_tablet_tool ) {
    temp += "tablet tool";
  }
  if( is_tablet_pad ) {
    temp += "tablet pad";
  }
  if( is_gesture ) {
    temp += "gesture";
  }
  if( is_switch ) {
    temp += "switch";
  }
  if( type == LIBINPUT_EVENT_DEVICE_ADDED ) {
    temp += " added : ";
  }
  else if( type == LIBINPUT_EVENT_DEVICE_REMOVED ) {
    temp += " removed : ";
  }
  temp += name;
  temp += " : ";
  temp += sname;
  temp += " : ";
  temp += pname;
  temp += " : ";
  temp += lname;
  dest.push_back( std::move( temp ) );
}

}

