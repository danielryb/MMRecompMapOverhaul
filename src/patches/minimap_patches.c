#include "map_overhaul.h"
#include "assets/interface/parameter_static/parameter_static.h"
#include "assets/objects/gameplay_keep/gameplay_keep.h"
#include "code/gDPLoadTextureBlock_Runtime.inc.c"

#ifdef DEBUG
PlayState* _play;
#endif

void onRoomEnter(PlayState* play, s16 room) {
    map_overhaul_reload_minimap_data(play, room);

#ifdef DEBUG
    _play = play;
#endif
}

RECOMP_HOOK("Map_InitRoomData") void on_Map_InitRoomData(PlayState* play, s16 room) {
    onRoomEnter(play, room);
}

RECOMP_HOOK("Map_Init") void on_Map_Init(PlayState* play) {
    onRoomEnter(play, play->roomCtx.curRoom.num);

#ifdef DEBUG
    s32 scene = ((void)0, gSaveContext.save.entrance) >> 9;
    u32 index = scene + 3;
    CycleSceneFlags* cycleFlags = &(gSaveContext.cycleSceneFlags[(index)]);

    recomp_printf("cycleFlags[%d].switches: 0x%08x, 0x%08x\n", index, cycleFlags->switch0, cycleFlags->switch1);
#endif
}

#ifdef DEBUG
RECOMP_HOOK_RETURN("Map_InitRoomData") void after_Map_InitRoomData() {
    map_overhaul_print_minimap_params(_play);
}

RECOMP_HOOK_RETURN("Map_Init") void after_Map_Init() {
    map_overhaul_print_minimap_params(_play);
}
#endif

extern MapDisp sMapDisp;

static Color_RGBA8 sMinimapActorCategoryColors[12] = {
    { 255, 255, 255, 255 }, { 255, 255, 255, 255 }, { 0, 255, 0, 255 },     { 255, 255, 255, 255 },
    { 255, 255, 255, 255 }, { 255, 0, 0, 255 },     { 255, 255, 255, 255 }, { 255, 255, 255, 255 },
    { 255, 255, 255, 255 }, { 255, 0, 0, 255 },     { 255, 255, 255, 255 }, { 255, 255, 255, 255 },
};

void MapDisp_GetMapOffset(MapDataRoom* mapDataRoom, s32* offsetX, s32* offsetY);
void MapDisp_GetMapTexDim(MapDataRoom* mapDataRoom, s32* width, s32* height);
void MapDisp_GetMapScale(MapDataRoom* mapDataRoom, s32* scale);
s32 MapDisp_IsDataRotated(PlayState* play);
f32 MapDisp_GetStoreyY(f32 checkY);

// #include "code/z_map_disp.c"

Vtx gMapEntityVtx[] = {
    VTX(-4,  4, 0, 0 << 6, 0 << 6, 128, 128, 128, 255),
    VTX( 4,  4, 0, 8 << 6, 0 << 6, 128, 128, 128, 255),
    VTX( 4, -4, 0, 8 << 6, 8 << 6, 128, 128, 128, 255),
    VTX(-4, -4, 0, 0 << 6, 8 << 6, 128, 128, 128, 255),
};

Gfx gMapEntityDL[] = {
    gsSPTexture(0xFFFF, 0xFFFF, 0, G_TX_RENDERTILE, G_ON),
    gsSPClearGeometryMode(G_CULL_BACK),
    gsDPSetTextureFilter(G_TF_POINT),

    gsSPVertex(gMapEntityVtx, 4, 0),
    gsSP1Quadrangle(0, 1, 2, 3, 0),
    gsSPEndDisplayList(),
};

