#ifndef PLUGIN_NETPLAY_H_
#define PLUGIN_NETPLAY_H_

#include "api/m64p_plugin.h"

extern int netplay_enabled;

// Returns the channel assigned to the port. If the netplay plugin is disabled,
// the return value will be the same as the input.
int netplay_channel_for_port(int port);

// Request a port remapping from the netplay server, or do nothing if netplay is
// disabled. This method blocks until the server indicates the game is ready to
// start.
int netplay_remap_ports();

#endif  // PLUGIN_NETPLAY_H_
