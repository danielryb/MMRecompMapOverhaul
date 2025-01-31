#include "map_overhaul.h"

RECOMP_HOOK("Map_InitRoomData") void on_Map_InitRoomData(PlayState* play, void* segmentAddress) {
    map_overhaul_reload_minimap_data();
}

RECOMP_HOOK("Map_Init") void on_Map_Init(PlayState* play, void* segmentAddress) {
    map_overhaul_reload_minimap_data();
}

#include "assets/interface/parameter_static/parameter_static.h"
#include "assets/objects/gameplay_keep/gameplay_keep.h"
#include "code/gDPLoadTextureBlock_Runtime.inc.c"

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

RECOMP_PATCH void MapDisp_Minimap_DrawActorIcon(PlayState* play, Actor* actor) {
    MapDataRoom* mapDataRoom;
    // @mod Change position calculations to float for extra precision.
    f32 posX;
    f32 posY;
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
    f32 mapPosX;
    f32 mapPosY;

    scaleFrac = 1.0f / scale;
    if (!MapDisp_IsDataRotated(play)) {
        mapPosX = ((actor->world.pos.x - mapDataRoom->centerX) * scaleFrac) + texOffsetX;
        mapPosY = ((actor->world.pos.z - mapDataRoom->centerZ) * scaleFrac) + texOffsetY;
    } else {
        mapPosX = -((actor->world.pos.x - mapDataRoom->centerX) * scaleFrac) + texOffsetX;
        mapPosY = -((actor->world.pos.z - mapDataRoom->centerZ) * scaleFrac) + texOffsetY;
    }
    posX = mapPosX + sMapDisp.minimapBaseX + sMapDisp.minimapCurX - sMapDisp.minimapBaseX;
    posY = mapPosY + sMapDisp.minimapBaseY + sMapDisp.minimapCurY - sMapDisp.minimapBaseY;

    // @mod Calculate cast to integer for calculations that depend on it.
    s32 _posX = (s32)posX;
    s32 _posY = (s32)posY;

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

            // @mod Save player's minimap unit position.
            map_overhaul_playerMinimapUnitPos.x = mapPosX / texWidth;
            map_overhaul_playerMinimapUnitPos.z = mapPosY / texHeight;
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

            // @mod Use position casted to integer.
            gSPTextureRectangle(OVERLAY_DISP++, (_posX - 4) << 2, (_posY - 4) << 2, (_posX + 4) << 2, (_posY + 4) << 2,
                                G_TX_RENDERTILE, 0, 0, 1 << 10, 1 << 10);
        } else {
            Gfx_SetupDL39_Overlay(play->state.gfxCtx);
            gDPSetCombineMode(OVERLAY_DISP++, G_CC_MODULATEIA_PRIM, G_CC_MODULATEIA_PRIM);
            if (actor->flags & ACTOR_FLAG_MINIMAP_ICON_ENABLED) {
                gDPSetPrimColor(OVERLAY_DISP++, 0, 0, sMinimapActorCategoryColors[actor->category].r,
                                sMinimapActorCategoryColors[actor->category].g,
                                sMinimapActorCategoryColors[actor->category].b, play->interfaceCtx.minimapAlpha);
                // @mod Use position casted to integer.
                gSPTextureRectangle(OVERLAY_DISP++, (_posX - 1) << 2, (_posY - 1) << 2, (_posX + 1) << 2, (_posY + 1) << 2,
                                    G_TX_RENDERTILE, 0, 0, 0x0001, 0x0001);
            }
        }
        CLOSE_DISPS(play->state.gfxCtx);
    }
}