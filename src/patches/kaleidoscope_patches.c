#include "map_overhaul.h"
#include "lib/math.h"

#include "PR/gbi.h"
#include "assets/interface/icon_item_field_static/icon_item_field_static.h"
#include "assets/archives/icon_item_24_static/icon_item_24_static_yar.h"

extern TexturePtr sCloudTextures[];
extern s16 sWorldMapDotPrimColors[][3];
extern s16 sWorldMapDotEnvColors[][3];
extern s16 sGreatFairySpawnRegions[];

RECOMP_HOOK("KaleidoScope_UpdateCursorSize") void on_KaleidoScope_UpdateCursorSize(PlayState* play) {
    map_overhul_update_cursor_position(play);
}

Vtx gMapFaceVtx[] = {
    VTX(-8,  8, 0,  0 << 5,  0 << 5, 0, 0, 0, 0xFF),
    VTX( 8,  8, 0, 16 << 5,  0 << 5, 0, 0, 0, 0xFF),
    VTX( 8, -8, 0, 16 << 5, 16 << 5, 0, 0, 0, 0xFF),
    VTX(-8, -8, 0,  0 << 5, 16 << 5, 0, 0, 0, 0xFF),
};

Vtx gMapDotVtx[] = {
    VTX(-4,  4, 0, 0 << 5, 0 << 5, 0, 0, 0, 0xFF),
    VTX( 4,  4, 0, 8 << 5, 0 << 5, 0, 0, 0, 0xFF),
    VTX( 4, -4, 0, 8 << 5, 8 << 5, 0, 0, 0, 0xFF),
    VTX(-4, -4, 0, 0 << 5, 8 << 5, 0, 0, 0, 0xFF),
};

Vtx gMapWarpVtx[] = {
    VTX(-12,  6, 0,  0 << 5,  0 << 5, 0, 0, 0, 0xFF),
    VTX( 12,  6, 0, 24 << 5,  0 << 5, 0, 0, 0, 0xFF),
    VTX( 12, -6, 0, 24 << 5, 12 << 5, 0, 0, 0, 0xFF),
    VTX(-12, -6, 0,  0 << 5, 12 << 5, 0, 0, 0, 0xFF),
};

RECOMP_IMPORT("*", void recomp_printf(char* temp, ...));

#define ZOOM_TRANSLATE_SPEED 2.5f
#define ZOOM_SCALE_SPEED 0.1f

#define DEFAULT_ZOOM_SCALE 2.0

#define MIN_ZOOM_SCALE 2.0
#define MAX_ZOOM_SCALE 6.0

#define MIN_ZOOM_TRANSLATION_X -108.0
#define MAX_ZOOM_TRANSLATION_X 108.0

#define MIN_ZOOM_TRANSLATION_Y -69.0
#define MAX_ZOOM_TRANSLATION_Y 59.0

static Vec2f DEFAULT_ZOOM_OFFSET = { 0.0, -5.0 };

Vec2f zoom_offset = { 0.0, 0.0 };
f32 zoom_scale = 1.0;

extern u16 sOwlWarpPauseItems[];

typedef enum WorldMapState {
    WORLD_MAP_STATE_DEFAULT,
    WORLD_MAP_STATE_TRANSITION,
    WORLD_MAP_STATE_ZOOM,
} WorldMapState;

u16 world_map_state = WORLD_MAP_STATE_DEFAULT;//WORLD_MAP_STATE_DEFAULT;

u16 transition_time;
Vec2f transition_target_zoom_offset;
f32 transition_target_zoom_scale;
u16 transition_target_state;
u16 transition_target_mainState;

