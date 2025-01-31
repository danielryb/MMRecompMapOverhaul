#ifndef __CONSTANTS_H__
#define __CONSTANTS_H__

// // x positions
// #define GREAT_BAY_X         65
// #define ZORA_HALL_X         87
// #define ROMANI_RANCH_X      132
// #define DEKU_PALACE_X       143
// #define WOODFALL_X          165
// #define CLOCK_TOWN_X        156
// #define SNOWHEAD_X          157
// #define IKANA_GRAVEYARD_X   191
// #define IKANA_CANYON_X      215
// #define GORON_VILLAGE_X     204
// #define STONE_TOWER_X       236

// #define OWL_WARP_GREAT_BAY_COAST_X  GREAT_BAY_X
// #define OWL_WARP_ZORA_CAPE_X        ZORA_HALL_X
// #define OWL_WARP_SNOWHEAD_X         SNOWHEAD_X
// #define OWL_WARP_MOUNTAIN_VILLAGE_X GORON_VILLAGE_X
// #define OWL_WARP_CLOCK_TOWN_X       CLOCK_TOWN_X
// #define OWL_WARP_MILK_ROAD_X        ROMANI_RANCH_X
// #define OWL_WARP_WOODFALL_X         WOODFALL_X
// #define OWL_WARP_SOUTHERN_SWAMP_X   DEKU_PALACE_X
// #define OWL_WARP_IKANA_CANYON_X     IKANA_CANYON_X
// #define OWL_WARP_STONE_TOWER_X      STONE_TOWER_X

// #define MAP_CENTER_PX_X 153

// // y positions
// #define GREAT_BAY_Y         113
// #define ZORA_HALL_Y         144
// #define ROMANI_RANCH_Y      134
// #define DEKU_PALACE_Y       170
// #define WOODFALL_Y          153
// #define CLOCK_TOWN_Y        123
// #define SNOWHEAD_Y          77
// #define IKANA_GRAVEYARD_Y   112
// #define IKANA_CANYON_Y      117
// #define GORON_VILLAGE_Y     73
// #define STONE_TOWER_Y       125

// #define OWL_WARP_GREAT_BAY_COAST_Y  GREAT_BAY_Y
// #define OWL_WARP_ZORA_CAPE_Y        ZORA_HALL_Y
// #define OWL_WARP_SNOWHEAD_Y         SNOWHEAD_Y
// #define OWL_WARP_MOUNTAIN_VILLAGE_Y GORON_VILLAGE_Y
// #define OWL_WARP_CLOCK_TOWN_Y       CLOCK_TOWN_Y
// #define OWL_WARP_MILK_ROAD_Y        ROMANI_RANCH_Y
// #define OWL_WARP_WOODFALL_Y         WOODFALL_Y
// #define OWL_WARP_SOUTHERN_SWAMP_Y   DEKU_PALACE_Y
// #define OWL_WARP_IKANA_CANYON_Y     IKANA_CANYON_Y
// #define OWL_WARP_STONE_TOWER_Y      STONE_TOWER_Y

// #define MAP_CENTER_PX_Y 113

// float positions' constants
#define PIXEL_TO_FLOAT_SCALE_X 1.005
#define PIXEL_TO_FLOAT_SCALE_Y -PIXEL_TO_FLOAT_SCALE_X
#define PIXEL_TO_FLOAT_PX_OFFSET_X 0.0
#define PIXEL_TO_FLOAT_PX_OFFSET_Y 6.0

#define MAP_TO_SCREEN_X ((104.0 / 132.0) / PIXEL_TO_FLOAT_SCALE_X)
#define MAP_TO_SCREEN_Y MAP_TO_SCREEN_X

// game objects
#define FACE_OFFSET_X 0.0
#define FACE_OFFSET_Y 1.0

#define DOT_RADIUS 4.0
#define DOT_OFFSET_X -DOT_RADIUS
#define DOT_OFFSET_Y DOT_RADIUS

#define CURSOR_OFFSET_X 2.77
#define CURSOR_OFFSET_Y (-CURSOR_OFFSET_X - 0.35)

#define OWL_WIDTH 24.0
#define OWL_HEIGHT 12.0
#define OWL_OFFSET_X (-OWL_WIDTH / 2.0)
#define OWL_OFFSET_Y (OWL_HEIGHT / 2.0 + 1.5)

#endif /* __CONSTANTS_H__ */