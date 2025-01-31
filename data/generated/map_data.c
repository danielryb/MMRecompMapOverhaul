// center

#define MAP_CENTER_PX_X 152.000000
#define MAP_CENTER_PX_Y 118.000000

// cursor

#define GREAT_BAY_X 65.000002
#define GREAT_BAY_Y 113.000000

#define ZORA_HALL_X 87.000001
#define ZORA_HALL_Y 143.999999

#define ROMANI_RANCH_X 132.000000
#define ROMANI_RANCH_Y 134.000000

#define DEKU_PALACE_X 143.000000
#define DEKU_PALACE_Y 169.999998

#define WOODFALL_X 165.000001
#define WOODFALL_Y 152.999999

#define CLOCK_TOWN_X 156.000000
#define CLOCK_TOWN_Y 123.000000

#define SNOWHEAD_X 157.000000
#define SNOWHEAD_Y 77.000000

#define IKANA_GRAVEYARD_X 191.000001
#define IKANA_GRAVEYARD_Y 112.000000

#define IKANA_CANYON_X 215.000002
#define IKANA_CANYON_Y 117.000000

#define GORON_VILLAGE_X 204.000002
#define GORON_VILLAGE_Y 72.999998

#define STONE_TOWER_X 235.999999
#define STONE_TOWER_Y 125.000000

// warps

#define OWL_WARP_GREAT_BAY_COAST_X 90.020223
#define OWL_WARP_GREAT_BAY_COAST_Y 123.232880

#define OWL_WARP_ZORA_CAPE_X 81.678613
#define OWL_WARP_ZORA_CAPE_Y 147.535515

#define OWL_WARP_SNOWHEAD_X 157.000000
#define OWL_WARP_SNOWHEAD_Y 77.000000

#define OWL_WARP_MOUNTAIN_VILLAGE_X 159.412161
#define OWL_WARP_MOUNTAIN_VILLAGE_Y 93.759639

#define OWL_WARP_CLOCK_TOWN_X 155.780864
#define OWL_WARP_CLOCK_TOWN_Y 118.833819

#define OWL_WARP_MILK_ROAD_X 142.179013
#define OWL_WARP_MILK_ROAD_Y 132.337205

#define OWL_WARP_WOODFALL_X 165.000001
#define OWL_WARP_WOODFALL_Y 152.999999

#define OWL_WARP_SOUTHERN_SWAMP_X 156.451419
#define OWL_WARP_SOUTHERN_SWAMP_Y 141.691300

#define OWL_WARP_IKANA_CANYON_X 213.428497
#define OWL_WARP_IKANA_CANYON_Y 114.617275

#define OWL_WARP_STONE_TOWER_X 235.999999
#define OWL_WARP_STONE_TOWER_Y 125.000000

// minimaps

minimap_map_t MiniToWorld_PiratesFortress[1] = {
    100.473721, 100.074587,
    110.581243, 94.236726,
    110.711227, 108.548589,
    119.334129, 97.287585,
};

minimap_map_t MiniToWorld_MilkRoad[1] = {
    138.917259, 132.469152,
    143.933800, 130.968188,
    138.111708, 134.279012,
    145.516155, 132.168644,
};

minimap_map_t MiniToWorld_PinnacleRock[1] = {
    55.170057, 117.589512,
    75.168328, 119.434709,
    63.201019, 109.271376,
    79.974474, 109.402278,
};

minimap_map_t MiniToWorld_TerminaField[1] = {
    133.208445, 107.277722,
    174.398151, 106.278612,
    117.752270, 138.754247,
    190.267691, 134.596284,
};

minimap_map_t MiniToWorld_RomaniRanch[1] = {
    129.827103, 128.588366,
    141.087762, 128.404823,
    126.990223, 137.263383,
    141.374705, 135.689043,
};

minimap_map_t MiniToWorld_GreatBayCoast[1] = {
    83.577787, 102.653671,
    121.024158, 101.511314,
    69.799366, 130.268021,
    126.259744, 132.411889,
};

minimap_map_t MiniToWorld_ZoraCape[1] = {
    70.459874, 133.180869,
    123.808744, 122.755188,
    76.626485, 175.988868,
    136.453322, 148.212045,
};

minimap_map_t MiniToWorld_PiratesFortressExterior[1] = {
    94.297357, 104.914168,
    100.164082, 99.823550,
    102.944950, 110.166045,
    108.779519, 105.850632,
};

minimap_map_t MiniToWorld_RoadToSouthernSwamp[1] = {
    156.176745, 133.387176,
    162.236724, 132.461425,
    158.452379, 139.260240,
    166.580094, 137.345161,
};

minimap_map_t MiniToWorld_WaterfallRapids[1] = {
    112.090162, 164.790825,
    107.042117, 158.664215,
    127.555887, 144.851737,
    124.690113, 142.647217,
};

minimap_map_t MiniToWorld_GormanTrack[1] = {
    145.282739, 132.446372,
    147.875954, 137.678622,
    139.902069, 132.601210,
    140.747538, 140.163505,
};

minimap_map_t* MiniToWorld[ENTR_SCENE_MAX] = {
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    MiniToWorld_PiratesFortress,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    MiniToWorld_MilkRoad,
    NULL,
    NULL,
    MiniToWorld_PinnacleRock,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    MiniToWorld_TerminaField,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    MiniToWorld_RomaniRanch,
    NULL,
    MiniToWorld_GreatBayCoast,
    MiniToWorld_ZoraCape,
    NULL,
    NULL,
    MiniToWorld_PiratesFortressExterior,
    NULL,
    NULL,
    NULL,
    NULL,
    MiniToWorld_RoadToSouthernSwamp,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    MiniToWorld_WaterfallRapids,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    MiniToWorld_GormanTrack,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
};