#include "constants.h"

#include "modding.h"
#include "global.h"

extern s16 sWorldMapCursorsRectLeft[REGION_MAX];
extern s16 sWorldMapCursorsRectTop[REGION_MAX];

extern f32 sWorldMapCursorsX[REGION_MAX];
extern f32 sWorldMapCursorsY[REGION_MAX];

extern f32 sOwlWarpWorldMapCursorsX[OWL_WARP_MAX - 1];
extern f32 sOwlWarpWorldMapCursorsY[OWL_WARP_MAX - 1];

RECOMP_IMPORT("*", int recomp_printf(const char* fmt, ...));

#define UPDATE_ARRAY(to, from, length)                                         \
    do {                                                                       \
        for (size_t i = 0; i < length; i++) {                                  \
            to[i] = from[i];                                                   \
        }                                                                      \
    } while (false)

// Face
#define FACE_POS(NAME, AXIS) NAME ## _ ## AXIS + FACE_OFFSET_ ## AXIS

s16 new_sWorldMapCursorsRectLeft[REGION_MAX] = {
    FACE_POS(GREAT_BAY, X),        // GREAT BAY
    FACE_POS(ZORA_HALL, X),        // ZORA HALL
    FACE_POS(ROMANI_RANCH, X),     // ROMANI RANCH
    FACE_POS(DEKU_PALACE, X),      // DEKU PALACE
    FACE_POS(WOODFALL, X),         // WOODFALL
    FACE_POS(CLOCK_TOWN, X),       // CLOCK TOWN
    FACE_POS(SNOWHEAD, X),         // SNOWHEAD
    FACE_POS(IKANA_GRAVEYARD, X),  // IKANA GRAVEYARD
    FACE_POS(IKANA_CANYON, X),     // IKANA CANYON
    FACE_POS(GORON_VILLAGE, X),    // GORON VILLAGE
    FACE_POS(STONE_TOWER, X),      // STONE TOWER
};

s16 new_sWorldMapCursorsRectTop[REGION_MAX] = {
    FACE_POS(GREAT_BAY, Y),        // GREAT BAY
    FACE_POS(ZORA_HALL, Y),        // ZORA HALL
    FACE_POS(ROMANI_RANCH, Y),     // ROMANI RANCH
    FACE_POS(DEKU_PALACE, Y),      // DEKU PALACE
    FACE_POS(WOODFALL, Y),         // WOODFALL
    FACE_POS(CLOCK_TOWN, Y),       // CLOCK TOWN
    FACE_POS(SNOWHEAD, Y),         // SNOWHEAD
    FACE_POS(IKANA_GRAVEYARD, Y),  // IKANA GRAVEYARD
    FACE_POS(IKANA_CANYON, Y),     // IKANA CANYON
    FACE_POS(GORON_VILLAGE, Y),    // GORON VILLAGE
    FACE_POS(STONE_TOWER, Y),      // STONE TOWER
};

// Float
#define PIXEL_TO_FLOAT(NAME, AXIS) (NAME ## _ ## AXIS - MAP_CENTER_PX_ ## AXIS) * PIXEL_TO_FLOAT_SCALE_ ## AXIS

#define GREAT_BAY_X_F       PIXEL_TO_FLOAT(GREAT_BAY, X)
#define ZORA_HALL_X_F       PIXEL_TO_FLOAT(ZORA_HALL, X)
#define ROMANI_RANCH_X_F    PIXEL_TO_FLOAT(ROMANI_RANCH, X)
#define DEKU_PALACE_X_F     PIXEL_TO_FLOAT(DEKU_PALACE, X)
#define WOODFALL_X_F        PIXEL_TO_FLOAT(WOODFALL, X)
#define CLOCK_TOWN_X_F      PIXEL_TO_FLOAT(CLOCK_TOWN, X)
#define SNOWHEAD_X_F        PIXEL_TO_FLOAT(SNOWHEAD, X)
#define IKANA_GRAVEYARD_X_F PIXEL_TO_FLOAT(IKANA_GRAVEYARD, X)
#define IKANA_CANYON_X_F    PIXEL_TO_FLOAT(IKANA_CANYON, X)
#define GORON_VILLAGE_X_F   PIXEL_TO_FLOAT(GORON_VILLAGE, X)
#define STONE_TOWER_X_F     PIXEL_TO_FLOAT(STONE_TOWER, X)

