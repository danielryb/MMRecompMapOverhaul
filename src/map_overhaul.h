#ifndef __MAP_OVERHAUL__
#define __MAP_OVERHAUL__

#include "modding.h"
#include "global.h"

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

extern Vec2f map_overhaul_playerMinimapUnitPos;

void map_overhaul_reload_minimap_data(void);

void map_overhaul_update_map(void);
bool map_overhaul_minimap_unit_to_world_map_position(Vec2f* minimapPos, Vec2f* dest);

#endif /* __MAP_OVERHAUL__ */