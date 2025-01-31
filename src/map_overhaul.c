#include "map_overhaul.h"

RECOMP_IMPORT("*", void recomp_printf(char *msg, ...));

typedef struct {
    Vec2f points[4];
} minimap_map_t;

#include "map_data.c"

#define UPDATE_ARRAY(to, from, length)                                         \
    do {                                                                       \
        for (size_t i = 0; i < length; i++) {                                  \
            to[i] = from[i];                                                   \
        }                                                                      \
    } while (false)

// Face
#define FACE_POS(NAME, AXIS) (NAME ## _ ## AXIS + FACE_OFFSET_ ## AXIS)

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
#define PIXEL_TO_FLOAT(NAME, AXIS) ((NAME ## _ ## AXIS - MAP_CENTER_PX_ ## AXIS + PIXEL_TO_FLOAT_PX_OFFSET_ ## AXIS) * PIXEL_TO_FLOAT_SCALE_ ## AXIS)

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

//Dots and Owls

#define sVtxPageMapWorldQuadsOffset 15

#define DOT_QUAD_POS(NAME, QUAD, AXIS) (NAME ## _ ## AXIS ## _F + QUAD ## _OFFSET_ ## AXIS)

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
#define SCREEN_POS(NAME, AXIS) (NAME ## _ ## AXIS ## _F * MAP_TO_SCREEN_ ## AXIS)

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

#define CURSOR_POS_CENTER(NAME, AXIS) (NAME ## _ ## AXIS ## _CURSOR + CURSOR_OFFSET_ ## AXIS)

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

void map_overhaul_update_map(void) {
    // recomp_printf("Player pos: %f, %f\n", playerMinimapUnitPos.x, playerMinimapUnitPos.z);

    UPDATE_ARRAY(sWorldMapCursorsRectLeft, new_sWorldMapCursorsRectLeft, REGION_MAX);
    UPDATE_ARRAY(sWorldMapCursorsRectTop, new_sWorldMapCursorsRectTop, REGION_MAX);

    UPDATE_ARRAY(sWorldMapCursorsX, new_sWorldMapCursorsX, REGION_MAX);
    UPDATE_ARRAY(sWorldMapCursorsY, new_sWorldMapCursorsY, REGION_MAX);

    UPDATE_ARRAY(sOwlWarpWorldMapCursorsX, new_sOwlWarpWorldMapCursorsX, REGION_MAX - 1);
    UPDATE_ARRAY(sOwlWarpWorldMapCursorsY, new_sOwlWarpWorldMapCursorsY, REGION_MAX - 1);

    UPDATE_ARRAY((&sVtxPageMapWorldQuadsX[sVtxPageMapWorldQuadsOffset]), new_sVtxPageMapWorldQuadsX, VTX_PAGE_MAP_WORLD_QUADS - sVtxPageMapWorldQuadsOffset);
    UPDATE_ARRAY((&sVtxPageMapWorldQuadsY[sVtxPageMapWorldQuadsOffset]), new_sVtxPageMapWorldQuadsY, VTX_PAGE_MAP_WORLD_QUADS - sVtxPageMapWorldQuadsOffset);
}

Vec2f map_overhaul_playerMinimapUnitPos;
minimap_map_t* minimapToWorldMap = NULL;

void map_overhaul_reload_minimap_data(void) {
    s32 scene = ((void)0, gSaveContext.save.entrance) >> 9;

    recomp_printf("Scene: %d\n", scene);

    u8 room = 0;

    minimap_map_t* scene_maps = MiniToWorld[scene];
    if (scene_maps != NULL) {
        minimapToWorldMap = &scene_maps[room];
        recomp_printf("%f\n", minimapToWorldMap[0]);
    } else {
        minimapToWorldMap = NULL;
    }
}

void Math_Vec2f_Sum(Vec2f* l, Vec2f* r, Vec2f* dest) {
    dest->x = l->x + r->x;
    dest->z = l->z + r->z;
}

void Math_Vec2f_Diff(Vec2f* l, Vec2f* r, Vec2f* dest) {
    dest->x = l->x - r->x;
    dest->z = l->z - r->z;
}

void Math_Vec2f_Scale(Vec2f* vec, f32 scale) {
    vec->x *= scale;
    vec->z *= scale;
}

void Math_Vec2f_ScaleAndStore(Vec2f* vec, f32 scale, Vec2f* dest) {
    dest->x = vec->x * scale;
    dest->z = vec->z * scale;
}

bool map_overhaul_minimap_unit_to_world_map_position(Vec2f* minimapPos, Vec2f* dest) {
    if (minimapToWorldMap == NULL) {
        return false;
    }

    f32 x = minimapPos->x;
    f32 y = minimapPos->z;

    Vec2f* A = &minimapToWorldMap->points[0];
    Vec2f* B = &minimapToWorldMap->points[1];
    Vec2f* C = &minimapToWorldMap->points[2];
    Vec2f* D = &minimapToWorldMap->points[3];

    Vec2f* OffsetAB = dest;
    Math_Vec2f_Diff(B, A, OffsetAB);
    Math_Vec2f_Scale(OffsetAB, x);
    Math_Vec2f_Sum(A, OffsetAB, OffsetAB);

    Vec2f tmp;
    Vec2f* OffsetCD = &tmp;
    Math_Vec2f_Diff(D, C, OffsetCD);
    Math_Vec2f_Scale(OffsetCD, x);
    Math_Vec2f_Sum(C, OffsetCD, OffsetCD);

    Math_Vec2f_Scale(OffsetAB, 1 - y);
    Math_Vec2f_Scale(OffsetCD, y);

    Math_Vec2f_Sum(OffsetAB, OffsetCD, dest);

    return true;
}