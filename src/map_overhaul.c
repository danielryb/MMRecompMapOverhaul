#include "map_overhaul.h"
#include "lib/math.h"

typedef struct {
    f32 posY; // Y pos in scene to which map is adjusted.
    Vec2f points[4]; // Plane which maps the minimap to world map.
} minimap_map_t;

#include "map_data.c"

#define UPDATE_ARRAY(to, from, length)                                         \
    do {                                                                       \
        for (size_t i = 0; i < length; i++) {                                  \
            to[i] = from[i];                                                   \
        }                                                                      \
    } while (false)

// Face
#define FACE_POS(NAME, AXIS) (NAME ## _ ## AXIS)// + FACE_OFFSET_ ## AXIS)

#define FACE_POS_GREAT_BAY_X        FACE_POS(GREAT_BAY, X)        // GREAT BAY
#define FACE_POS_ZORA_HALL_X        FACE_POS(ZORA_HALL, X)        // ZORA HALL
#define FACE_POS_ROMANI_RANCH_X     FACE_POS(ROMANI_RANCH, X)     // ROMANI RANCH
#define FACE_POS_DEKU_PALACE_X      FACE_POS(DEKU_PALACE, X)      // DEKU PALACE
#define FACE_POS_WOODFALL_X         FACE_POS(WOODFALL, X)         // WOODFALL
#define FACE_POS_CLOCK_TOWN_X       FACE_POS(CLOCK_TOWN, X)       // CLOCK TOWN
#define FACE_POS_SNOWHEAD_X         FACE_POS(SNOWHEAD, X)         // SNOWHEAD
#define FACE_POS_IKANA_GRAVEYARD_X  FACE_POS(IKANA_GRAVEYARD, X)  // IKANA GRAVEYARD
#define FACE_POS_IKANA_CANYON_X     FACE_POS(IKANA_CANYON, X)     // IKANA CANYON
#define FACE_POS_GORON_VILLAGE_X    FACE_POS(GORON_VILLAGE, X)    // GORON VILLAGE
#define FACE_POS_STONE_TOWER_X      FACE_POS(STONE_TOWER, X)      // STONE TOWER

#define FACE_POS_GREAT_BAY_Y        FACE_POS(GREAT_BAY, Y)        // GREAT BAY
#define FACE_POS_ZORA_HALL_Y        FACE_POS(ZORA_HALL, Y)        // ZORA HALL
#define FACE_POS_ROMANI_RANCH_Y     FACE_POS(ROMANI_RANCH, Y)     // ROMANI RANCH
#define FACE_POS_DEKU_PALACE_Y      FACE_POS(DEKU_PALACE, Y)      // DEKU PALACE
#define FACE_POS_WOODFALL_Y         FACE_POS(WOODFALL, Y)         // WOODFALL
#define FACE_POS_CLOCK_TOWN_Y       FACE_POS(CLOCK_TOWN, Y)       // CLOCK TOWN
#define FACE_POS_SNOWHEAD_Y         FACE_POS(SNOWHEAD, Y)         // SNOWHEAD
#define FACE_POS_IKANA_GRAVEYARD_Y  FACE_POS(IKANA_GRAVEYARD, Y)  // IKANA GRAVEYARD
#define FACE_POS_IKANA_CANYON_Y     FACE_POS(IKANA_CANYON, Y)     // IKANA CANYON
#define FACE_POS_GORON_VILLAGE_Y    FACE_POS(GORON_VILLAGE, Y)    // GORON VILLAGE
#define FACE_POS_STONE_TOWER_Y      FACE_POS(STONE_TOWER, Y)      // STONE TOWER

//Dots and Owls

#define sVtxPageMapWorldQuadsOffset 15

// #define QUAD_POS(NAME, QUAD, AXIS) (NAME ## _ ## AXIS ## _F + QUAD ## _OFFSET_ ## AXIS)
#define QUAD_POS(NAME, QUAD, AXIS) (NAME ## _ ## AXIS)