RECOMP_PATCH void KaleidoScope_UpdateWorldMapCursor(PlayState* play) {
    static u16 sStickAdjTimer = 0; // unused timer that counts up every frame. Resets on reading a stickAdj.
    PauseContext* pauseCtx = &play->pauseCtx;
    s16 oldCursorPoint;
    Input* input = CONTROLLER1(&play->state);

    static u16 transition_timer = 0; // @mod

    if ((pauseCtx->state == PAUSE_STATE_MAIN) && (pauseCtx->pageIndex == PAUSE_MAP)) {
        switch (world_map_state) {
            case WORLD_MAP_STATE_ZOOM:
                ;
                s32 stickAdjX = input->rel.stick_x;
                s32 stickAdjY = input->rel.stick_y;

                if (stickAdjX != 0 || stickAdjY != 0) {
                    // recomp_printf("X: %d, Y: %d, Magnitude %f\n", stickAdjX, stickAdjY, stickAdjMagnitude);
                    zoom_offset.x -= (stickAdjX / 60.0f) * (ZOOM_TRANSLATE_SPEED / zoom_scale);
                    zoom_offset.x = CLAMP(zoom_offset.x, MIN_ZOOM_TRANSLATION_X, MAX_ZOOM_TRANSLATION_X);

                    zoom_offset.z += (stickAdjY / 60.0f) * (ZOOM_TRANSLATE_SPEED / zoom_scale);
                    zoom_offset.z = CLAMP(zoom_offset.z, MIN_ZOOM_TRANSLATION_Y, MAX_ZOOM_TRANSLATION_Y);
                } else {
                    sStickAdjTimer++;
                }

                s32 rStickAdjY;
                if (recomp_get_analog_cam_enabled()) {
                    float cameraX, cameraY;
                    recomp_get_camera_inputs(&cameraX, &cameraY);

                    rStickAdjY = cameraY * 60;
                } else {
                    rStickAdjY = CHECK_BTN_ANY(input->cur.button, BTN_CDOWN) * 60;
                    rStickAdjY -= CHECK_BTN_ANY(input->cur.button, BTN_CUP) * 60;
                }

                if (rStickAdjY != 0) {
                    zoom_scale -= (rStickAdjY / 60.0f) * (ZOOM_SCALE_SPEED);
                    zoom_scale = CLAMP(zoom_scale, MIN_ZOOM_SCALE, MAX_ZOOM_SCALE);
                }

                if (CHECK_BTN_ANY(input->press.button, BTN_B)) {
                    transition_time = 20;
                    transition_target_zoom_scale = 1.0;
                    transition_target_zoom_offset = (Vec2f){ 0.0, 0.0 };
                    transition_target_state = WORLD_MAP_STATE_DEFAULT;
                    transition_target_mainState = PAUSE_MAIN_STATE_IDLE;

                    world_map_state = WORLD_MAP_STATE_TRANSITION;
                    pauseCtx->mainState = PAUSE_MAIN_STATE_SONG_PLAYBACK_INIT;
                }

                break;

            case WORLD_MAP_STATE_TRANSITION:
                transition_timer++;

                if (transition_timer == transition_time) {
                    zoom_offset = transition_target_zoom_offset;
                    zoom_scale = transition_target_zoom_scale;
                    world_map_state = transition_target_state;
                    pauseCtx->mainState = transition_target_mainState;

                    transition_timer = 0;
                } else {
                    f32 progress = (f32)transition_timer / (f32)(transition_time + 1);
                    zoom_offset.x += (transition_target_zoom_offset.x - zoom_offset.x) * progress;
                    zoom_offset.z += (transition_target_zoom_offset.z - zoom_offset.z) * progress;
                    zoom_scale += (transition_target_zoom_scale - zoom_scale) * progress;
                }

                break;

            case WORLD_MAP_STATE_DEFAULT:
                if (pauseCtx->mainState == PAUSE_MAIN_STATE_IDLE) {
                    // @mod
                    if (CHECK_BTN_ANY(input->press.button, BTN_A)) {
                        transition_time = 20;
                        transition_target_zoom_scale = DEFAULT_ZOOM_SCALE;
                        transition_target_zoom_offset = DEFAULT_ZOOM_OFFSET; // TODO
                        transition_target_state = WORLD_MAP_STATE_ZOOM;
                        transition_target_mainState = PAUSE_MAIN_STATE_SONG_PLAYBACK_INIT;

                        world_map_state = WORLD_MAP_STATE_TRANSITION;
                        pauseCtx->mainState = PAUSE_MAIN_STATE_SONG_PLAYBACK_INIT;
                    }

                    pauseCtx->cursorColorSet = PAUSE_CURSOR_COLOR_SET_WHITE;
                    oldCursorPoint = pauseCtx->cursorPoint[PAUSE_WORLD_MAP];

                    if (gSaveContext.buttonStatus[EQUIP_SLOT_A] != BTN_DISABLED) {
                        gSaveContext.buttonStatus[EQUIP_SLOT_A] = BTN_DISABLED;
                        gSaveContext.hudVisibility = HUD_VISIBILITY_IDLE;
                        Interface_SetHudVisibility(HUD_VISIBILITY_ALL);
                    }

                    if (pauseCtx->cursorSpecialPos == 0) {
                        if (pauseCtx->stickAdjX > 30) {
                            pauseCtx->cursorShrinkRate = 4.0f;
                            sStickAdjTimer = 0;

                            while (true) {
                                pauseCtx->cursorPoint[PAUSE_WORLD_MAP]++;
                                if (pauseCtx->cursorPoint[PAUSE_WORLD_MAP] >= REGION_MAX) {
                                    KaleidoScope_MoveCursorToSpecialPos(play, PAUSE_CURSOR_PAGE_RIGHT);
                                    pauseCtx->cursorItem[PAUSE_MAP] = PAUSE_ITEM_NONE;
                                    break;
                                }
                                if (pauseCtx->worldMapPoints[pauseCtx->cursorPoint[PAUSE_WORLD_MAP]]) {
                                    break;
                                }
                            }
                        } else if (pauseCtx->stickAdjX < -30) {
                            pauseCtx->cursorShrinkRate = 4.0f;
                            sStickAdjTimer = 0;

                            while (true) {
                                pauseCtx->cursorPoint[PAUSE_WORLD_MAP]--;
                                if (pauseCtx->cursorPoint[PAUSE_WORLD_MAP] <= REGION_NONE) {
                                    KaleidoScope_MoveCursorToSpecialPos(play, PAUSE_CURSOR_PAGE_LEFT);
                                    pauseCtx->cursorItem[PAUSE_MAP] = PAUSE_ITEM_NONE;
                                    break;
                                }
                                if (pauseCtx->worldMapPoints[pauseCtx->cursorPoint[PAUSE_WORLD_MAP]]) {
                                    break;
                                }
                            }
                        } else {
                            sStickAdjTimer++;
                        }

                        if (pauseCtx->cursorSpecialPos == 0) {
                            pauseCtx->cursorItem[PAUSE_MAP] = pauseCtx->cursorPoint[PAUSE_WORLD_MAP];
                            // Used as cursor vtxIndex
                            pauseCtx->cursorSlot[PAUSE_MAP] = 31 + pauseCtx->cursorPoint[PAUSE_WORLD_MAP];
                        }
                    } else {
                        pauseCtx->cursorItem[PAUSE_MAP] = PAUSE_ITEM_NONE;
                        if (pauseCtx->cursorSpecialPos == PAUSE_CURSOR_PAGE_LEFT) {
                            if (pauseCtx->stickAdjX > 30) {
                                pauseCtx->cursorPoint[PAUSE_WORLD_MAP] = REGION_NONE;
                                pauseCtx->cursorSpecialPos = 0;
                                pauseCtx->cursorShrinkRate = 4.0f;

                                while (true) {
                                    pauseCtx->cursorPoint[PAUSE_WORLD_MAP]++;
                                    if (pauseCtx->cursorPoint[PAUSE_WORLD_MAP] >= REGION_MAX) {
                                        KaleidoScope_MoveCursorToSpecialPos(play, PAUSE_CURSOR_PAGE_RIGHT);
                                        pauseCtx->cursorItem[PAUSE_MAP] = PAUSE_ITEM_NONE;
                                        break;
                                    }
                                    if (pauseCtx->worldMapPoints[pauseCtx->cursorPoint[PAUSE_WORLD_MAP]]) {
                                        break;
                                    }
                                }

                                if (pauseCtx->cursorSpecialPos == 0) {
                                    pauseCtx->cursorItem[PAUSE_MAP] = pauseCtx->cursorPoint[PAUSE_WORLD_MAP];
                                    // Used as cursor vtxIndex
                                    pauseCtx->cursorSlot[PAUSE_MAP] = 31 + pauseCtx->cursorPoint[PAUSE_WORLD_MAP];
                                }
                                Audio_PlaySfx(NA_SE_SY_CURSOR);
                                sStickAdjTimer = 0;
                            }
                        } else if (pauseCtx->stickAdjX < -30) {
                            pauseCtx->cursorPoint[PAUSE_WORLD_MAP] = REGION_MAX;
                            pauseCtx->cursorSpecialPos = 0;
                            pauseCtx->cursorShrinkRate = 4.0f;

                            while (true) {
                                pauseCtx->cursorPoint[PAUSE_WORLD_MAP]--;
                                if (pauseCtx->cursorPoint[PAUSE_WORLD_MAP] <= REGION_NONE) {
                                    KaleidoScope_MoveCursorToSpecialPos(play, PAUSE_CURSOR_PAGE_LEFT);
                                    pauseCtx->cursorItem[PAUSE_MAP] = PAUSE_ITEM_NONE;
                                    break;
                                }
                                if (pauseCtx->worldMapPoints[pauseCtx->cursorPoint[PAUSE_WORLD_MAP]]) {
                                    break;
                                }
                            }

                            if (pauseCtx->cursorSpecialPos == 0) {
                                pauseCtx->cursorItem[PAUSE_MAP] = pauseCtx->cursorPoint[PAUSE_WORLD_MAP];
                                // Used as cursor vtxIndex
                                pauseCtx->cursorSlot[PAUSE_MAP] = 31 + pauseCtx->cursorPoint[PAUSE_WORLD_MAP];
                            }
                            Audio_PlaySfx(NA_SE_SY_CURSOR);
                            sStickAdjTimer = 0;
                        }
                    }

                    if (!pauseCtx->worldMapPoints[pauseCtx->cursorPoint[PAUSE_WORLD_MAP]]) {
                        pauseCtx->cursorItem[PAUSE_MAP] = PAUSE_ITEM_NONE;
                    }
                    if (oldCursorPoint != pauseCtx->cursorPoint[PAUSE_WORLD_MAP]) {
                        Audio_PlaySfx(NA_SE_SY_CURSOR);
                    }
                }
                break;
        }
    } else if (pauseCtx->state == PAUSE_STATE_OWL_WARP_SELECT) {
        pauseCtx->cursorColorSet = PAUSE_CURSOR_COLOR_SET_BLUE;
        oldCursorPoint = pauseCtx->cursorPoint[PAUSE_WORLD_MAP];

        if (pauseCtx->stickAdjX > 30) {
            pauseCtx->cursorShrinkRate = 4.0f;
            sStickAdjTimer = 0;
            do {
                pauseCtx->cursorPoint[PAUSE_WORLD_MAP]++;
                if (pauseCtx->cursorPoint[PAUSE_WORLD_MAP] > OWL_WARP_STONE_TOWER) {
                    pauseCtx->cursorPoint[PAUSE_WORLD_MAP] = OWL_WARP_GREAT_BAY_COAST;
                }
            } while (!pauseCtx->worldMapPoints[pauseCtx->cursorPoint[PAUSE_WORLD_MAP]]);
        } else if (pauseCtx->stickAdjX < -30) {
            pauseCtx->cursorShrinkRate = 4.0f;
            sStickAdjTimer = 0;
            do {
                pauseCtx->cursorPoint[PAUSE_WORLD_MAP]--;
                if (pauseCtx->cursorPoint[PAUSE_WORLD_MAP] < OWL_WARP_GREAT_BAY_COAST) {
                    pauseCtx->cursorPoint[PAUSE_WORLD_MAP] = OWL_WARP_STONE_TOWER;
                }
            } while (!pauseCtx->worldMapPoints[pauseCtx->cursorPoint[PAUSE_WORLD_MAP]]);
        } else {
            sStickAdjTimer++;
        }

        // Offset from `ITEM_MAP_POINT_GREAT_BAY` is to get the correct ordering in `map_name_static`
        pauseCtx->cursorItem[PAUSE_MAP] =
            sOwlWarpPauseItems[pauseCtx->cursorPoint[PAUSE_WORLD_MAP]] - ITEM_MAP_POINT_GREAT_BAY;
        // Used as cursor vtxIndex
        pauseCtx->cursorSlot[PAUSE_MAP] = 31 + pauseCtx->cursorPoint[PAUSE_WORLD_MAP];

        if (oldCursorPoint != pauseCtx->cursorPoint[PAUSE_WORLD_MAP]) {
            Audio_PlaySfx(NA_SE_SY_CURSOR);
        }
    }
}