#define GREAT_BAY_Y_F       PIXEL_TO_FLOAT(GREAT_BAY, Y)
#define ZORA_HALL_Y_F       PIXEL_TO_FLOAT(ZORA_HALL, Y)
#define ROMANI_RANCH_Y_F    PIXEL_TO_FLOAT(ROMANI_RANCH, Y)
#define DEKU_PALACE_Y_F     PIXEL_TO_FLOAT(DEKU_PALACE, Y)
#define WOODFALL_Y_F        PIXEL_TO_FLOAT(WOODFALL, Y)
#define CLOCK_TOWN_Y_F      PIXEL_TO_FLOAT(CLOCK_TOWN, Y)
#define SNOWHEAD_Y_F        PIXEL_TO_FLOAT(SNOWHEAD, Y)
#define IKANA_GRAVEYARD_Y_F PIXEL_TO_FLOAT(IKANA_GRAVEYARD, Y)
#define IKANA_CANYON_Y_F    PIXEL_TO_FLOAT(IKANA_CANYON, Y)
#define GORON_VILLAGE_Y_F   PIXEL_TO_FLOAT(GORON_VILLAGE, Y)
#define STONE_TOWER_Y_F     PIXEL_TO_FLOAT(STONE_TOWER, Y)

#define OWL_WARP_GREAT_BAY_COAST_X_F    PIXEL_TO_FLOAT(OWL_WARP_GREAT_BAY_COAST, X)
#define OWL_WARP_ZORA_CAPE_X_F          PIXEL_TO_FLOAT(OWL_WARP_ZORA_CAPE, X)
#define OWL_WARP_SNOWHEAD_X_F           PIXEL_TO_FLOAT(OWL_WARP_SNOWHEAD, X)
#define OWL_WARP_MOUNTAIN_VILLAGE_X_F   PIXEL_TO_FLOAT(OWL_WARP_MOUNTAIN_VILLAGE, X)
#define OWL_WARP_CLOCK_TOWN_X_F         PIXEL_TO_FLOAT(OWL_WARP_CLOCK_TOWN, X)
#define OWL_WARP_MILK_ROAD_X_F          PIXEL_TO_FLOAT(OWL_WARP_MILK_ROAD, X)
#define OWL_WARP_WOODFALL_X_F           PIXEL_TO_FLOAT(OWL_WARP_WOODFALL, X)
#define OWL_WARP_SOUTHERN_SWAMP_X_F     PIXEL_TO_FLOAT(OWL_WARP_SOUTHERN_SWAMP, X)
#define OWL_WARP_IKANA_CANYON_X_F       PIXEL_TO_FLOAT(OWL_WARP_IKANA_CANYON, X)
#define OWL_WARP_STONE_TOWER_X_F        PIXEL_TO_FLOAT(OWL_WARP_STONE_TOWER, X)

#define OWL_WARP_GREAT_BAY_COAST_Y_F    PIXEL_TO_FLOAT(OWL_WARP_GREAT_BAY_COAST, Y)
#define OWL_WARP_ZORA_CAPE_Y_F          PIXEL_TO_FLOAT(OWL_WARP_ZORA_CAPE, Y)
#define OWL_WARP_SNOWHEAD_Y_F           PIXEL_TO_FLOAT(OWL_WARP_SNOWHEAD, Y)
#define OWL_WARP_MOUNTAIN_VILLAGE_Y_F   PIXEL_TO_FLOAT(OWL_WARP_MOUNTAIN_VILLAGE, Y)
#define OWL_WARP_CLOCK_TOWN_Y_F         PIXEL_TO_FLOAT(OWL_WARP_CLOCK_TOWN, Y)
#define OWL_WARP_MILK_ROAD_Y_F          PIXEL_TO_FLOAT(OWL_WARP_MILK_ROAD, Y)
#define OWL_WARP_WOODFALL_Y_F           PIXEL_TO_FLOAT(OWL_WARP_WOODFALL, Y)
#define OWL_WARP_SOUTHERN_SWAMP_Y_F     PIXEL_TO_FLOAT(OWL_WARP_SOUTHERN_SWAMP, Y)
#define OWL_WARP_IKANA_CANYON_Y_F       PIXEL_TO_FLOAT(OWL_WARP_IKANA_CANYON, Y)
#define OWL_WARP_STONE_TOWER_Y_F        PIXEL_TO_FLOAT(OWL_WARP_STONE_TOWER, Y)

