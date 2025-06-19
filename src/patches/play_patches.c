#include "map_overhaul.h"

#ifdef DEBUG
#define SCENE_COUNT 120

PermanentSceneFlags permanentSceneFlagsData[SCENE_COUNT];
#define PERM_SCENE_FLAGS(index) (gSaveContext.save.saveInfo.permanentSceneFlags[(index)])

CycleSceneFlags cycleSceneFlagsData[SCENE_COUNT];
#define CYCLE_SCENE_FLAGS(index) (gSaveContext.cycleSceneFlags[(index)])

#define COMPARE_DATA(data, flags, data_name, index) do {                                                    \
    u32 a = flags->data_name;                                                                               \
    u32 b = data[i].data_name;                                                                              \
    if (a != b) {                                                                                           \
        recomp_printf("%s[%d].%s differed: 0x%08x -> 0x%08x\n", #data, index, #data_name, b, a);            \
        data[i].data_name = a;                                                                              \
    }                                                                                                       \
} while(false)

#define WEEKREG_SIZE 100

u8 weekEventData[WEEKREG_SIZE];
#define _WEEKEVENTDATA(index) (weekEventData[(index)])
#define _GET_WEEKEVENTDATA(index) ((void)0, _WEEKEVENTDATA(index))

#define _CHECK_WEEKEVENTDATA(flag) (_WEEKEVENTDATA((flag) >> 8) & ((flag) & 0xFF))
#define _SET_WEEKEVENTDATA(flag) (_WEEKEVENTDATA((flag) >> 8) = _GET_WEEKEVENTDATA((flag) >> 8) | ((flag) & 0xFF))
#define _CLEAR_WEEKEVENTDATA(flag) (_WEEKEVENTDATA((flag) >> 8) = _GET_WEEKEVENTDATA((flag) >> 8) & (u8)~((flag) & 0xFF))
#endif

s16 sGreatFairySpawnRegions[] = {
    REGION_CLOCK_TOWN, REGION_WOODFALL, REGION_SNOWHEAD, REGION_GREAT_BAY, REGION_IKANA_CANYON,
    REGION_CLOCK_TOWN, REGION_WOODFALL, REGION_SNOWHEAD, REGION_GREAT_BAY, REGION_IKANA_CANYON,
};

// @mod Calculate player's world map position
RECOMP_HOOK("Play_DrawMain") void onPlay_DrawMain(PlayState* play) {
    if (R_PAUSE_BG_PRERENDER_STATE == PAUSE_BG_PRERENDER_SETUP) {
        s32 category;
        Actor* actor;
        Player* player = GET_PLAYER(play);
        ActorListEntry* entry;

        ActorContext* actorCtx = &play->actorCtx;

        entry = actorCtx->actorLists + 2;
        for (actor = entry->first; actor != NULL; actor = actor->next) {
            if (actor->id == ACTOR_PLAYER) {
                s16 j = 0;
                s16 n = 0;
                s16 sceneId = play->sceneId;

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

                // @mod Calculate accurate player position.
                bool res = map_overhaul_calculate_world_map_position(play, &actor->world.pos, &map_overhaul_playerWorldMapPos);
                if (!res) {
                    // Draw Player's face at the current region
                    // @mod replace with custom function for extra float precision.
                    map_overhaul_get_face_position(n, &map_overhaul_playerWorldMapPos);
                }
                break;
            }
        }
    }

#ifdef DEBUG
    for (u8 i = 0; i < WEEKREG_SIZE; i++) {
        for (u8 j = 0; j <= 7; j++) {
            u8 flag = 1 << j;

            u8 res = CHECK_WEEKEVENTREG(PACK_WEEKEVENTREG_FLAG(i, flag));
            if (res != _CHECK_WEEKEVENTDATA(PACK_WEEKEVENTREG_FLAG(i, flag))) {
                recomp_printf("weekevent %d, mask 0x%02x ", i, flag);
                if (res) {
                    _SET_WEEKEVENTDATA(PACK_WEEKEVENTREG_FLAG(i, flag));
                    recomp_printf("set\n");
                } else {
                    _CLEAR_WEEKEVENTDATA(PACK_WEEKEVENTREG_FLAG(i, flag));
                    recomp_printf("cleared\n");
                }
            }
        }
    }

    for (u8 i = 0; i < SCENE_COUNT; i++) {
        PermanentSceneFlags* permanentFlags = &PERM_SCENE_FLAGS(i);
        COMPARE_DATA(permanentSceneFlagsData, permanentFlags, chest, i);
        COMPARE_DATA(permanentSceneFlagsData, permanentFlags, switch0, i);
        COMPARE_DATA(permanentSceneFlagsData, permanentFlags, switch1, i);
        COMPARE_DATA(permanentSceneFlagsData, permanentFlags, clearedRoom, i);
        COMPARE_DATA(permanentSceneFlagsData, permanentFlags, collectible, i);
        COMPARE_DATA(permanentSceneFlagsData, permanentFlags, unk_14, i);
        COMPARE_DATA(permanentSceneFlagsData, permanentFlags, rooms, i);

        CycleSceneFlags* cycleFlags = &CYCLE_SCENE_FLAGS(i);
        COMPARE_DATA(cycleSceneFlagsData, cycleFlags, chest, i);
        COMPARE_DATA(cycleSceneFlagsData, cycleFlags, switch0, i);
        COMPARE_DATA(cycleSceneFlagsData, cycleFlags, switch1, i);
        COMPARE_DATA(cycleSceneFlagsData, cycleFlags, clearedRoom, i);
        COMPARE_DATA(cycleSceneFlagsData, cycleFlags, collectible, i);
    }
#endif
}