RECOMP_PATCH void KaleidoScope_DrawWorldMap(PlayState* play) {
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

    // @mod Add support for zoom transformations.
    Matrix_Push();
    Matrix_Scale(zoom_scale, zoom_scale, 1.0f, MTXMODE_APPLY);
    Matrix_Translate(zoom_offset.x, -zoom_offset.z, 0.0f, MTXMODE_APPLY);
    MATRIX_FINALIZE_AND_LOAD(POLY_OPA_DISP++, play->state.gfxCtx);

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

    Vtx* print_vtx = &pauseCtx->mapPageVtx[(QUAD_MAP_PAGE_WORLD_IMAGE_FIRST + 0) * 4];
    // recomp_printf("pauseCtx->mainState: %d, X: %d, Y: %d, Z: %d\n", pauseCtx->mainState, print_vtx->v.ob[0], print_vtx->v.ob[1], print_vtx->v.ob[2]);
    // recomp_printf("zoom_scale: %f, zoom_offset_x: %f, zoom_offset_y: %f\n", zoom_scale, zoom_offset.x, zoom_offset.z);

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

    // @mod Add support for zoom-in transformations.
    Matrix_Pop();

    Gfx_SetupDL42_Opa(play->state.gfxCtx);

    // @mod
    gSPTexture(POLY_OPA_DISP++, 0xFFFF, 0xFFFF, 0, G_TX_RENDERTILE, G_ON);
    gSPClearGeometryMode(POLY_OPA_DISP++, G_CULL_BACK);
    gDPSetTextureFilter(POLY_OPA_DISP++, G_TF_POINT);

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
                // @mod Draw texture using transformation matrix and a quad for extra precision.
                Matrix_Push();

                gSPMatrix(POLY_OPA_DISP++, &gIdentityMtx, G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW);
                Matrix_Translate(
                    (new_sVtxPageMapWorldQuadsX[i] - SCREEN_CENTER_PX_X + zoom_offset.x) * zoom_scale,
                    (SCREEN_CENTER_PX_Y - new_sVtxPageMapWorldQuadsY[i] - zoom_offset.z) * zoom_scale,
                    0.0f, MTXMODE_APPLY);
                if (pauseCtx->state == PAUSE_STATE_UNPAUSE_SETUP) {
                    Matrix_Translate(0.0f, 80.0f, 0.0f, MTXMODE_APPLY);
                }
                MATRIX_FINALIZE_AND_LOAD(POLY_OPA_DISP++, play->state.gfxCtx);

                gSPVertex(POLY_OPA_DISP++, gMapDotVtx, 4, 0);
                gSP1Quadrangle(POLY_OPA_DISP++, 0, 1, 2, 3, 0);

                Matrix_Pop();
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
                // @mod Draw texture using transformation matrix and a quad for extra precision.
                Matrix_Push();

                gSPMatrix(POLY_OPA_DISP++, &gIdentityMtx, G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW);
                Matrix_Translate(
                    (new_sVtxPageMapWorldQuadsX[11 + i] - SCREEN_CENTER_PX_X + zoom_offset.x) * zoom_scale,
                    (SCREEN_CENTER_PX_Y - new_sVtxPageMapWorldQuadsY[11 + i] - zoom_offset.z) * zoom_scale,
                    0.0f, MTXMODE_APPLY);
                if (pauseCtx->state == PAUSE_STATE_OWL_WARP_3 || pauseCtx->state == PAUSE_STATE_OWL_WARP_6) {
                    Matrix_Translate(0.0f, 80.0f, 0.0f, MTXMODE_APPLY);
                }
                MATRIX_FINALIZE_AND_LOAD(POLY_OPA_DISP++, play->state.gfxCtx);

                gSPVertex(POLY_OPA_DISP++, gMapWarpVtx, 4, 0);
                gSP1Quadrangle(POLY_OPA_DISP++, 0, 1, 2, 3, 0);

                Matrix_Pop();

            }
        }
    }

    // Find and draw Player's face at the current region based on the current scene
    // If (pauseCtx->state == PAUSE_STATE_MAIN), then the other pauseCtx->state conditions are redundant
    // and always return true
    if ((pauseCtx->pageIndex == PAUSE_MAP) &&
        // @mod
        // (pauseCtx->mainState == PAUSE_MAIN_STATE_IDLE) &&
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

        if (n != REGION_MAX) {
            // @mod Calculate accurate player position.
            Vec2f playerPos;
            bool res = map_overhaul_minimap_unit_to_world_map_position(&map_overhaul_playerMinimapUnitPos, &playerPos);
            if (!res) {
                // Draw Player's face at the current region
                // @mod replace with custom function for extra float precision.
                map_overhaul_get_face_position(n, &playerPos);
            }

            KaleidoScope_SetView(pauseCtx, pauseCtx->eye.x, pauseCtx->eye.y, pauseCtx->eye.z);
            Gfx_SetupDL39_Opa(play->state.gfxCtx);
            gDPSetOtherMode(POLY_OPA_DISP++, G_AD_PATTERN | G_CD_MAGICSQ | G_CK_NONE | G_TC_FILT | G_TF_POINT | G_TT_NONE | G_TL_TILE |
                                G_TD_CLAMP | G_TP_PERSP | G_CYC_1CYCLE | G_PM_NPRIMITIVE,
                            G_AC_THRESHOLD | G_ZS_PIXEL | G_RM_XLU_SURF | G_RM_XLU_SURF2);

            gDPSetPrimColor(POLY_OPA_DISP++, 0, 0, 255, 255, 255, pauseCtx->alpha);

            gDPLoadTextureBlock(POLY_OPA_DISP++, gQuestIconLinkHumanFaceTex, G_IM_FMT_RGBA, G_IM_SIZ_16b, 16, 16, 0,
                                G_TX_NOMIRROR | G_TX_WRAP, G_TX_NOMIRROR | G_TX_WRAP, G_TX_NOMASK, G_TX_NOMASK, G_TX_NOLOD,
                                G_TX_NOLOD);

            gSPTexture(POLY_OPA_DISP++, 0xFFFF, 0xFFFF, 0, G_TX_RENDERTILE, G_ON);
            gSPClearGeometryMode(POLY_OPA_DISP++, G_CULL_BACK);

            // @mod Draw texture using transformation matrix and a quad for extra precision.
            Matrix_Push();

            gSPMatrix(POLY_OPA_DISP++, &gIdentityMtx, G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW);
            Matrix_Translate(
                (playerPos.x - SCREEN_CENTER_PX_X + zoom_offset.x) * zoom_scale,
                (SCREEN_CENTER_PX_Y - playerPos.z - zoom_offset.z) * zoom_scale,
                0.0f, MTXMODE_APPLY);
            MATRIX_FINALIZE_AND_LOAD(POLY_OPA_DISP++, play->state.gfxCtx);

            gSPVertex(POLY_OPA_DISP++, &gMapFaceVtx, 4, 0);
            gSP1Quadrangle(POLY_OPA_DISP++, 0, 1, 2, 3, 0);

            Matrix_Pop();
        }

    }

    gDPPipeSync(POLY_OPA_DISP++);

    CLOSE_DISPS(play->state.gfxCtx);
}