f32 new_sVtxPageMapWorldQuadsX[VTX_PAGE_MAP_WORLD_QUADS - sVtxPageMapWorldQuadsOffset] = {
    QUAD_POS(GREAT_BAY, DOT, X),                 // mapPageVtx[120] world map region Great Bay
    QUAD_POS(ZORA_HALL, DOT, X),                 // mapPageVtx[124] world map region Zora Hall
    QUAD_POS(ROMANI_RANCH, DOT, X),              // mapPageVtx[128] world map region Romani Ranch
    QUAD_POS(DEKU_PALACE, DOT, X),               // mapPageVtx[132] world map region Deku Palace
    QUAD_POS(WOODFALL, DOT, X),                  // mapPageVtx[136] world map region Woodfall
    QUAD_POS(CLOCK_TOWN, DOT, X),                // mapPageVtx[140] world map region Clock Town
    QUAD_POS(SNOWHEAD, DOT, X),                  // mapPageVtx[144] world map region Snowhead
    QUAD_POS(IKANA_GRAVEYARD, DOT, X),           // mapPageVtx[148] world map region Ikana Graveyard
    QUAD_POS(IKANA_CANYON, DOT, X),              // mapPageVtx[152] world map region Ikana Canyon
    QUAD_POS(GORON_VILLAGE, DOT, X),             // mapPageVtx[156] world map region Goron Village
    QUAD_POS(STONE_TOWER, DOT, X),               // mapPageVtx[160] world map region Stone Tower

    QUAD_POS(OWL_WARP_GREAT_BAY_COAST, OWL, X),  // mapPageVtx[164] world map owl warp Great Bay Coast
    QUAD_POS(OWL_WARP_ZORA_CAPE, OWL, X),        // mapPageVtx[168] world map owl warp Zora Cape
    QUAD_POS(OWL_WARP_SNOWHEAD, OWL, X),         // mapPageVtx[172] world map owl warp Snowhead
    QUAD_POS(OWL_WARP_MOUNTAIN_VILLAGE, OWL, X), // mapPageVtx[176] world map owl warp Mountain Village
    QUAD_POS(OWL_WARP_CLOCK_TOWN, OWL, X),       // mapPageVtx[180] world map owl warp Clock Town
    QUAD_POS(OWL_WARP_MILK_ROAD, OWL, X),        // mapPageVtx[184] world map owl warp Milk Road
    QUAD_POS(OWL_WARP_WOODFALL, OWL, X),         // mapPageVtx[188] world map owl warp Woodfall
    QUAD_POS(OWL_WARP_SOUTHERN_SWAMP, OWL, X),   // mapPageVtx[192] world map owl warp Southern Swamp
    QUAD_POS(OWL_WARP_IKANA_CANYON, OWL, X),     // mapPageVtx[196] world map owl warp Ikana Canyon
    QUAD_POS(OWL_WARP_STONE_TOWER, OWL, X),      // mapPageVtx[200] world map owl warp Stone Tower
};