#include "overlays/kaleido_scope/ovl_kaleido_scope/z_kaleido_scope.h"

//Dots and Owls

#define sVtxPageMapWorldQuadsOffset 15

extern s16 sVtxPageMapWorldQuadsX[VTX_PAGE_MAP_WORLD_QUADS];
extern s16 sVtxPageMapWorldQuadsY[VTX_PAGE_MAP_WORLD_QUADS];

#define DOT_QUAD_POS(NAME, QUAD, AXIS) NAME ## _ ## AXIS ## _F + QUAD ## _OFFSET_ ## AXIS

s16 new_sVtxPageMapWorldQuadsX[VTX_PAGE_MAP_WORLD_QUADS - sVtxPageMapWorldQuadsOffset] = {
    DOT_QUAD_POS(GREAT_BAY, DOT, X),                 // mapPageVtx[120] world map region Great Bay
    DOT_QUAD_POS(ZORA_HALL, DOT, X),                 // mapPageVtx[124] world map region Zora Hall
    DOT_QUAD_POS(ROMANI_RANCH, DOT, X),              // mapPageVtx[128] world map region Romani Ranch
    DOT_QUAD_POS(DEKU_PALACE, DOT, X),               // mapPageVtx[132] world map region Deku Palace
    DOT_QUAD_POS(WOODFALL, DOT, X),                  // mapPageVtx[136] world map region Woodfall
    DOT_QUAD_POS(CLOCK_TOWN, DOT, X),                // mapPageVtx[140] world map region Clock Town
    DOT_QUAD_POS(SNOWHEAD, DOT, X),                  // mapPageVtx[144] world map region Snowhead
    DOT_QUAD_POS(IKANA_GRAVEYARD, DOT, X),           // mapPageVtx[148] world map region Ikana Graveyard
    DOT_QUAD_POS(IKANA_CANYON, DOT, X),              // mapPageVtx[152] world map region Ikana Canyon
    DOT_QUAD_POS(GORON_VILLAGE, DOT, X),             // mapPageVtx[156] world map region Goron Village
    DOT_QUAD_POS(STONE_TOWER, DOT, X),               // mapPageVtx[160] world map region Stone Tower

    DOT_QUAD_POS(OWL_WARP_GREAT_BAY_COAST, OWL, X),  // mapPageVtx[164] world map owl warp Great Bay Coast
    DOT_QUAD_POS(OWL_WARP_ZORA_CAPE, OWL, X),        // mapPageVtx[168] world map owl warp Zora Cape
    DOT_QUAD_POS(OWL_WARP_SNOWHEAD, OWL, X),         // mapPageVtx[172] world map owl warp Snowhead
    DOT_QUAD_POS(OWL_WARP_MOUNTAIN_VILLAGE, OWL, X), // mapPageVtx[176] world map owl warp Mountain Village
    DOT_QUAD_POS(OWL_WARP_CLOCK_TOWN, OWL, X),       // mapPageVtx[180] world map owl warp Clock Town
    DOT_QUAD_POS(OWL_WARP_MILK_ROAD, OWL, X),        // mapPageVtx[184] world map owl warp Milk Road
    DOT_QUAD_POS(OWL_WARP_WOODFALL, OWL, X),         // mapPageVtx[188] world map owl warp Woodfall
    DOT_QUAD_POS(OWL_WARP_SOUTHERN_SWAMP, OWL, X),   // mapPageVtx[192] world map owl warp Southern Swamp
    DOT_QUAD_POS(OWL_WARP_IKANA_CANYON, OWL, X),     // mapPageVtx[196] world map owl warp Ikana Canyon
    DOT_QUAD_POS(OWL_WARP_STONE_TOWER, OWL, X),      // mapPageVtx[200] world map owl warp Stone Tower
};

