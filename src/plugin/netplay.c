#include "plugin/plugin.h"

#include "./api/callbacks.h"

int netplay_enabled;

int netplay_channel_for_port(int port) {
  if (!netplay_enabled) {
    return port;
  } else {
    return NetplayControls[port].Channel;
  }
}

int netplay_remap_ports(int* channels)
{
  return 1;
}