void KaleidoScope_SetVertices(PlayState* play, GraphicsContext* gfxCtx);
void KaleidoScope_DrawPages(PlayState* play, GraphicsContext* gfxCtx);
void KaleidoScope_UpdateCursorSize(PlayState* play);
void KaleidoScope_DrawCursor(PlayState* play);
void KaleidoScope_DrawGameOver(PlayState* play);
void KaleidoScope_DrawOwlWarpMapPage(PlayState* play);
void KaleidoScope_DrawOwlWarpInfoPanel(PlayState* play);
void KaleidoScope_DrawInfoPanel(PlayState* play);

// @recomp Patched to set pageIndex to a dummy value when KaleidoScope_SetVertices is called to make it
// allocate vertices for all pages at all times. This is simpler than patching KaleidoScope_SetVertices directly.
RECOMP_FORCE_PATCH void KaleidoScope_Draw(PlayState* play) {
    s32 pad;
    PauseContext* pauseCtx = &play->pauseCtx;
    InterfaceContext* interfaceCtx = &play->interfaceCtx;

    OPEN_DISPS(play->state.gfxCtx);

    gSPSegment(POLY_OPA_DISP++, 0x02, interfaceCtx->parameterSegment);
    gSPSegment(POLY_OPA_DISP++, 0x08, pauseCtx->iconItemSegment);
    gSPSegment(POLY_OPA_DISP++, 0x09, pauseCtx->iconItem24Segment);
    gSPSegment(POLY_OPA_DISP++, 0x0A, pauseCtx->nameSegment);
    gSPSegment(POLY_OPA_DISP++, 0x0C, pauseCtx->iconItemAltSegment);
    gSPSegment(POLY_OPA_DISP++, 0x0D, pauseCtx->iconItemLangSegment);
    gSPSegment(POLY_OPA_DISP++, 0x0B, pauseCtx->iconItemVtxSegment);

    if (pauseCtx->debugEditor == DEBUG_EDITOR_NONE) {
        KaleidoScope_SetView(pauseCtx, pauseCtx->eye.x, pauseCtx->eye.y, pauseCtx->eye.z);
        Gfx_SetupDL42_Opa(play->state.gfxCtx);

        if (!IS_PAUSE_STATE_OWL_WARP(pauseCtx)) {
            // Draw Default or Game Over Menus
            // @recomp Record the current pageIndex, then change it to a dummy value for the KaleidoScope_SetVertices.
            u16 saved_page_index = pauseCtx->pageIndex;
            pauseCtx->pageIndex = (u16)-1;
            KaleidoScope_SetVertices(play, play->state.gfxCtx);
            // @recomp Restore the old pageIndex value.
            pauseCtx->pageIndex = saved_page_index;
            KaleidoScope_DrawPages(play, play->state.gfxCtx);

            Gfx_SetupDL42_Opa(play->state.gfxCtx);
            gDPSetCombineLERP(POLY_OPA_DISP++, PRIMITIVE, ENVIRONMENT, TEXEL0, ENVIRONMENT, TEXEL0, 0, PRIMITIVE, 0,
                              PRIMITIVE, ENVIRONMENT, TEXEL0, ENVIRONMENT, TEXEL0, 0, PRIMITIVE, 0);

            KaleidoScope_SetView(pauseCtx, 0.0f, 0.0f, 64.0f);

            // @mod Disable drawing info panel for map zoom-in.
            if (!(IS_PAUSE_STATE_GAMEOVER(pauseCtx)) &&
                ((pauseCtx->pageIndex != PAUSE_MAP) || (world_map_state == WORLD_MAP_STATE_DEFAULT))) {
                KaleidoScope_DrawInfoPanel(play);
            }

            KaleidoScope_UpdateCursorSize(play);

            if (pauseCtx->state == PAUSE_STATE_MAIN) {
                KaleidoScope_DrawCursor(play);
            }

            if ((pauseCtx->state >= PAUSE_STATE_GAMEOVER_3) && (pauseCtx->state <= PAUSE_STATE_GAMEOVER_10) &&
                (play->gameOverCtx.state != GAMEOVER_INACTIVE)) {
                KaleidoScope_DrawGameOver(play);
            }
        } else {
            // Draw Owl Warp Menu
            // @recomp Record the current pageIndex, then change it to a dummy value for the KaleidoScope_SetVertices.
            u16 saved_page_index = pauseCtx->pageIndex;
            pauseCtx->pageIndex = (u16)-1;
            KaleidoScope_SetVertices(play, play->state.gfxCtx);
            // @recomp Restore the old pageIndex value.
            pauseCtx->pageIndex = saved_page_index;
            KaleidoScope_DrawPages(play, play->state.gfxCtx);
            KaleidoScope_DrawOwlWarpMapPage(play);

            Gfx_SetupDL42_Opa(play->state.gfxCtx);
            gDPSetCombineLERP(POLY_OPA_DISP++, PRIMITIVE, ENVIRONMENT, TEXEL0, ENVIRONMENT, TEXEL0, 0, PRIMITIVE, 0,
                              PRIMITIVE, ENVIRONMENT, TEXEL0, ENVIRONMENT, TEXEL0, 0, PRIMITIVE, 0);

            KaleidoScope_SetView(pauseCtx, 0.0f, 0.0f, 64.0f);
            KaleidoScope_DrawOwlWarpInfoPanel(play);
            KaleidoScope_UpdateCursorSize(play);

            if (pauseCtx->state == PAUSE_STATE_OWL_WARP_SELECT) {
                KaleidoScope_DrawCursor(play);
            }
        }
    }

    if ((pauseCtx->debugEditor == DEBUG_EDITOR_INVENTORY_INIT) || (pauseCtx->debugEditor == DEBUG_EDITOR_INVENTORY)) {
        KaleidoScope_DrawInventoryEditor(play);
    }

    CLOSE_DISPS(play->state.gfxCtx);
}