s16 new_sVtxPageMapWorldQuadsY[VTX_PAGE_MAP_WORLD_QUADS - sVtxPageMapWorldQuadsOffset] = {
    DOT_QUAD_POS(GREAT_BAY, DOT, Y),                 // mapPageVtx[120] world map region Great Bay
    DOT_QUAD_POS(ZORA_HALL, DOT, Y),                 // mapPageVtx[124] world map region Zora Hall
    DOT_QUAD_POS(ROMANI_RANCH, DOT, Y),              // mapPageVtx[128] world map region Romani Ranch
    DOT_QUAD_POS(DEKU_PALACE, DOT, Y),               // mapPageVtx[132] world map region Deku Palace
    DOT_QUAD_POS(WOODFALL, DOT, Y),                  // mapPageVtx[136] world map region Woodfall
    DOT_QUAD_POS(CLOCK_TOWN, DOT, Y),                // mapPageVtx[140] world map region Clock Town
    DOT_QUAD_POS(SNOWHEAD, DOT, Y),                  // mapPageVtx[144] world map region Snowhead
    DOT_QUAD_POS(IKANA_GRAVEYARD, DOT, Y),           // mapPageVtx[148] world map region Ikana Graveyard
    DOT_QUAD_POS(IKANA_CANYON, DOT, Y),              // mapPageVtx[152] world map region Ikana Canyon
    DOT_QUAD_POS(GORON_VILLAGE, DOT, Y),             // mapPageVtx[156] world map region Goron Village
    DOT_QUAD_POS(STONE_TOWER, DOT, Y),               // mapPageVtx[160] world map region Stone Tower

    DOT_QUAD_POS(OWL_WARP_GREAT_BAY_COAST, OWL, Y),  // mapPageVtx[164] world map owl warp Great Bay Coast
    DOT_QUAD_POS(OWL_WARP_ZORA_CAPE, OWL, Y),        // mapPageVtx[168] world map owl warp Zora Cape
    DOT_QUAD_POS(OWL_WARP_SNOWHEAD, OWL, Y),         // mapPageVtx[172] world map owl warp Snowhead
    DOT_QUAD_POS(OWL_WARP_MOUNTAIN_VILLAGE, OWL, Y), // mapPageVtx[176] world map owl warp Mountain Village
    DOT_QUAD_POS(OWL_WARP_CLOCK_TOWN, OWL, Y),       // mapPageVtx[180] world map owl warp Clock Town
    DOT_QUAD_POS(OWL_WARP_MILK_ROAD, OWL, Y),        // mapPageVtx[184] world map owl warp Milk Road
    DOT_QUAD_POS(OWL_WARP_WOODFALL, OWL, Y),         // mapPageVtx[188] world map owl warp Woodfall
    DOT_QUAD_POS(OWL_WARP_SOUTHERN_SWAMP, OWL, Y),   // mapPageVtx[192] world map owl warp Southern Swamp
    DOT_QUAD_POS(OWL_WARP_IKANA_CANYON, OWL, Y),     // mapPageVtx[196] world map owl warp Ikana Canyon
    DOT_QUAD_POS(OWL_WARP_STONE_TOWER, OWL, Y),      // mapPageVtx[200] world map owl warp Stone Tower
};

// Cursor
#define SCREEN_POS(NAME, AXIS) NAME ## _ ## AXIS ## _F * MAP_TO_SCREEN_ ## AXIS

#define GREAT_BAY_X_CURSOR       SCREEN_POS(GREAT_BAY, X)
#define ZORA_HALL_X_CURSOR       SCREEN_POS(ZORA_HALL, X)
#define ROMANI_RANCH_X_CURSOR    SCREEN_POS(ROMANI_RANCH, X)
#define DEKU_PALACE_X_CURSOR     SCREEN_POS(DEKU_PALACE, X)
#define WOODFALL_X_CURSOR        SCREEN_POS(WOODFALL, X)
#define CLOCK_TOWN_X_CURSOR      SCREEN_POS(CLOCK_TOWN, X)
#define SNOWHEAD_X_CURSOR        SCREEN_POS(SNOWHEAD, X)
#define IKANA_GRAVEYARD_X_CURSOR SCREEN_POS(IKANA_GRAVEYARD, X)
#define IKANA_CANYON_X_CURSOR    SCREEN_POS(IKANA_CANYON, X)
#define GORON_VILLAGE_X_CURSOR   SCREEN_POS(GORON_VILLAGE, X)
#define STONE_TOWER_X_CURSOR     SCREEN_POS(STONE_TOWER, X)