f32 new_sVtxPageMapWorldQuadsY[VTX_PAGE_MAP_WORLD_QUADS - sVtxPageMapWorldQuadsOffset] = {
    QUAD_POS(GREAT_BAY, DOT, Y),                 // mapPageVtx[120] world map region Great Bay
    QUAD_POS(ZORA_HALL, DOT, Y),                 // mapPageVtx[124] world map region Zora Hall
    QUAD_POS(ROMANI_RANCH, DOT, Y),              // mapPageVtx[128] world map region Romani Ranch
    QUAD_POS(DEKU_PALACE, DOT, Y),               // mapPageVtx[132] world map region Deku Palace
    QUAD_POS(WOODFALL, DOT, Y),                  // mapPageVtx[136] world map region Woodfall
    QUAD_POS(CLOCK_TOWN, DOT, Y),                // mapPageVtx[140] world map region Clock Town
    QUAD_POS(SNOWHEAD, DOT, Y),                  // mapPageVtx[144] world map region Snowhead
    QUAD_POS(IKANA_GRAVEYARD, DOT, Y),           // mapPageVtx[148] world map region Ikana Graveyard
    QUAD_POS(IKANA_CANYON, DOT, Y),              // mapPageVtx[152] world map region Ikana Canyon
    QUAD_POS(GORON_VILLAGE, DOT, Y),             // mapPageVtx[156] world map region Goron Village
    QUAD_POS(STONE_TOWER, DOT, Y),               // mapPageVtx[160] world map region Stone Tower

    QUAD_POS(OWL_WARP_GREAT_BAY_COAST, OWL, Y),  // mapPageVtx[164] world map owl warp Great Bay Coast
    QUAD_POS(OWL_WARP_ZORA_CAPE, OWL, Y),        // mapPageVtx[168] world map owl warp Zora Cape
    QUAD_POS(OWL_WARP_SNOWHEAD, OWL, Y),         // mapPageVtx[172] world map owl warp Snowhead
    QUAD_POS(OWL_WARP_MOUNTAIN_VILLAGE, OWL, Y), // mapPageVtx[176] world map owl warp Mountain Village
    QUAD_POS(OWL_WARP_CLOCK_TOWN, OWL, Y),       // mapPageVtx[180] world map owl warp Clock Town
    QUAD_POS(OWL_WARP_MILK_ROAD, OWL, Y),        // mapPageVtx[184] world map owl warp Milk Road
    QUAD_POS(OWL_WARP_WOODFALL, OWL, Y),         // mapPageVtx[188] world map owl warp Woodfall
    QUAD_POS(OWL_WARP_SOUTHERN_SWAMP, OWL, Y),   // mapPageVtx[192] world map owl warp Southern Swamp
    QUAD_POS(OWL_WARP_IKANA_CANYON, OWL, Y),     // mapPageVtx[196] world map owl warp Ikana Canyon
    QUAD_POS(OWL_WARP_STONE_TOWER, OWL, Y),      // mapPageVtx[200] world map owl warp Stone Tower
};

// Cursor
#define SCREEN_POS(NAME, AXIS) ((NAME ## _ ## AXIS - SCREEN_CENTER_PX_ ## AXIS) * MAP_TO_SCREEN_ ## AXIS)

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

#define CURSOR_POS_CENTER_GREAT_BAY_X       CURSOR_POS_CENTER(GREAT_BAY, X)         // GREAT BAY
#define CURSOR_POS_CENTER_ZORA_HALL_X       CURSOR_POS_CENTER(ZORA_HALL, X)         // ZORA HALL
#define CURSOR_POS_CENTER_ROMANI_RANCH_X    CURSOR_POS_CENTER(ROMANI_RANCH, X)      // ROMANI RANCH
#define CURSOR_POS_CENTER_DEKU_PALACE_X     CURSOR_POS_CENTER(DEKU_PALACE, X)       // DEKU PALACE
#define CURSOR_POS_CENTER_WOODFALL_X        CURSOR_POS_CENTER(WOODFALL, X)          // WOODFALL
#define CURSOR_POS_CENTER_CLOCK_TOWN_X      CURSOR_POS_CENTER(CLOCK_TOWN, X)        // CLOCK TOWN
#define CURSOR_POS_CENTER_SNOWHEAD_X        CURSOR_POS_CENTER(SNOWHEAD, X)          // SNOWHEAD
#define CURSOR_POS_CENTER_IKANA_GRAVEYARD_X CURSOR_POS_CENTER(IKANA_GRAVEYARD, X)   // IKANA GRAVEYARD
#define CURSOR_POS_CENTER_IKANA_CANYON_X    CURSOR_POS_CENTER(IKANA_CANYON, X)      // IKANA CANYON
#define CURSOR_POS_CENTER_GORON_VILLAGE_X   CURSOR_POS_CENTER(GORON_VILLAGE, X)     // GORON VILLAGE
#define CURSOR_POS_CENTER_STONE_TOWER_X     CURSOR_POS_CENTER(STONE_TOWER, X)       // STONE TOWER

