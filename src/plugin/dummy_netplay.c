#include <stdlib.h>

#include "api/callbacks.h"
#include "api/m64p_plugin.h"
#include "dummy_netplay.h"
#include "plugin/plugin.h"

m64p_error dummynetplay_PluginGetVersion(m64p_plugin_type *PluginType,
                                         int *PluginVersion, int *APIVersion,
                                         const char **PluginNamePtr,
                                         int *Capabilities)
{
  if (PluginType != NULL) *PluginType = M64PLUGIN_NETPLAY;

  if (PluginVersion != NULL) *PluginVersion = 0x00010000;

  if (APIVersion != NULL) *APIVersion = NETPLAY_API_VERSION;

  if (PluginNamePtr != NULL) *PluginNamePtr = "Mupen64Plus-NoNetplay";

  if (Capabilities != NULL) *Capabilities = 0;

  return M64ERR_SUCCESS;
}

int dummynetplay_RomOpen(void) { return 1; }

void dummynetplay_RomClosed(void) {}

int dummynetplay_InitiateNetplay(NETPLAY_INFO* netplay_info)
{
  return 0;
}

int dummynetplay_Netplay_PutKeys(const m64p_netplay_frame_update *updates, int nupdates)
{
  return 1;
}

int dummynetplay_Netplay_GetKeys(m64p_netplay_frame_update *update)
{
  return 1;
}