RECOMP_PATCH void MapDisp_Minimap_DrawActorIcon(PlayState* play, Actor* actor) {
    MapDataRoom* mapDataRoom;
    f32 posX;   // @mod Change position calculations to float for extra precision.
    f32 posY;   //
    s32 texOffsetX;
    s32 texOffsetY;
    s32 texWidth;
    s32 texHeight;
    f32 scaleFrac;
    f32 unused1;
    f32 unused2;
    Player* player = GET_PLAYER(play);
    s32 scale;

    // inferred from `MapDisp_Minimap_DrawDoorActor`
    unused1 = fabsf(player->actor.world.pos.y - actor->world.pos.y);
    unused2 = 1.0f - (1 / 350.0f) * unused1;

    if (unused2 < 0.0f) {
        unused2 = 0.0f;
    }

    mapDataRoom = &sMapDisp.mapDataScene->rooms[sMapDisp.curRoom];
    if (mapDataRoom->mapId == MAP_DATA_NO_MAP) {
        return;
    }

    MapDisp_GetMapOffset(mapDataRoom, &texOffsetX, &texOffsetY);
    MapDisp_GetMapTexDim(mapDataRoom, &texWidth, &texHeight);

    scale = sMapDisp.mapDataScene->scale;
    if (sMapDisp.mapDataScene->scale == 0) {
        scale = 20;
    } else if (sMapDisp.mapDataScene->scale == -1) {
        s32 scaleTemp;

        MapDisp_GetMapScale(mapDataRoom, &scaleTemp);
        scale = scaleTemp;
    }

    // @mod Change position calculations to float for extra precision.
    scaleFrac = 1.0f / scale;
    if (!MapDisp_IsDataRotated(play)) {
        posX = ((actor->world.pos.x - mapDataRoom->centerX) * scaleFrac) + texOffsetX;
        posY = ((actor->world.pos.z - mapDataRoom->centerZ) * scaleFrac) + texOffsetY;
    } else {
        posX = -((actor->world.pos.x - mapDataRoom->centerX) * scaleFrac) + texOffsetX;
        posY = -((actor->world.pos.z - mapDataRoom->centerZ) * scaleFrac) + texOffsetY;
    }
    posX += sMapDisp.minimapBaseX + sMapDisp.minimapCurX - sMapDisp.minimapBaseX;
    posY += sMapDisp.minimapBaseY + sMapDisp.minimapCurY - sMapDisp.minimapBaseY;

    if ((posX > 0) && (posX < 0x3FF) && (posY > 0) && (posY < 0x3FF)) {
        OPEN_DISPS(play->state.gfxCtx);

        if ((actor->category == ACTORCAT_PLAYER) && (actor->flags & ACTOR_FLAG_MINIMAP_ICON_ENABLED)) {
            s16 compassRot;

            Gfx_SetupDL42_Overlay(play->state.gfxCtx);
            gSPMatrix(OVERLAY_DISP++, &gIdentityMtx, G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW);
            gDPSetCombineLERP(OVERLAY_DISP++, PRIMITIVE, ENVIRONMENT, TEXEL0, ENVIRONMENT, TEXEL0, 0, PRIMITIVE, 0,
                              PRIMITIVE, ENVIRONMENT, TEXEL0, ENVIRONMENT, TEXEL0, 0, PRIMITIVE, 0);
            gDPSetEnvColor(OVERLAY_DISP++, 0, 0, 0, play->interfaceCtx.minimapAlpha);
            gDPSetCombineMode(OVERLAY_DISP++, G_CC_PRIMITIVE, G_CC_PRIMITIVE);
            gDPSetRenderMode(OVERLAY_DISP++, G_RM_AA_DEC_LINE, G_RM_NOOP2);

            Matrix_Translate(posX - 160.0f, 120.0f - posY, 0.0f, MTXMODE_NEW);
            Matrix_RotateXFApply(-1.6f);
            compassRot = (s32)(0x7FFF - actor->focus.rot.y) / 1024;
            if (MapDisp_IsDataRotated(play)) {
                compassRot += 0x7FFF;
            }
            Matrix_RotateYF(compassRot / 10.0f, MTXMODE_APPLY);
            Matrix_Scale(0.4f, 0.4f, 0.4f, MTXMODE_APPLY);
            MATRIX_FINALIZE_AND_LOAD(OVERLAY_DISP++, play->state.gfxCtx);
            gDPSetPrimColor(OVERLAY_DISP++, 0, 0, 200, 255, 0, play->interfaceCtx.minimapAlpha);
            gSPDisplayList(OVERLAY_DISP++, gCompassArrowDL);
        } else if ((actor->id == ACTOR_EN_BOX) && !Flags_GetTreasure(play, actor->params & 0x1F) &&
                   (MapDisp_GetStoreyY(player->actor.world.pos.y) == MapDisp_GetStoreyY(actor->world.pos.y))) {
            Gfx_SetupDL39_Overlay(play->state.gfxCtx);
            gDPPipeSync(OVERLAY_DISP++);
            gDPSetTextureLUT(OVERLAY_DISP++, G_TT_NONE);
            gDPSetPrimColor(OVERLAY_DISP++, 0, 0, 255, 255, 255, play->interfaceCtx.minimapAlpha);
            gDPSetEnvColor(OVERLAY_DISP++, 0, 0, 0, play->interfaceCtx.minimapAlpha);
            gDPPipeSync(OVERLAY_DISP++);

            gDPLoadTextureBlock_Runtime(OVERLAY_DISP++, gMapChestIconTex, G_IM_FMT_RGBA, G_IM_SIZ_16b, 8, 8, 0,
                                        G_TX_NOMIRROR | G_TX_WRAP, G_TX_NOMIRROR | G_TX_WRAP, G_TX_NOMASK, G_TX_NOMASK,
                                        G_TX_NOLOD, G_TX_NOLOD);

            // @mod Draw texture using transformation matrix and a quad for extra precision.
            gSPMatrix(OVERLAY_DISP++, &gIdentityMtx, G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW);
            Matrix_Translate(posX - 160.0f, 120.0f - posY, 0.0f, MTXMODE_NEW);
            MATRIX_FINALIZE_AND_LOAD(OVERLAY_DISP++, play->state.gfxCtx);

            gSPDisplayList(OVERLAY_DISP++, gMapEntityDL);
        } else {
            Gfx_SetupDL39_Overlay(play->state.gfxCtx);
            gDPSetCombineMode(OVERLAY_DISP++, G_CC_MODULATEIA_PRIM, G_CC_MODULATEIA_PRIM);
            if (actor->flags & ACTOR_FLAG_MINIMAP_ICON_ENABLED) {
                gDPSetCombineLERP(OVERLAY_DISP++, PRIMITIVE, ENVIRONMENT, TEXEL0, ENVIRONMENT, TEXEL0, 0, PRIMITIVE, 0,
                                PRIMITIVE, ENVIRONMENT, TEXEL0, ENVIRONMENT, TEXEL0, 0, PRIMITIVE, 0);
                gDPSetEnvColor(OVERLAY_DISP++, 0, 0, 0, play->interfaceCtx.minimapAlpha);
                gDPSetRenderMode(OVERLAY_DISP++, G_RM_AA_DEC_LINE, G_RM_NOOP2);
                gDPSetPrimColor(OVERLAY_DISP++, 0, 0, sMinimapActorCategoryColors[actor->category].r,
                                sMinimapActorCategoryColors[actor->category].g,
                                sMinimapActorCategoryColors[actor->category].b, play->interfaceCtx.minimapAlpha);

                // @mod Draw texture using transformation matrix and a quad for extra precision.
                gSPMatrix(OVERLAY_DISP++, &gIdentityMtx, G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW);
                Matrix_Translate(posX - 160.0f, 120.0f - posY, 0.0f, MTXMODE_NEW);
                Matrix_Scale(0.25f, 0.25f, 0.25f, MTXMODE_APPLY);
                MATRIX_FINALIZE_AND_LOAD(OVERLAY_DISP++, play->state.gfxCtx);

                gSPDisplayList(OVERLAY_DISP++, gMapEntityDL);
            }
        }
        CLOSE_DISPS(play->state.gfxCtx);
    }
}