#define CURSOR_POS_CENTER_GREAT_BAY_Y       CURSOR_POS_CENTER(GREAT_BAY, Y)         // GREAT BAY
#define CURSOR_POS_CENTER_ZORA_HALL_Y       CURSOR_POS_CENTER(ZORA_HALL, Y)         // ZORA HALL
#define CURSOR_POS_CENTER_ROMANI_RANCH_Y    CURSOR_POS_CENTER(ROMANI_RANCH, Y)      // ROMANI RANCH
#define CURSOR_POS_CENTER_DEKU_PALACE_Y     CURSOR_POS_CENTER(DEKU_PALACE, Y)       // DEKU PALACE
#define CURSOR_POS_CENTER_WOODFALL_Y        CURSOR_POS_CENTER(WOODFALL, Y)          // WOODFALL
#define CURSOR_POS_CENTER_CLOCK_TOWN_Y      CURSOR_POS_CENTER(CLOCK_TOWN, Y)        // CLOCK TOWN
#define CURSOR_POS_CENTER_SNOWHEAD_Y        CURSOR_POS_CENTER(SNOWHEAD, Y)          // SNOWHEAD
#define CURSOR_POS_CENTER_IKANA_GRAVEYARD_Y CURSOR_POS_CENTER(IKANA_GRAVEYARD, Y)   // IKANA GRAVEYARD
#define CURSOR_POS_CENTER_IKANA_CANYON_Y    CURSOR_POS_CENTER(IKANA_CANYON, Y)      // IKANA CANYON
#define CURSOR_POS_CENTER_GORON_VILLAGE_Y   CURSOR_POS_CENTER(GORON_VILLAGE, Y)     // GORON VILLAGE
#define CURSOR_POS_CENTER_STONE_TOWER_Y     CURSOR_POS_CENTER(STONE_TOWER, Y)       // STONE TOWER

#define CURSOR_POS_CENTER_OWL_WARP_GREAT_BAY_COAST_X    CURSOR_POS_CENTER(OWL_WARP_GREAT_BAY_COAST, X)  // OWL_WARP_GREAT_BAY_COAST
#define CURSOR_POS_CENTER_OWL_WARP_ZORA_CAPE_X          CURSOR_POS_CENTER(OWL_WARP_ZORA_CAPE, X)        // OWL_WARP_ZORA_CAPE
#define CURSOR_POS_CENTER_OWL_WARP_SNOWHEAD_X           CURSOR_POS_CENTER(OWL_WARP_SNOWHEAD, X)         // OWL_WARP_SNOWHEAD
#define CURSOR_POS_CENTER_OWL_WARP_MOUNTAIN_VILLAGE_X   CURSOR_POS_CENTER(OWL_WARP_MOUNTAIN_VILLAGE, X) // OWL_WARP_MOUNTAIN_VILLAGE
#define CURSOR_POS_CENTER_OWL_WARP_CLOCK_TOWN_X         CURSOR_POS_CENTER(OWL_WARP_CLOCK_TOWN, X)       // OWL_WARP_CLOCK_TOWN
#define CURSOR_POS_CENTER_OWL_WARP_MILK_ROAD_X          CURSOR_POS_CENTER(OWL_WARP_MILK_ROAD, X)        // OWL_WARP_MILK_ROAD
#define CURSOR_POS_CENTER_OWL_WARP_WOODFALL_X           CURSOR_POS_CENTER(OWL_WARP_WOODFALL, X)         // OWL_WARP_WOODFALL
#define CURSOR_POS_CENTER_OWL_WARP_SOUTHERN_SWAMP_X     CURSOR_POS_CENTER(OWL_WARP_SOUTHERN_SWAMP, X)   // OWL_WARP_SOUTHERN_SWAMP
#define CURSOR_POS_CENTER_OWL_WARP_IKANA_CANYON_X       CURSOR_POS_CENTER(OWL_WARP_IKANA_CANYON, X)     // OWL_WARP_IKANA_CANYON
#define CURSOR_POS_CENTER_OWL_WARP_STONE_TOWER_X        CURSOR_POS_CENTER(OWL_WARP_STONE_TOWER, X)      // OWL_WARP_STONE_TOWER

