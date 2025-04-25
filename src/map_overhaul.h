#ifndef __MAP_OVERHAUL__
#define __MAP_OVERHAUL__

#include "modding.h"
#include "global.h"
#include "rt64_extended_gbi.h"

extern s16 sWorldMapCursorsRectLeft[REGION_MAX];
extern s16 sWorldMapCursorsRectTop[REGION_MAX];

extern f32 sWorldMapCursorsX[REGION_MAX];
extern f32 sWorldMapCursorsY[REGION_MAX];

extern f32 sOwlWarpWorldMapCursorsX[OWL_WARP_MAX - 1];
extern f32 sOwlWarpWorldMapCursorsY[OWL_WARP_MAX - 1];

#include "overlays/kaleido_scope/ovl_kaleido_scope/z_kaleido_scope.h"

extern s16 sVtxPageMapWorldQuadsX[VTX_PAGE_MAP_WORLD_QUADS];
extern s16 sVtxPageMapWorldQuadsY[VTX_PAGE_MAP_WORLD_QUADS];

#include "constants.h"

extern f32 new_sVtxPageMapWorldQuadsX[];
extern f32 new_sVtxPageMapWorldQuadsY[];

extern Vec2f map_overhaul_playerWorldMapPos;

void map_overhaul_reload_minimap_data(PlayState *play /*TMP*/, s8 room);

void map_overhaul_get_face_position(size_t i, Vec2f* pos);
void map_overhul_update_cursor_position(PlayState *play);

bool map_overhaul_minimap_unit_to_world_map_position(Vec2f* minimapPos, Vec2f* dest);
bool map_overhaul_calculate_world_map_position(PlayState* play, Vec3f* pos, Vec2f* dest);

RECOMP_IMPORT("*", s32 recomp_get_analog_cam_enabled());
RECOMP_IMPORT("*", void recomp_get_camera_inputs(float* x, float* y));

RECOMP_IMPORT("*", double recomp_get_config_double(const char* key));
RECOMP_IMPORT("*", u32 recomp_get_config_u32(const char* key));

typedef enum {
    MAP_OVERLAY_MODE_PROGRESSIVE,
    MAP_OVERLAY_MODE_REALTIME,
    MAP_OVERLAY_MODE_COMBINED,
    MAP_OVERLAY_MODE_STATIC,
} MapOverlayMode;

#define CFG_MAP_OVERLAY_MODE ((MapOverlayMode)recomp_get_config_u32("map_overlay_mode"))

#ifdef DEBUG
RECOMP_IMPORT("*", void recomp_printf(char* temp, ...));

void map_overhaul_print_minimap_params(PlayState* play);
#endif

#endif /* __MAP_OVERHAUL__ */