#ifndef GCT_INPUT_DEVICE_MONITOR_HPP
#define GCT_INPUT_DEVICE_MONITOR_HPP

#include <string>
#include <vector>

struct libinput_event;

namespace gct::input {

void device_monitor( std::vector< std::string > &dest, libinput_event *event );

}

#endif