#define CURSOR_POS_CENTER_OWL_WARP_GREAT_BAY_COAST_Y    CURSOR_POS_CENTER(OWL_WARP_GREAT_BAY_COAST, Y)  // OWL_WARP_GREAT_BAY_COAST
#define CURSOR_POS_CENTER_OWL_WARP_ZORA_CAPE_Y          CURSOR_POS_CENTER(OWL_WARP_ZORA_CAPE, Y)        // OWL_WARP_ZORA_CAPE
#define CURSOR_POS_CENTER_OWL_WARP_SNOWHEAD_Y           CURSOR_POS_CENTER(OWL_WARP_SNOWHEAD, Y)         // OWL_WARP_SNOWHEAD
#define CURSOR_POS_CENTER_OWL_WARP_MOUNTAIN_VILLAGE_Y   CURSOR_POS_CENTER(OWL_WARP_MOUNTAIN_VILLAGE, Y) // OWL_WARP_MOUNTAIN_VILLAGE
#define CURSOR_POS_CENTER_OWL_WARP_CLOCK_TOWN_Y         CURSOR_POS_CENTER(OWL_WARP_CLOCK_TOWN, Y)       // OWL_WARP_CLOCK_TOWN
#define CURSOR_POS_CENTER_OWL_WARP_MILK_ROAD_Y          CURSOR_POS_CENTER(OWL_WARP_MILK_ROAD, Y)        // OWL_WARP_MILK_ROAD
#define CURSOR_POS_CENTER_OWL_WARP_WOODFALL_Y           CURSOR_POS_CENTER(OWL_WARP_WOODFALL, Y)         // OWL_WARP_WOODFALL
#define CURSOR_POS_CENTER_OWL_WARP_SOUTHERN_SWAMP_Y     CURSOR_POS_CENTER(OWL_WARP_SOUTHERN_SWAMP, Y)   // OWL_WARP_SOUTHERN_SWAMP
#define CURSOR_POS_CENTER_OWL_WARP_IKANA_CANYON_Y       CURSOR_POS_CENTER(OWL_WARP_IKANA_CANYON, Y)     // OWL_WARP_IKANA_CANYON
#define CURSOR_POS_CENTER_OWL_WARP_STONE_TOWER_Y        CURSOR_POS_CENTER(OWL_WARP_STONE_TOWER, Y)      // OWL_WARP_STONE_TOWER

// Dynamic float position calculations

#define SELECT_POS(VALUE, SYMBOL, DATA)                                        \
    case VALUE:                                                                \
        pos->x = DATA ## _ ## SYMBOL ## _X;                                    \
        pos->z = DATA ## _ ## SYMBOL ## _Y;                                    \
        break

#define SELECT_LOCATIONS(OFFSET, DATA)                  \
    SELECT_POS(OFFSET,      GREAT_BAY,       DATA);     \
    SELECT_POS(OFFSET + 1,  ZORA_HALL,       DATA);     \
    SELECT_POS(OFFSET + 2,  ROMANI_RANCH,    DATA);     \
    SELECT_POS(OFFSET + 3,  DEKU_PALACE,     DATA);     \
    SELECT_POS(OFFSET + 4,  WOODFALL,        DATA);     \
    SELECT_POS(OFFSET + 5,  CLOCK_TOWN,      DATA);     \
    SELECT_POS(OFFSET + 6,  SNOWHEAD,        DATA);     \
    SELECT_POS(OFFSET + 7,  IKANA_GRAVEYARD, DATA);     \
    SELECT_POS(OFFSET + 8,  IKANA_CANYON,    DATA);     \
    SELECT_POS(OFFSET + 9,  GORON_VILLAGE,   DATA);     \
    SELECT_POS(OFFSET + 10, STONE_TOWER,     DATA)

