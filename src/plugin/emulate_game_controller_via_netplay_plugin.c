#include "emulate_game_controller_via_netplay_plugin.h"

#include "api/callbacks.h"
#include "plugin/netplay.h"
#include "plugin/plugin.h"

int egcvnp_is_connected(void* opaque, enum pak_type* pak) {
  int channel = *(int*)opaque;

  return NetplayControls[channel].Present;
}

struct button_cache {
  int frame;
  BUTTONS buttons;
};
struct button_cache button_cache[4] = {{0}, {0}, {0}, {0}};

static inline int should_refresh_button_cache(int port) {
  int i;

  if (!NetplayControls[port].Present) {
    return 0;
  }

  int max_frame = -1;
  for (i = 0; i < 4; ++i) {
    if (NetplayControls[i].Present) {
      if (max_frame < button_cache[i].frame) {
        max_frame = button_cache[i].frame;
      }
    }
  }

  return max_frame == button_cache[port].frame;
}

static inline void refresh_button_cache() {
  int i;
  m64p_netplay_frame_update updates[4];
  int n_updates = 0;
  BUTTONS update_buttons[4];

  // First put all local ports onto the network.
  for (i = 0; i < 4; ++i) {
    const int frame_num = button_cache[i].frame;

    if (NetplayControls[i].Present && !NetplayControls[i].Remote) {
      update_buttons[i].Value = egcvip_get_input(&NetplayControls[i].Channel);

      m64p_netplay_frame_update* update = &updates[n_updates];
      update->port = i;
      update->frame = frame_num;
      update->buttons = &update_buttons[i];

      n_updates += 1;
    }
  }

  if (n_updates > 0 && !netplay.putKeys(updates, n_updates)) {
    DebugMessage(M64MSG_ERROR, "Netplay PutKeys failed");
  }

  // Extract the button values from the netplay plugin.
  for (i = 0; i < 4; ++i) {
    if (NetplayControls[i].Present) {
      m64p_netplay_frame_update update;
      update.port = i;
      update.frame = button_cache[i].frame;
      update.buttons = &button_cache[i].buttons;

      if (!netplay.getKeys(&update)) {
        DebugMessage(M64MSG_ERROR, "Netplay GetKeys failed");
      }
    }
  }
}

uint32_t egcvnp_get_input(void* opaque) {
  int port = (*(int*)opaque);

  if (!NetplayControls[port].Present) {
    return 0;
  }

  if (should_refresh_button_cache(port)) {
    refresh_button_cache();
  }

  button_cache[port].frame++;

  return button_cache[port].buttons.Value;
}
