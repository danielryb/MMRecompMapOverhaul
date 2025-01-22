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

// TODO Change to hook, once they're ready
RECOMP_PATCH void KaleidoScope_DrawWorldMap(PlayState* play) {
    // @mod
    update_map();

    s16 sceneId;
    s16 t;
    s16 n;
    s16 j;
    s16 k;
    s16 i;
    PauseContext* pauseCtx = &play->pauseCtx;
    s16 rectLeft;
    s16 rectRight;

    OPEN_DISPS(play->state.gfxCtx);

    KaleidoScope_SetCursorVtxPos(pauseCtx, pauseCtx->cursorSlot[PAUSE_MAP] * 4, pauseCtx->mapPageVtx);

    // Draw the world map image
    if ((pauseCtx->pageIndex == PAUSE_MAP) && (pauseCtx->state == PAUSE_STATE_MAIN) &&
        ((pauseCtx->mainState == PAUSE_MAIN_STATE_IDLE) || (pauseCtx->mainState == PAUSE_MAIN_STATE_EQUIP_ITEM)) &&
        YREG(6) && (pauseCtx->state != PAUSE_STATE_SAVEPROMPT) && !IS_PAUSE_STATE_GAMEOVER(pauseCtx)) {

        // Draw the world map image flat
        // Because it is flat, the texture is loaded by filling it in 8 rows at a time.
        // 8 is chosen because it is smaller than `TMEM_SIZE / 2 / textureWidth` and divides the texture's height.
        // Each loaded chunk must have `size <= TMEM_SIZE / 2`
        // because the texture is color-indexed, so the TLUT uses the other half of TMEM.

        Gfx_SetupDL39_Opa(play->state.gfxCtx);

        gDPSetTextureFilter(POLY_OPA_DISP++, G_TF_POINT);
        gDPLoadTLUT_pal256(POLY_OPA_DISP++, gWorldMapImageTLUT);
        gDPSetTextureLUT(POLY_OPA_DISP++, G_TT_RGBA16);

        gDPSetPrimColor(POLY_OPA_DISP++, 0, 0, 255, 255, 255, pauseCtx->alpha);

        // Process the 128 rows of pixels for gWorldMapImageTex, 8 rows at a time over 16 iterations
        // Loop over yPos (t), textureIndex (j)
        for (t = 62, j = 0; j < 16; j++, t += 8) {
            gDPLoadTextureBlock(POLY_OPA_DISP++, (u8*)gWorldMapImageTex + j * (WORLD_MAP_IMAGE_WIDTH * 8), G_IM_FMT_CI,
                                G_IM_SIZ_8b, WORLD_MAP_IMAGE_WIDTH, 8, 0, G_TX_NOMIRROR | G_TX_WRAP,
                                G_TX_NOMIRROR | G_TX_WRAP, G_TX_NOMASK, G_TX_NOMASK, G_TX_NOLOD, G_TX_NOLOD);

            rectLeft = 51 << 2;
            rectRight = rectLeft + (WORLD_MAP_IMAGE_WIDTH << 2);
            gSPTextureRectangle(POLY_OPA_DISP++, rectLeft, t << 2, rectRight, (t << 2) + (8 << 2), G_TX_RENDERTILE, 0,
                                0, 1 << 10, 1 << 10);
        }

        Gfx_SetupDL42_Opa(play->state.gfxCtx);

    } else {
        // Draw the world map angled
        // Because it is at an angle, vertices are used to place it.
        // The structure of the loops here is to satisfy the constraints of both TMEM and the size of the vertex cache.
        // - Each loop iteration loads 9 rows, because 9 is the largest number smaller than
        //   `TMEM_SIZE / 2 / textureWidth`.
        // - Each loop is at most 8 iterations long because each row uses 4 vertices and the vertex cache has size
        //   `32 = 8 * 4`.
        // .
        // Hence there is one loop of length 8, one of length 6, and then the remaining `128 - (8 + 6) * 9 = 2` rows are
        // drawn at the end.

        gDPPipeSync(POLY_OPA_DISP++);
        gDPSetTextureFilter(POLY_OPA_DISP++, G_TF_POINT);
        gDPLoadTLUT_pal256(POLY_OPA_DISP++, gWorldMapImageTLUT);
        gDPSetTextureLUT(POLY_OPA_DISP++, G_TT_RGBA16);

        gDPSetPrimColor(POLY_OPA_DISP++, 0, 0, 255, 255, 255, pauseCtx->alpha);

        // Set the vertices for the first 8 quads attached to the world map texture.
        gSPVertex(POLY_OPA_DISP++, &pauseCtx->mapPageVtx[QUAD_MAP_PAGE_WORLD_IMAGE_FIRST * 4], 8 * 4, 0);

        // Process the first 72 rows of pixels for gWorldMapImageTex, 9 rows at a time over 8 iterations
        // Loop over quadIndex of this loop (i), quadIndex of the entire texture (k), vtxIndex (j)
        for (i = 0, k = 0, j = 0; i < 8; i++, k++, j += 4) {
            gDPLoadTextureBlock(
                POLY_OPA_DISP++, (u8*)gWorldMapImageTex + k * (WORLD_MAP_IMAGE_WIDTH * WORLD_MAP_IMAGE_FRAG_HEIGHT),
                G_IM_FMT_CI, G_IM_SIZ_8b, WORLD_MAP_IMAGE_WIDTH, WORLD_MAP_IMAGE_FRAG_HEIGHT, 0,
                G_TX_NOMIRROR | G_TX_WRAP, G_TX_NOMIRROR | G_TX_WRAP, G_TX_NOMASK, G_TX_NOMASK, G_TX_NOLOD, G_TX_NOLOD);

            gSP1Quadrangle(POLY_OPA_DISP++, j, j + 2, j + 3, j + 1, 0);
        }

        // Set the vertices for the last 7 quads attached to the world map texture:
        // 6 quads with a height of 9, 1 quad with a height of 2
        gSPVertex(POLY_OPA_DISP++, &pauseCtx->mapPageVtx[(QUAD_MAP_PAGE_WORLD_IMAGE_FIRST + 8) * 4], (6 + 1) * 4, 0);

        // Process the next 54 rows of pixels for gWorldMapImageTex, 9 rows at a time over 6 iterations
        // Loop over quadIndex of this loop (i), quadIndex of the entire texture (k), vtxIndex (j)
        for (i = 0, j = 0; i < 6; i++, k++, j += 4) {
            gDPLoadTextureBlock(
                POLY_OPA_DISP++, (u8*)gWorldMapImageTex + k * (WORLD_MAP_IMAGE_WIDTH * WORLD_MAP_IMAGE_FRAG_HEIGHT),
                G_IM_FMT_CI, G_IM_SIZ_8b, WORLD_MAP_IMAGE_WIDTH, WORLD_MAP_IMAGE_FRAG_HEIGHT, 0,
                G_TX_NOMIRROR | G_TX_WRAP, G_TX_NOMIRROR | G_TX_WRAP, G_TX_NOMASK, G_TX_NOMASK, G_TX_NOLOD, G_TX_NOLOD);

            gSP1Quadrangle(POLY_OPA_DISP++, j, j + 2, j + 3, j + 1, 0);
        }

        // Process the last 2 rows of pixels for gWorldMapImageTex
        gDPLoadTextureBlock(
            POLY_OPA_DISP++, (u8*)gWorldMapImageTex + k * (WORLD_MAP_IMAGE_WIDTH * WORLD_MAP_IMAGE_FRAG_HEIGHT),
            G_IM_FMT_CI, G_IM_SIZ_8b, WORLD_MAP_IMAGE_WIDTH, WORLD_MAP_IMAGE_HEIGHT % WORLD_MAP_IMAGE_FRAG_HEIGHT, 0,
            G_TX_NOMIRROR | G_TX_WRAP, G_TX_NOMIRROR | G_TX_WRAP, G_TX_NOMASK, G_TX_NOMASK, G_TX_NOLOD, G_TX_NOLOD);

        gSP1Quadrangle(POLY_OPA_DISP++, j, j + 2, j + 3, j + 1, 0);
    }

    Gfx_SetupDL42_Opa(play->state.gfxCtx);

    gDPPipeSync(POLY_OPA_DISP++);

    gDPSetTextureFilter(POLY_OPA_DISP++, G_TF_BILERP);

    gDPSetCombineLERP(POLY_OPA_DISP++, PRIMITIVE, ENVIRONMENT, TEXEL0, ENVIRONMENT, TEXEL0, 0, PRIMITIVE, 0, PRIMITIVE,
                      ENVIRONMENT, TEXEL0, ENVIRONMENT, TEXEL0, 0, PRIMITIVE, 0);

    gDPSetPrimColor(POLY_OPA_DISP++, 0, 0, 215, 235, 235, pauseCtx->alpha);
    gDPSetEnvColor(POLY_OPA_DISP++, 40, 60, 100, 0);

    // Draw clouds over the world map
    // Iterate over cloud bits (n)
    for (n = 0; n < WORLD_MAP_NUM_CLOUDS; n++) {
        if (!(((void)0, gSaveContext.save.saveInfo.worldMapCloudVisibility) & gBitFlags[n])) {

            gSPVertex(POLY_OPA_DISP++, &pauseCtx->mapPageVtx[(QUAD_MAP_PAGE_WORLD_CLOUDS_FIRST + n) * 4], 4, 0);

            POLY_OPA_DISP = Gfx_DrawTexQuadIA8(POLY_OPA_DISP, sCloudTextures[n], gVtxPageMapWorldQuadsWidth[n],
                                               gVtxPageMapWorldQuadsHeight[n], 0);
        }
    }

    if (IS_PAUSE_STATE_OWL_WARP(pauseCtx)) {
        gDPPipeSync(POLY_OPA_DISP++);
        gDPSetRenderMode(POLY_OPA_DISP++, G_RM_XLU_SURF, G_RM_XLU_SURF2);
        gDPSetCombineMode(POLY_OPA_DISP++, G_CC_PRIMITIVE, G_CC_PRIMITIVE);
        gDPSetPrimColor(POLY_OPA_DISP++, 0, 0, 0, 0, 0, R_PAUSE_OWL_WARP_ALPHA);
        gDPFillRectangle(POLY_OPA_DISP++, 50, 62, 270, 190);
    }

    Gfx_SetupDL42_Opa(play->state.gfxCtx);

    if (!IS_PAUSE_STATE_OWL_WARP(pauseCtx)) {
        // Browsing the world map regions on the pause menu
        gDPLoadTextureBlock(POLY_OPA_DISP++, gWorldMapDotTex, G_IM_FMT_IA, G_IM_SIZ_8b, 8, 8, 0,
                            G_TX_NOMIRROR | G_TX_WRAP, G_TX_NOMIRROR | G_TX_WRAP, G_TX_NOMASK, G_TX_NOMASK, G_TX_NOLOD,
                            G_TX_NOLOD);
        gDPSetCombineLERP(POLY_OPA_DISP++, PRIMITIVE, ENVIRONMENT, TEXEL0, ENVIRONMENT, TEXEL0, 0, PRIMITIVE, 0,
                          PRIMITIVE, ENVIRONMENT, TEXEL0, ENVIRONMENT, TEXEL0, 0, PRIMITIVE, 0);

        gDPSetPrimColor(POLY_OPA_DISP++, 0, 0, sWorldMapDotPrimColors[0][0], sWorldMapDotPrimColors[0][1],
                        sWorldMapDotPrimColors[0][2], pauseCtx->alpha);
        gDPSetEnvColor(POLY_OPA_DISP++, sWorldMapDotEnvColors[0][0], sWorldMapDotEnvColors[0][1],
                       sWorldMapDotEnvColors[0][2], 0);

        if (R_PAUSE_DBG_MAP_CLOUD_ON) {
            gSaveContext.save.saveInfo.worldMapCloudVisibility |= (u16)~0x8000;

            // QUAD_MAP_PAGE_WORLD_REGION_FIRST
            pauseCtx->mapPageVtx[120].v.ob[0] = pauseCtx->mapPageVtx[122].v.ob[0] = R_PAUSE_DBG_MAP_CLOUD_X;

            pauseCtx->mapPageVtx[121].v.ob[0] = pauseCtx->mapPageVtx[123].v.ob[0] =
                pauseCtx->mapPageVtx[120].v.ob[0] + 8;

            pauseCtx->mapPageVtx[120].v.ob[1] = pauseCtx->mapPageVtx[121].v.ob[1] = R_PAUSE_DBG_MAP_CLOUD_Y;

            pauseCtx->mapPageVtx[122].v.ob[1] = pauseCtx->mapPageVtx[123].v.ob[1] =
                pauseCtx->mapPageVtx[120].v.ob[1] - 8;
        }

        // Loop over RegionId (i), unused vtxIndex (j), unused (k)
        for (i = 0, j = 0; i < REGION_MAX; i++, k++, j += 4) {
            if (pauseCtx->worldMapPoints[i]) {
                gSPVertex(POLY_OPA_DISP++, &pauseCtx->mapPageVtx[(QUAD_MAP_PAGE_WORLD_REGION_FIRST + i) * 4], 4, 0);
                gSP1Quadrangle(POLY_OPA_DISP++, 0, 2, 3, 1, 0);
            }
        }

    } else {
        // Selecting an owl warp
        gDPPipeSync(POLY_OPA_DISP++);
        gDPSetCombineMode(POLY_OPA_DISP++, G_CC_MODULATEIA_PRIM, G_CC_MODULATEIA_PRIM);
        gDPLoadTextureBlock(POLY_OPA_DISP++, gWorldMapOwlFaceTex, G_IM_FMT_RGBA, G_IM_SIZ_32b, 24, 12, 0,
                            G_TX_NOMIRROR | G_TX_WRAP, G_TX_NOMIRROR | G_TX_WRAP, G_TX_NOMASK, G_TX_NOMASK, G_TX_NOLOD,
                            G_TX_NOLOD);

        gDPSetPrimColor(POLY_OPA_DISP++, 0, 0, 255, 255, 255, pauseCtx->alpha);

        if (R_PAUSE_DBG_MAP_CLOUD_ON) {
            gSaveContext.save.saveInfo.worldMapCloudVisibility |= (u16)~0x8000;

            // QUAD_MAP_PAGE_WORLD_WARP_FIRST
            pauseCtx->mapPageVtx[164].v.ob[0] = pauseCtx->mapPageVtx[166].v.ob[0] = R_PAUSE_DBG_MAP_CLOUD_X;

            pauseCtx->mapPageVtx[165].v.ob[0] = pauseCtx->mapPageVtx[167].v.ob[0] =
                pauseCtx->mapPageVtx[164].v.ob[0] + 24;

            pauseCtx->mapPageVtx[164].v.ob[1] = pauseCtx->mapPageVtx[165].v.ob[1] = R_PAUSE_DBG_MAP_CLOUD_Y;

            pauseCtx->mapPageVtx[166].v.ob[1] = pauseCtx->mapPageVtx[167].v.ob[1] =
                pauseCtx->mapPageVtx[164].v.ob[1] - 12;
        }

        // Loop over OwlWarpId (i), unused vtxIndex (j), unused (k)
        for (i = 0, j = 0; i < OWL_WARP_ENTRANCE; i++, k++, j += 4) {
            if (pauseCtx->worldMapPoints[i]) {
                gSPVertex(POLY_OPA_DISP++, &pauseCtx->mapPageVtx[(QUAD_MAP_PAGE_WORLD_WARP_FIRST + i) * 4], 4, 0);
                gSP1Quadrangle(POLY_OPA_DISP++, 0, 2, 3, 1, 0);
            }
        }
    }

    // Find and draw Player's face at the current region based on the current scene
    // If (pauseCtx->state == PAUSE_STATE_MAIN), then the other pauseCtx->state conditions are redundant
    // and always return true
    if ((pauseCtx->pageIndex == PAUSE_MAP) && (pauseCtx->mainState == PAUSE_MAIN_STATE_IDLE) &&
        (pauseCtx->state == PAUSE_STATE_MAIN) && (pauseCtx->state != PAUSE_STATE_SAVEPROMPT) &&
        !IS_PAUSE_STATE_GAMEOVER(pauseCtx)) {
        j = 0;
        n = 0;

        sceneId = play->sceneId;

        // Map grottos/shrines to sceneId's to be used in different regions
        if (sceneId == SCENE_KAKUSIANA) {
            if (play->roomCtx.curRoom.num == 5) {
                sceneId = SCENE_11GORONNOSATO;
            } else if ((play->roomCtx.curRoom.num == 6) || (play->roomCtx.curRoom.num == 8) ||
                       (play->roomCtx.curRoom.num == 12)) {
                sceneId = SCENE_22DEKUCITY;
            } else {
                sceneId = Entrance_GetSceneIdAbsolute(((void)0, gSaveContext.respawn[RESPAWN_MODE_UNK_3].entrance));
            }
        }

        // Find the region that player is currently in
        // Loop over region (n) and regionIndex (j)
        while (true) {
            if (gSceneIdsPerRegion[n][j] == 0xFFFF) {
                n++;
                j = 0;
                if (n == REGION_MAX) {
                    n = 0;

                    // Special case for fairy fountains
                    if (sceneId == SCENE_YOUSEI_IZUMI) {
                        j = play->curSpawn;
                        n = sGreatFairySpawnRegions[j];
                        break;
                    }

                    while (true) {
                        if (gSceneIdsPerRegion[n][j] == 0xFFFF) {
                            n++;
                            if (n == REGION_MAX) {
                                break;
                            }
                            j = 0;
                            if (Entrance_GetSceneIdAbsolute(
                                    ((void)0, gSaveContext.respawn[RESPAWN_MODE_UNK_3].entrance)) ==
                                gSceneIdsPerRegion[n][j]) {
                                break;
                            }
                        }
                        j++;
                    }
                    break;
                }
            }

            if (sceneId == gSceneIdsPerRegion[n][j]) {
                break;
            }
            j++;
        }

        // Draw Player's face at the current region
        if (n != REGION_MAX) {
            KaleidoScope_SetView(pauseCtx, pauseCtx->eye.x, pauseCtx->eye.y, pauseCtx->eye.z);
            Gfx_SetupDL39_Opa(play->state.gfxCtx);

            gDPSetPrimColor(POLY_OPA_DISP++, 0, 0, 255, 255, 255, pauseCtx->alpha);

            POLY_OPA_DISP =
                Gfx_DrawTexRectRGBA16(POLY_OPA_DISP, gQuestIconLinkHumanFaceTex, 16, 16, sWorldMapCursorsRectLeft[n],
                                      sWorldMapCursorsRectTop[n], 16, 16, 1 << 10, 1 << 10);
        }
    }

    gDPPipeSync(POLY_OPA_DISP++);

    CLOSE_DISPS(play->state.gfxCtx);
}