#define SELECT_WARPS(OFFSET, DATA)  \
    SELECT_POS(OFFSET,     OWL_WARP_GREAT_BAY_COAST,    DATA);     \
    SELECT_POS(OFFSET + 1, OWL_WARP_ZORA_CAPE,          DATA);     \
    SELECT_POS(OFFSET + 2, OWL_WARP_SNOWHEAD,           DATA);     \
    SELECT_POS(OFFSET + 3, OWL_WARP_MOUNTAIN_VILLAGE,   DATA);     \
    SELECT_POS(OFFSET + 4, OWL_WARP_CLOCK_TOWN,         DATA);     \
    SELECT_POS(OFFSET + 5, OWL_WARP_MILK_ROAD,          DATA);     \
    SELECT_POS(OFFSET + 6, OWL_WARP_WOODFALL,           DATA);     \
    SELECT_POS(OFFSET + 7, OWL_WARP_SOUTHERN_SWAMP,     DATA);     \
    SELECT_POS(OFFSET + 8, OWL_WARP_IKANA_CANYON,       DATA);     \
    SELECT_POS(OFFSET + 9, OWL_WARP_STONE_TOWER,        DATA)


void map_overhaul_get_face_position(size_t i, Vec2f* pos) {
    switch(i) {
        SELECT_LOCATIONS(0, FACE_POS);
    }
}

void get_cursor_position_map(size_t i, Vec2f* pos) {
    switch(i) {
        SELECT_LOCATIONS(0, CURSOR_POS_CENTER);
    }
}

void get_cursor_position_warp(size_t i, Vec2f* pos) {
    switch(i) {
        SELECT_WARPS(0, CURSOR_POS_CENTER);
    }
}

void map_overhul_update_cursor_position(PlayState *play) {
    PauseContext* pauseCtx = &play->pauseCtx;

    Vec2f cursor_pos;
    s16 cursorPoint = pauseCtx->cursorPoint[PAUSE_WORLD_MAP];
    if (IS_PAUSE_STATE_OWL_WARP(pauseCtx)) {
        get_cursor_position_warp(cursorPoint, &cursor_pos);
        sOwlWarpWorldMapCursorsX[cursorPoint] = cursor_pos.x;
        sOwlWarpWorldMapCursorsY[cursorPoint] = cursor_pos.z;
    } else {
        get_cursor_position_map(cursorPoint, &cursor_pos);
        sWorldMapCursorsX[cursorPoint] = cursor_pos.x;
        sWorldMapCursorsY[cursorPoint] = cursor_pos.z;
    }
}

Vec2f map_overhaul_playerMinimapUnitPos;
minimap_map_t* minimapToWorldMap = NULL;


void MapDisp_GetMapOffset(MapDataRoom* mapDataRoom, s32* offsetX, s32* offsetY);
void MapDisp_GetMapTexDim(MapDataRoom* mapDataRoom, s32* width, s32* height);
void MapDisp_GetMapScale(MapDataRoom* mapDataRoom, s32* scale);
s32 MapDisp_IsDataRotated(PlayState* play);
extern MapDisp sMapDisp;