#define GREAT_BAY_Y_CURSOR       SCREEN_POS(GREAT_BAY, Y)
#define ZORA_HALL_Y_CURSOR       SCREEN_POS(ZORA_HALL, Y)
#define ROMANI_RANCH_Y_CURSOR    SCREEN_POS(ROMANI_RANCH, Y)
#define DEKU_PALACE_Y_CURSOR     SCREEN_POS(DEKU_PALACE, Y)
#define WOODFALL_Y_CURSOR        SCREEN_POS(WOODFALL, Y)
#define CLOCK_TOWN_Y_CURSOR      SCREEN_POS(CLOCK_TOWN, Y)
#define SNOWHEAD_Y_CURSOR        SCREEN_POS(SNOWHEAD, Y)
#define IKANA_GRAVEYARD_Y_CURSOR SCREEN_POS(IKANA_GRAVEYARD, Y)
#define IKANA_CANYON_Y_CURSOR    SCREEN_POS(IKANA_CANYON, Y)
#define GORON_VILLAGE_Y_CURSOR   SCREEN_POS(GORON_VILLAGE, Y)
#define STONE_TOWER_Y_CURSOR     SCREEN_POS(STONE_TOWER, Y)

#define OWL_WARP_GREAT_BAY_COAST_X_CURSOR   SCREEN_POS(OWL_WARP_GREAT_BAY_COAST, X)
#define OWL_WARP_ZORA_CAPE_X_CURSOR         SCREEN_POS(OWL_WARP_ZORA_CAPE, X)
#define OWL_WARP_SNOWHEAD_X_CURSOR          SCREEN_POS(OWL_WARP_SNOWHEAD, X)
#define OWL_WARP_MOUNTAIN_VILLAGE_X_CURSOR  SCREEN_POS(OWL_WARP_MOUNTAIN_VILLAGE, X)
#define OWL_WARP_CLOCK_TOWN_X_CURSOR        SCREEN_POS(OWL_WARP_CLOCK_TOWN, X)
#define OWL_WARP_MILK_ROAD_X_CURSOR         SCREEN_POS(OWL_WARP_MILK_ROAD, X)
#define OWL_WARP_WOODFALL_X_CURSOR          SCREEN_POS(OWL_WARP_WOODFALL, X)
#define OWL_WARP_SOUTHERN_SWAMP_X_CURSOR    SCREEN_POS(OWL_WARP_SOUTHERN_SWAMP, X)
#define OWL_WARP_IKANA_CANYON_X_CURSOR      SCREEN_POS(OWL_WARP_IKANA_CANYON, X)
#define OWL_WARP_STONE_TOWER_X_CURSOR       SCREEN_POS(OWL_WARP_STONE_TOWER, X)

#define OWL_WARP_GREAT_BAY_COAST_Y_CURSOR   SCREEN_POS(OWL_WARP_GREAT_BAY_COAST, Y)
#define OWL_WARP_ZORA_CAPE_Y_CURSOR         SCREEN_POS(OWL_WARP_ZORA_CAPE, Y)
#define OWL_WARP_SNOWHEAD_Y_CURSOR          SCREEN_POS(OWL_WARP_SNOWHEAD, Y)
#define OWL_WARP_MOUNTAIN_VILLAGE_Y_CURSOR  SCREEN_POS(OWL_WARP_MOUNTAIN_VILLAGE, Y)
#define OWL_WARP_CLOCK_TOWN_Y_CURSOR        SCREEN_POS(OWL_WARP_CLOCK_TOWN, Y)
#define OWL_WARP_MILK_ROAD_Y_CURSOR         SCREEN_POS(OWL_WARP_MILK_ROAD, Y)
#define OWL_WARP_WOODFALL_Y_CURSOR          SCREEN_POS(OWL_WARP_WOODFALL, Y)
#define OWL_WARP_SOUTHERN_SWAMP_Y_CURSOR    SCREEN_POS(OWL_WARP_SOUTHERN_SWAMP, Y)
#define OWL_WARP_IKANA_CANYON_Y_CURSOR      SCREEN_POS(OWL_WARP_IKANA_CANYON, Y)
#define OWL_WARP_STONE_TOWER_Y_CURSOR       SCREEN_POS(OWL_WARP_STONE_TOWER, Y)

