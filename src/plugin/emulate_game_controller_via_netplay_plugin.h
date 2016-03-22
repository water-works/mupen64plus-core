#ifndef M64P_PLUGIN_EMULATE_GAME_CONTROLLER_VIA_NETPLAY_PLUGIN_H
#define M64P_PLUGIN_EMULATE_GAME_CONTROLLER_VIA_NETPLAY_PLUGIN_H

#include <stdint.h>

#include "emulate_game_controller_via_input_plugin.h"

int egcvnp_is_connected(void* opaque, enum pak_type* pak);

uint32_t egcvnp_get_input(void* opaque);

#endif  // M64P_PLUGIN_EMULATE_GAME_CONTROLLER_VIA_NETPLAY_PLUGIN_H