void map_overhaul_reload_minimap_data(PlayState* play /*TMP*/, s8 room) {
    s32 scene = ((void)0, gSaveContext.save.entrance) >> 9;
    recomp_printf("scene: %d, room: %d\n", scene, room);

    minimap_map_t* scene_maps = MiniToWorld[scene];
    if (scene_maps != NULL) {
        minimapToWorldMap = &scene_maps[room];
        recomp_printf("%f\n", minimapToWorldMap[0]);
    } else {
        minimapToWorldMap = NULL;
    }

    s32 texOffsetX;
    s32 texOffsetY;
    s32 texWidth;
    s32 texHeight;
    MapDataRoom* mapDataRoom = &sMapDisp.mapDataScene->rooms[sMapDisp.curRoom];
    MapDisp_GetMapOffset(mapDataRoom, &texOffsetX, &texOffsetY);
    MapDisp_GetMapTexDim(mapDataRoom, &texWidth, &texHeight);

    s32 scale;
    if (sMapDisp.mapDataScene->scale == 0) {
        scale = 20;
    } else if (sMapDisp.mapDataScene->scale == -1) {
        s32 scaleTemp;

        MapDisp_GetMapScale(mapDataRoom, &scaleTemp);
        scale = scaleTemp;
    } else {
        scale = sMapDisp.mapDataScene->scale;
    }

    s16 sceneId = Play_GetOriginalSceneId(play->sceneId);
    recomp_printf("originalSceneId %d\n", sceneId);
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

void unit_to_world_map(Vec2f* unitPos, Vec2f* points, Vec2f* dest) {
    Vec2f* A = &points[0];
    Vec2f* B = &points[1];
    Vec2f* C = &points[2];
    Vec2f* D = &points[3];

    f32 minimapUnitPosX = unitPos->x;
    f32 minimapUnitPosY = unitPos->z;

    Vec2f* OffsetAB = dest;
    Math_Vec2f_Diff(B, A, OffsetAB);
    Math_Vec2f_Scale(OffsetAB, minimapUnitPosX);
    Math_Vec2f_Sum(A, OffsetAB, OffsetAB);

    Vec2f tmp;
    Vec2f* OffsetCD = &tmp;
    Math_Vec2f_Diff(D, C, OffsetCD);
    Math_Vec2f_Scale(OffsetCD, minimapUnitPosX);
    Math_Vec2f_Sum(C, OffsetCD, OffsetCD);

    Math_Vec2f_Scale(OffsetAB, 1 - minimapUnitPosY);
    Math_Vec2f_Scale(OffsetCD, minimapUnitPosY);

    Math_Vec2f_Sum(OffsetAB, OffsetCD, dest);
}

f32 get_y_offset(Vec2f* points, Vec3f* scenePos, Vec2f* unitPos, f32 scaleFrac) {
    Vec2f* A = &points[0];
    Vec2f* B = &points[1];
    Vec2f* C = &points[2];
    Vec2f* D = &points[3];

    Vec2f v1, v2;
    Vec2f* AB = &v1;
    Math_Vec2f_Diff(B, A, AB);
    Vec2f* CD = &v2;
    Math_Vec2f_Diff(D, C, CD);

    MapDataRoom* mapDataRoom = &sMapDisp.mapDataScene->rooms[sMapDisp.curRoom];
    s32 texOffsetX;
    s32 texOffsetY;
    s32 texWidth;
    s32 texHeight;
    MapDisp_GetMapOffset(mapDataRoom, &texOffsetX, &texOffsetY);
    MapDisp_GetMapTexDim(mapDataRoom, &texWidth, &texHeight);

    // Calculate differential of the inverse function for scene-to-world-map function.
    f32 div = (AB->x * CD->z  - AB->z * CD->x) * unitPos->x +
              ((CD->x - AB->x) * (C->z - A->z) - (CD->z - AB->z) * (C->x - A->x)) * unitPos->z +
              ((C->z - A->z) * AB->x - (C->x - A->x) * AB->z);
    div *= scaleFrac;

    f32 differential[2][2] = {
         texWidth * ((CD->z - AB->z) * unitPos->x + (C->z - A->z)),  -texWidth * ((CD->x - AB->x) * unitPos->x + (C->x - A->x)),
        -texHeight * ((CD->z - AB->z) * unitPos->z + AB->z),          texHeight * ((CD->x - AB->x) * unitPos->z + AB->x),
    };

    for (int i = 0; i < 4; i++) {
        differential[i / 2][i % 2] /= div;
    }

    // Get partial derivatives.
    Vec2f v3, v4;
    Vec2f* partial_derivative_dx = &v3;
    partial_derivative_dx->x = differential[0][0];
    partial_derivative_dx->z = differential[1][0];
    Vec2f* partial_derivative_dz = &v4;
    partial_derivative_dz->x = differential[0][1];
    partial_derivative_dz->z = differential[1][1];

    f32 x = Math_Vec2f_Len(partial_derivative_dx);
    f32 z = Math_Vec2f_Len(partial_derivative_dz);

    // Get cos and sin of the projection ray for Y axis.
    f32 cos = MIN(x / z, 1);
    f32 sin = sqrtf(1 - SQ(cos));

    f32 y_offset = ((scenePos->y - minimapToWorldMap->posY) / x) * sin;

    return y_offset;
}

bool map_overhaul_calculate_world_map_position(PlayState* play, Vec3f* pos, Vec2f* dest) {
    if (minimapToWorldMap == NULL) {
        return false;
    }

    MapDataRoom* mapDataRoom = &sMapDisp.mapDataScene->rooms[sMapDisp.curRoom];
    if (mapDataRoom->mapId == MAP_DATA_NO_MAP) {
        return false;
    }

    s32 texOffsetX;
    s32 texOffsetY;
    s32 texWidth;
    s32 texHeight;
    MapDisp_GetMapOffset(mapDataRoom, &texOffsetX, &texOffsetY);
    MapDisp_GetMapTexDim(mapDataRoom, &texWidth, &texHeight);

    s32 scale;
    if (sMapDisp.mapDataScene->scale == 0) {
        scale = 20;
    } else if (sMapDisp.mapDataScene->scale == -1) {
        s32 scaleTemp;

        MapDisp_GetMapScale(mapDataRoom, &scaleTemp);
        scale = scaleTemp;
    } else {
        scale = sMapDisp.mapDataScene->scale;
    }

    f32 minimapPosX;
    f32 minimapPosY;
    f32 scaleFrac = 1.0f / scale;
    if (!MapDisp_IsDataRotated(play)) {
        minimapPosX = ((pos->x - mapDataRoom->centerX) * scaleFrac) + texOffsetX;
        minimapPosY = ((pos->z - mapDataRoom->centerZ) * scaleFrac) + texOffsetY;
    } else {
        minimapPosX = -((pos->x - mapDataRoom->centerX) * scaleFrac) + texOffsetX;
        minimapPosY = -((pos->z - mapDataRoom->centerZ) * scaleFrac) + texOffsetY;
    }

    Vec2f unitPos = { minimapPosX / texWidth, minimapPosY / texHeight };

    // Calculate position on world map
    unit_to_world_map(&unitPos, minimapToWorldMap->points, dest);

#ifdef DEBUG
    recomp_printf("[ %f, %f, %f ],\n", pos->x, pos->y, pos->z);
#endif

    f32 offsetY = get_y_offset(minimapToWorldMap->points, pos, &unitPos, scaleFrac);

    Vec2f offsetVec = { 0, -offsetY }; //-(pos->y - CFG_MAP_Y_ZERO) * CFG_MAP_Y_SCALE};
    Math_Vec2f_Sum(dest, &offsetVec, dest);

    return true;
}

#ifdef DEBUG
void map_overhaul_print_minimap_params(PlayState* play) {
    MapDataRoom* mapDataRoom = &sMapDisp.mapDataScene->rooms[sMapDisp.curRoom];

    if (mapDataRoom->mapId == MAP_DATA_NO_MAP) {
        return;
    }

    s32 texOffsetX;
    s32 texOffsetY;
    s32 texWidth;
    s32 texHeight;
    MapDisp_GetMapOffset(mapDataRoom, &texOffsetX, &texOffsetY);
    MapDisp_GetMapTexDim(mapDataRoom, &texWidth, &texHeight);

    s32 scale;
    if (sMapDisp.mapDataScene->scale == 0) {
        scale = 20;
    } else if (sMapDisp.mapDataScene->scale == -1) {
        s32 scaleTemp;

        MapDisp_GetMapScale(mapDataRoom, &scaleTemp);
        scale = scaleTemp;
    } else {
        scale = sMapDisp.mapDataScene->scale;
    }

    f32 minimapPosX;
    f32 minimapPosY;
    f32 scaleFrac = 1.0f / scale;

    recomp_printf(
        "TexOffsetX: %d\n"
        "TexOffsetY: %d\n"
        "TexWidth: %d\n"
        "TexHeight: %d\n"
        "CenterX: %d\n"
        "CenterZ: %d\n"
        "Scale: %d\n"
        "DataRotated: %d\n"
        , texOffsetX, texOffsetY, texWidth, texHeight, mapDataRoom->centerX, mapDataRoom->centerZ, scale, MapDisp_IsDataRotated(play)
    );
}
#endif