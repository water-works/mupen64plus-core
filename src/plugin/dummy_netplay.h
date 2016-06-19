#if !defined(DUMMY_NETPLAY_H)
#define DUMMY_NETPLAY_H

#include "api/m64p_plugin.h"

extern m64p_error dummynetplay_PluginGetVersion(m64p_plugin_type *PluginType,
                                                int *PluginVersion,
                                                int *APIVersion,
                                                const char **PluginNamePtr,
                                                int *Capabilities);
extern int dummynetplay_RomOpen(void);
extern void dummynetplay_RomClosed(void);
extern int dummynetplay_InitiateNetplay(NETPLAY_INFO *netplay_info,
                                        const char *goodname, const char *md5);
extern int dummynetplay_Netplay_PutKeys(const m64p_netplay_frame_update *updates,
                                        int nupdates);
extern int dummynetplay_Netplay_GetKeys(m64p_netplay_frame_update *update);

#endif /* DUMMY_NETPLAY_H */