#define CURSOR_POS_CENTER(NAME, AXIS) NAME ## _ ## AXIS ## _CURSOR + CURSOR_OFFSET_ ## AXIS

f32 new_sWorldMapCursorsX[REGION_MAX] = {
    CURSOR_POS_CENTER(GREAT_BAY, X),         // GREAT BAY
    CURSOR_POS_CENTER(ZORA_HALL, X),         // ZORA HALL
    CURSOR_POS_CENTER(ROMANI_RANCH, X),      // ROMANI RANCH
    CURSOR_POS_CENTER(DEKU_PALACE, X),       // DEKU PALACE
    CURSOR_POS_CENTER(WOODFALL, X),          // WOODFALL
    CURSOR_POS_CENTER(CLOCK_TOWN, X),        // CLOCK TOWN
    CURSOR_POS_CENTER(SNOWHEAD, X),          // SNOWHEAD
    CURSOR_POS_CENTER(IKANA_GRAVEYARD, X),   // IKANA GRAVEYARD
    CURSOR_POS_CENTER(IKANA_CANYON, X),      // IKANA CANYON
    CURSOR_POS_CENTER(GORON_VILLAGE, X),     // GORON VILLAGE
    CURSOR_POS_CENTER(STONE_TOWER, X),       // STONE TOWER
};

f32 new_sWorldMapCursorsY[REGION_MAX] = {
    CURSOR_POS_CENTER(GREAT_BAY, Y),       // GREAT BAY
    CURSOR_POS_CENTER(ZORA_HALL, Y),       // ZORA HALL
    CURSOR_POS_CENTER(ROMANI_RANCH, Y),    // ROMANI RANCH
    CURSOR_POS_CENTER(DEKU_PALACE, Y),     // DEKU PALACE
    CURSOR_POS_CENTER(WOODFALL, Y),        // WOODFALL
    CURSOR_POS_CENTER(CLOCK_TOWN, Y),      // CLOCK TOWN
    CURSOR_POS_CENTER(SNOWHEAD, Y),        // SNOWHEAD
    CURSOR_POS_CENTER(IKANA_GRAVEYARD, Y), // IKANA GRAVEYARD
    CURSOR_POS_CENTER(IKANA_CANYON, Y),    // IKANA CANYON
    CURSOR_POS_CENTER(GORON_VILLAGE, Y),   // GORON VILLAGE
    CURSOR_POS_CENTER(STONE_TOWER, Y),     // STONE TOWER
};

f32 new_sOwlWarpWorldMapCursorsX[OWL_WARP_MAX - 1] = {
    CURSOR_POS_CENTER(OWL_WARP_GREAT_BAY_COAST, X),     // OWL_WARP_GREAT_BAY_COAST
    CURSOR_POS_CENTER(OWL_WARP_ZORA_CAPE, X),           // OWL_WARP_ZORA_CAPE
    CURSOR_POS_CENTER(OWL_WARP_SNOWHEAD, X),            // OWL_WARP_SNOWHEAD
    CURSOR_POS_CENTER(OWL_WARP_MOUNTAIN_VILLAGE, X),    // OWL_WARP_MOUNTAIN_VILLAGE
    CURSOR_POS_CENTER(OWL_WARP_CLOCK_TOWN, X),          // OWL_WARP_CLOCK_TOWN
    CURSOR_POS_CENTER(OWL_WARP_MILK_ROAD, X),           // OWL_WARP_MILK_ROAD
    CURSOR_POS_CENTER(OWL_WARP_WOODFALL, X),            // OWL_WARP_WOODFALL
    CURSOR_POS_CENTER(OWL_WARP_SOUTHERN_SWAMP, X),      // OWL_WARP_SOUTHERN_SWAMP
    CURSOR_POS_CENTER(OWL_WARP_IKANA_CANYON, X),        // OWL_WARP_IKANA_CANYON
    CURSOR_POS_CENTER(OWL_WARP_STONE_TOWER, X),         // OWL_WARP_STONE_TOWER
};

