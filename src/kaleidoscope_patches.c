#include "map_overhaul.h"

#include "PR/gbi.h"
#include "assets/interface/icon_item_field_static/icon_item_field_static.h"
#include "assets/archives/icon_item_24_static/icon_item_24_static_yar.h"

extern TexturePtr sCloudTextures[];
extern s16 sWorldMapDotPrimColors[][3];
extern s16 sWorldMapDotEnvColors[][3];
extern s16 sGreatFairySpawnRegions[];

RECOMP_PATCH void KaleidoScope_DrawWorldMap(PlayState* play) {
    //@mod
    map_overhaul_update_map();

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

        if (n != REGION_MAX) {
            // @mod Calculate accurate player position
            Vec2f playerPos;
            bool res = map_overhaul_minimap_unit_to_world_map_position(&map_overhaul_playerMinimapUnitPos, &playerPos);
            if (!res) {
                // Draw Player's face at the current region
                playerPos.x = sWorldMapCursorsRectLeft[n];
                playerPos.z = sWorldMapCursorsRectTop[n];
            } else {
                playerPos.x += FACE_OFFSET_X;
                playerPos.z += FACE_OFFSET_Y;
            }

            KaleidoScope_SetView(pauseCtx, pauseCtx->eye.x, pauseCtx->eye.y, pauseCtx->eye.z);
            Gfx_SetupDL39_Opa(play->state.gfxCtx);

            gDPSetPrimColor(POLY_OPA_DISP++, 0, 0, 255, 255, 255, pauseCtx->alpha);

            POLY_OPA_DISP =
                Gfx_DrawTexRectRGBA16(POLY_OPA_DISP, gQuestIconLinkHumanFaceTex, 16, 16, playerPos.x,
                                    playerPos.z, 16, 16, 1 << 10, 1 << 10);
        }

    }

    gDPPipeSync(POLY_OPA_DISP++);

    CLOSE_DISPS(play->state.gfxCtx);
}