f32 new_sOwlWarpWorldMapCursorsY[OWL_WARP_MAX - 1] = {
    CURSOR_POS_CENTER(OWL_WARP_GREAT_BAY_COAST, Y),  // OWL_WARP_GREAT_BAY_COAST
    CURSOR_POS_CENTER(OWL_WARP_ZORA_CAPE, Y),        // OWL_WARP_ZORA_CAPE
    CURSOR_POS_CENTER(OWL_WARP_SNOWHEAD, Y),         // OWL_WARP_SNOWHEAD
    CURSOR_POS_CENTER(OWL_WARP_MOUNTAIN_VILLAGE, Y), // OWL_WARP_MOUNTAIN_VILLAGE
    CURSOR_POS_CENTER(OWL_WARP_CLOCK_TOWN, Y),       // OWL_WARP_CLOCK_TOWN
    CURSOR_POS_CENTER(OWL_WARP_MILK_ROAD, Y),        // OWL_WARP_MILK_ROAD
    CURSOR_POS_CENTER(OWL_WARP_WOODFALL, Y),         // OWL_WARP_WOODFALL
    CURSOR_POS_CENTER(OWL_WARP_SOUTHERN_SWAMP, Y),   // OWL_WARP_SOUTHERN_SWAMP
    CURSOR_POS_CENTER(OWL_WARP_IKANA_CANYON, Y),     // OWL_WARP_IKANA_CANYON
    CURSOR_POS_CENTER(OWL_WARP_STONE_TOWER, Y),      // OWL_WARP_STONE_TOWER
};

void update_map(void) {
    UPDATE_ARRAY(sWorldMapCursorsRectLeft, new_sWorldMapCursorsRectLeft, REGION_MAX);
    UPDATE_ARRAY(sWorldMapCursorsRectTop, new_sWorldMapCursorsRectTop, REGION_MAX);

    UPDATE_ARRAY(sWorldMapCursorsX, new_sWorldMapCursorsX, REGION_MAX);
    UPDATE_ARRAY(sWorldMapCursorsY, new_sWorldMapCursorsY, REGION_MAX);

    UPDATE_ARRAY(sOwlWarpWorldMapCursorsX, new_sOwlWarpWorldMapCursorsX, REGION_MAX - 1);
    UPDATE_ARRAY(sOwlWarpWorldMapCursorsY, new_sOwlWarpWorldMapCursorsY, REGION_MAX - 1);

    UPDATE_ARRAY((&sVtxPageMapWorldQuadsX[sVtxPageMapWorldQuadsOffset]), new_sVtxPageMapWorldQuadsX, VTX_PAGE_MAP_WORLD_QUADS - sVtxPageMapWorldQuadsOffset);
    UPDATE_ARRAY((&sVtxPageMapWorldQuadsY[sVtxPageMapWorldQuadsOffset]), new_sVtxPageMapWorldQuadsY, VTX_PAGE_MAP_WORLD_QUADS - sVtxPageMapWorldQuadsOffset);
}

#include "PR/gbi.h"
#include "assets/interface/icon_item_field_static/icon_item_field_static.h"
#include "assets/archives/icon_item_24_static/icon_item_24_static_yar.h"

extern TexturePtr sCloudTextures[];
extern s16 sWorldMapDotPrimColors[][3];
extern s16 sWorldMapDotEnvColors[][3];
extern s16 sGreatFairySpawnRegions[];

RECOMP_HOOK("KaleidoScope_DrawWorldMap") void on_KaleidoScope_DrawWorldMap(PlayState* play) {
    update_map();
}