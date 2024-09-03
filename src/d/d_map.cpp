//
// Generated by dtk
// Translation Unit: d_map.cpp
//

#include "d/d_map.h"
#include "d/d_com_inf_game.h"
#include "d/d_stage.h"

enum {
    Floor_B5F = 123,
    Floor_B4F = 124,
    Floor_B3F = 125,
    Floor_B2F = 126,
    Floor_B1F = 127,
    Floor_1F = 128,
    Floor_2F = 129,
    Floor_3F = 130,
    Floor_4F = 131,
    Floor_5F = 132,

    Floor_Base = Floor_B5F,
};

#define Floor_Num (Floor_5F - Floor_B5F + 1)
#define Floor_Valid(no) (no >= 0) && (no < Floor_Num)

static inline int gridPos2GridNo(int i_gridX, int i_gridY) {
    JUT_ASSERT(0x4a4, (i_gridX >= -3) && (i_gridX <= 3) && (i_gridY >= -3) && (i_gridY <= 3));
    return i_gridX + 3 + (i_gridY + 3) * 7;
}

/* 800455AC-80045660       .text onSaveArriveGridForAgbUseGridPos__Fii */
void onSaveArriveGridForAgbUseGridPos(int i_gridX, int i_gridY) {
    dComIfGs_onSaveArriveGridForAgb(gridPos2GridNo(i_gridX, i_gridY));
}

/* 80045660-80045714       .text isSaveArriveGridForAgbUseGridPos__Fii */
BOOL isSaveArriveGridForAgbUseGridPos(int i_gridX, int i_gridY) {
    dComIfGs_isSaveArriveGridForAgb(gridPos2GridNo(i_gridX, i_gridY));
}

/* 80045714-80045734       .text IsFloorNo__Fi */
bool IsFloorNo(int i_no) {
    return i_no >= Floor_B5F && i_no <= Floor_5F;
}

/* 80045734-800457B8       .text getFloorInfo_WithRoom__Fi */
dStage_FloorInfo_c* getFloorInfo_WithRoom(int i_roomNo) {
    dStage_FloorInfo_c* floor = NULL;
    if (i_roomNo >= 0) {
        dStage_roomDt_c* room = dComIfGp_roomControl_getStatusRoomDt(i_roomNo);
        if (room != NULL)
            floor = room->getFloorInfo();
    }
    if (floor == NULL)
        floor = dComIfGp_getStage().getFloorInfo();
    return floor;
}

/* 800457B8-800458E0       .text mapOffsetY__Fv */
f32 mapOffsetY() {
    f32 ret = 0.0f;
    if (dStage_stagInfo_GetSTType(dComIfGp_getStageStagInfo()) == dStageType_BOSS_e || dStage_stagInfo_GetSTType(dComIfGp_getStageStagInfo()) == dStageType_MINIBOSS_e) {
        if (dComIfGp_getStage().getDMap() != NULL) {
            dStage_DMap_c* pinf = dComIfGp_getStage().getDMap();
            dStage_DMap_entry_c* entry = pinf->entry;
            JUT_ASSERT(0x587, pinf->num == 1);
            for (int i = 0; i < pinf->num; i++, entry++)
                ret = entry->offsetY;
        }
    }
    return ret;
}

/* 800458E0-800459E4       .text dMap_GetTopBottomFloorNo__FP11dStage_dt_cPUcPUc */
BOOL dMap_GetTopBottomFloorNo(dStage_dt_c* stag, u8* bottom_p, u8* top_p) {
    if (stag == NULL)
        return FALSE;

    u8 floorNo;
    u8 bottom;
    u8 top;

    dStage_FloorInfo_c* floor = stag->getFloorInfo();
    dStage_FloorInfo_entry_c* entry;

    if (floor != NULL && floor->num >= 1) {
        entry = floor->entry;
        bottom = Floor_B5F;
        top = Floor_5F;
        for (int i = 0; i < floor->num; i++, entry++) {
            for (int j = 0; j < ARRAY_SIZE(entry->field_0x05); j++) {
                if (entry->field_0x05[j] != -1) {
                    floorNo = entry->floorNo;
                    if (floorNo > bottom)
                        bottom = floorNo;
                    if (floorNo < top)
                        top = floorNo;
                    break;
                }
            }
        }
    } else {
        bottom = Floor_1F;
        top = Floor_1F;
    }

    if (bottom_p != NULL)
        *bottom_p = bottom;
    if (top_p != NULL)
        *top_p = top;

    return FALSE;
}

/* 800459E4-80045A98       .text dMap_GetFloorInfoDtP__FP18dStage_FloorInfo_cf */
void dMap_GetFloorInfoDtP(dStage_FloorInfo_c* floor, f32 ret) {
    /* Nonmatching */
}

/* 80045A98-80045AEC       .text dMap_GetFloorInfoDtPFromFloorNo__FP18dStage_FloorInfo_cUc */
dStage_FloorInfo_entry_c* dMap_GetFloorInfoDtPFromFloorNo(dStage_FloorInfo_c* floor, u8 i_floorNo) {
    /* Nonmatching */
    dStage_FloorInfo_entry_c* entry;
    BOOL valid = FALSE;

    if (floor != NULL) {
        entry = floor->entry;
        for (int i = 0; i < floor->num; i++, entry++) {
            if (entry->floorNo == i_floorNo) {
                valid = TRUE;
                break;
            }
        }
    }

    if (!valid)
        entry = NULL;
    return entry;
}

/* 80045AEC-80045C24       .text dMap_GetFloorNoForDmap__FP11dStage_dt_cif */
void dMap_GetFloorNoForDmap(dStage_dt_c*, int, f32) {
    /* Nonmatching */
}

/* 80045C24-80045C6C       .text dMap_GetFloorNo_WithRoom__Fif */
void dMap_GetFloorNo_WithRoom(int, f32) {
    /* Nonmatching */
}

/* 80045C6C-80045C90       .text dMap_GetFloorNo__FP11dStage_dt_cf */
void dMap_GetFloorNo(dStage_dt_c*, f32) {
    /* Nonmatching */
}

/* 80045C90-80045E54       .text getRoomDspFloorNo__15dMap_RoomInfo_cFUci */
u32 dMap_RoomInfo_c::getRoomDspFloorNo(u8 i_no, BOOL search) {
    /* Nonmatching */
    s32 no = i_no - Floor_Base;
    JUT_ASSERT(0x702, Floor_Valid(no));

    s32 dspFloorNo = field_0x2[no];
    if (search) {
        if (!IsFloorNo(dspFloorNo)) {
            while (!IsFloorNo(dspFloorNo) && --no >= 0) {
                JUT_ASSERT(0x70f, Floor_Valid(no));
                dspFloorNo = field_0x2[no];
            }
        }

        if (!IsFloorNo(dspFloorNo)) {
            while (!IsFloorNo(dspFloorNo) && ++no <= (Floor_Num - 1)) {
                JUT_ASSERT(0x718, Floor_Valid(no));
                dspFloorNo = field_0x2[no];
            }
        }

        if (!IsFloorNo(dspFloorNo))
            dspFloorNo = 255;
    }

    return dspFloorNo;
}

/* 80045E54-80045F40       .text init__15dMap_RoomInfo_cFP15dMap_RoomInfo_ci */
dMap_RoomInfo_c* dMap_RoomInfo_c::init(dMap_RoomInfo_c* prev, int p2) {
    m_exist = 0;
    field_0x1 = 0;
    for (int i = 0; i < ARRAY_SIZE(field_0x2); i++)
        field_0x2[i] = 255;
    field_0xc = 255;
    m_no = -1;
    field_0x14 = p2;
    field_0x18 = 0.0f;
    field_0x1c = 0.0f;
    field_0x20 = 0.0f;
    field_0x24 = 0.0f;
    field_0x8c.init(NULL, NULL, 0.0f, 0.0f, 0, 0, 0, 0, 1.0f, 1.0f, 0);
    field_0x44[0].field_0x0[0] = 0;
    field_0x30.init(1, &field_0x44[0]);
    m_next = NULL;
    mStageMapInfoP = NULL;
    if (prev != NULL)
        prev->m_next = this;
    return this;
}

/* 80045F40-80046314       .text getRoomImage__15dMap_RoomInfo_cFiUciPP7ResTIMGPP7ResTIMGPP8map_dt_cPP20stage_map_info_classPUc */
void dMap_RoomInfo_c::getRoomImage(int, u8, int, ResTIMG**, ResTIMG**, map_dt_c**, stage_map_info_class**, u8*) {
    /* Nonmatching */
}

/* 80046314-80046470       .text makeRoomDspFloorNoTbl__15dMap_RoomInfo_cFi */
void dMap_RoomInfo_c::makeRoomDspFloorNoTbl(int) {
    /* Nonmatching */
}

/* 80046470-80046A58       .text roomEntryRoom__15dMap_RoomInfo_cFiUciUcP15dMap_RoomInfo_cssf */
u32 dMap_RoomInfo_c::roomEntryRoom(int, u8, int, u8, dMap_RoomInfo_c*, s16, s16, f32) {
    /* Nonmatching */
}

/* 80046A58-80046F08       .text Changeimage__15dMap_RoomInfo_cFUcUcissf */
void dMap_RoomInfo_c::Changeimage(u8, u8, int, s16, s16, f32) {
    /* Nonmatching */
}

/* 80046F08-80046FE0       .text deleteRoom__15dMap_RoomInfo_cFv */
BOOL dMap_RoomInfo_c::deleteRoom() {
    m_no = -1;
    m_exist = 0;
    mStageMapInfoP = NULL;
    field_0x1 = 0;
    for (int i = 0; i < ARRAY_SIZE(field_0x2); i++)
        field_0x2[i] = 255;
    field_0xc = 255;
    m_no = -1;
    field_0x18 = 0.0f;
    field_0x1c = 0.0f;
    field_0x20 = 0.0f;
    field_0x24 = 0.0f;
    field_0x28 = 0.0f;
    field_0x2c = 0.0f;
    field_0x8c.init(NULL, NULL, 0.0f, 0.0f, 0, 0, 0, 0, 1.0f, 1.0f, 0);
    field_0x44[0].field_0x0[0] = 0;
    field_0x30.init(1, &field_0x44[0]);
    return TRUE;
}

/* 80046FE0-800470CC       .text enlagementSizeTextureCordCalc__15dMap_RoomInfo_cFPfPfPfPfffffff */
void dMap_RoomInfo_c::enlagementSizeTextureCordCalc(f32*, f32*, f32*, f32*, f32, f32, f32, f32, f32, f32) {
    /* Nonmatching */
}

/* 800470CC-800475B8       .text roomDrawRoomEnlargementSize__15dMap_RoomInfo_cFiiiiffffUc */
void dMap_RoomInfo_c::roomDrawRoomEnlargementSize(int, int, int, int, f32, f32, f32, f32, u8) {
    /* Nonmatching */
}

/* 800475B8-80047834       .text roomDrawRoomRealSize__15dMap_RoomInfo_cFiiiiffffffUc */
void dMap_RoomInfo_c::roomDrawRoomRealSize(int, int, int, int, f32, f32, f32, f32, f32, f32, u8) {
    /* Nonmatching */
}

/* 80047834-8004793C       .text roomExistenceCheck__19dMap_RoomInfoCtrl_cFiPP15dMap_RoomInfo_c */
bool dMap_RoomInfoCtrl_c::roomExistenceCheck(int i_no, dMap_RoomInfo_c** roomInfoPP) {
    /* Nonmatching */
    JUT_ASSERT(0xb83, roomInfoPP != NULL);
    bool ret = false;
    *roomInfoPP = NULL;
    dMap_RoomInfo_c* roomInfoP = m_info;
    JUT_ASSERT(0xb8b, roomInfoP != NULL);
    for (; !ret && roomInfoP != NULL; roomInfoP = roomInfoP->m_next) {
        if (roomInfoP->m_exist) {
            if (i_no == roomInfoP->m_no) {
                *roomInfoPP = roomInfoP;
                ret = TRUE;
            }
        } else {
            if (*roomInfoPP == NULL)
                *roomInfoPP = roomInfoP;
        }
    }
    return ret;
}

/* 8004793C-80047960       .text getNextRoomP__19dMap_RoomInfoCtrl_cFP15dMap_RoomInfo_c */
dMap_RoomInfo_c* dMap_RoomInfoCtrl_c::getNextRoomP(dMap_RoomInfo_c* info) {
    if (info == NULL) {
        if (m_info != NULL)
            return m_info;
        else
            return NULL;
    } else {
        return info->m_next;
    }
}

/* 80047960-80047A8C       .text ctrlEntryRoom__19dMap_RoomInfoCtrl_cFiUciUcssf */
dMap_RoomInfo_c* dMap_RoomInfoCtrl_c::ctrlEntryRoom(int i_no, u8 p1, int p2, u8 p3, s16 p4, s16 p5, f32 p6) {
    dMap_RoomInfo_c* roomInfoP;
    if (roomExistenceCheck(i_no, &roomInfoP))
        return NULL;

    dMap_RoomInfo_c* prev = NULL;
    roomInfoP = m_info;
    JUT_ASSERT(0xbfd, roomInfoP != NULL);

    if (roomInfoP == NULL)
        return NULL;

    for (; roomInfoP != NULL && roomInfoP->m_exist; roomInfoP = roomInfoP->m_next)
        prev = roomInfoP;

    if (roomInfoP != NULL) {
        if (roomInfoP->roomEntryRoom(i_no, p1, p2, p3, prev, p4, p5, p6))
            return roomInfoP;
    }

    return NULL;
}

/* 80047A8C-80047AC4       .text deleteRoom__19dMap_RoomInfoCtrl_cFi */
bool dMap_RoomInfoCtrl_c::deleteRoom(int i_no) {
    dMap_RoomInfo_c* roomInfoP;
    if (roomExistenceCheck(i_no, &roomInfoP))
        roomInfoP->deleteRoom();
    return FALSE;
}

/* 80047AC4-80047B8C       .text ctrlDrawRoomEnlargementSize__19dMap_RoomInfoCtrl_cFiiiiiffffUc */
void dMap_RoomInfoCtrl_c::ctrlDrawRoomEnlargementSize(int i_no, int p1, int p2, int p3, int p4, f32 p5, f32 p6, f32 p7, f32 p8, u8 p9) {
    dMap_RoomInfo_c* roomInfoP = NULL;
    while ((roomInfoP = getNextRoomP(roomInfoP))) {
        if (roomInfoP->m_exist && i_no == roomInfoP->m_no)
            roomInfoP->roomDrawRoomEnlargementSize(p1, p2, p3, p4, p5, p6, p7, p8, p9);
    }
}

/* 80047B8C-80047C64       .text ctrlDrawRoomRealSize__19dMap_RoomInfoCtrl_cFiiiiiffffffUc */
void dMap_RoomInfoCtrl_c::ctrlDrawRoomRealSize(int i_no, int p1, int p2, int p3, int p4, f32 p5, f32 p6, f32 p7, f32 p8, f32 p9, f32 p10, u8 p11) {
    dMap_RoomInfo_c* roomInfoP = NULL;
    while ((roomInfoP = getNextRoomP(roomInfoP))) {
        if (roomInfoP->m_exist && i_no == roomInfoP->m_no)
            roomInfoP->roomDrawRoomRealSize(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, p11);
    }
}

/* 80047C64-80047CD0       .text init__19dMap_RoomInfoCtrl_cFv */
void dMap_RoomInfoCtrl_c::init() {
    /* Nonmatching */
    int no = 0;
    dMap_RoomInfo_c* info = m_info;
    dMap_RoomInfo_c* prev = NULL;
    for (int i = 0; i < m_num; i++, no++, info++) {
        info->init(prev, no);
        prev = info;
    }
}

/* 80047CD0-8004826C       .text create__6dMap_cFv */
void dMap_c::create() {
    /* Nonmatching */
}

/* 8004826C-800482B4       .text isEnableEnlargementScroll__6dMap_cFv */
void dMap_c::isEnableEnlargementScroll() {
    /* Nonmatching */
}

/* 800482B4-800482D8       .text isEnableDispMap__6dMap_cFv */
bool dMap_c::isEnableDispMap() {
    /* Nonmatching */
}

/* 800482D8-80048340       .text getKindMapType__6dMap_cFv */
void dMap_c::getKindMapType() {
    /* Nonmatching */
}

/* 80048340-80048370       .text remove__6dMap_cFv */
void dMap_c::remove() {
    /* Nonmatching */
}

/* 80048370-800484A4       .text setImage__6dMap_cFiif */
void dMap_c::setImage(int, int, f32) {
    /* Nonmatching */
}

/* 800484A4-800484F4       .text deleteImage__6dMap_cFi */
void dMap_c::deleteImage(int) {
    /* Nonmatching */
}

/* 800484F4-80048660       .text setNowRoom__6dMap_cFi */
void dMap_c::setNowRoom(int) {
    /* Nonmatching */
}

/* 80048660-80048A20       .text draw__11dMap_2DSQ_cFv */
void dMap_2DSQ_c::draw() {
    /* Nonmatching */
}

/* 80048A20-80048B54       .text mapDrawIconFree__6dMap_cFssUc */
void dMap_c::mapDrawIconFree(s16, s16, u8) {
    /* Nonmatching */
}

/* 80048B54-80048C80       .text mapDrawIconSelf__6dMap_cFssUc */
void dMap_c::mapDrawIconSelf(s16, s16, u8) {
    /* Nonmatching */
}

/* 80048C80-80048D80       .text mapDrawFrame__6dMap_cFUc */
void dMap_c::mapDrawFrame(u8) {
    /* Nonmatching */
}

/* 80048D80-80048F74       .text mapDrawEnlargementSize__6dMap_cFffffUc */
void dMap_c::mapDrawEnlargementSize(f32, f32, f32, f32, u8) {
    /* Nonmatching */
}

/* 80048F74-80049354       .text mapDrawRealSize__6dMap_cFffUc */
void dMap_c::mapDrawRealSize(f32, f32, u8) {
    /* Nonmatching */
}

/* 80049354-800494A0       .text mapAGBSendIslandData__6dMap_cFv */
void dMap_c::mapAGBSendIslandData() {
    /* Nonmatching */
}

/* 800494A0-800494C8       .text setPlayerStayAgbMapTypeNow__6dMap_cFff */
void dMap_c::setPlayerStayAgbMapTypeNow(f32, f32) {
    /* Nonmatching */
}

/* 800494C8-80049538       .text agbMapNoSet__6dMap_cFUcUc */
void dMap_c::agbMapNoSet(u8, u8) {
    /* Nonmatching */
}

/* 80049538-800495D8       .text agbMapNoSetCall__6dMap_cFv */
void dMap_c::agbMapNoSetCall() {
    /* Nonmatching */
}

/* 800495D8-800495F8       .text agbIsMsgSend__6dMap_cFv */
void dMap_c::agbIsMsgSend() {
    /* Nonmatching */
}

/* 800495F8-80049630       .text agbResetCursor__6dMap_cFv */
void dMap_c::agbResetCursor() {
    /* Nonmatching */
}

/* 80049630-80049650       .text agbScrollX__6dMap_cFv */
void dMap_c::agbScrollX() {
    /* Nonmatching */
}

/* 80049650-80049670       .text agbScrollY__6dMap_cFv */
void dMap_c::agbScrollY() {
    /* Nonmatching */
}

/* 80049670-800496B0       .text agbFlashCheck__6dMap_cFv */
void dMap_c::agbFlashCheck() {
    /* Nonmatching */
}

/* 800496B0-800496D0       .text agbIsActive__6dMap_cFv */
void dMap_c::agbIsActive() {
    /* Nonmatching */
}

/* 800496D0-80049B1C       .text isInDspArea__6dMap_cFffb */
void dMap_c::isInDspArea(f32, f32, bool) {
    /* Nonmatching */
}

/* 80049B1C-80049B64       .text mapAGBSendStatInit__6dMap_cFv */
void dMap_c::mapAGBSendStatInit() {
    /* Nonmatching */
}

/* 80049B64-80049F4C       .text mapAGBSendMapMain__6dMap_cFff */
void dMap_c::mapAGBSendMapMain(f32, f32) {
    /* Nonmatching */
}

/* 80049F4C-8004A3A4       .text calcEnlargementSizeParameter__6dMap_cFff */
void dMap_c::calcEnlargementSizeParameter(f32, f32) {
    /* Nonmatching */
}

/* 8004A3A4-8004A478       .text calcScissor__6dMap_cFv */
void dMap_c::calcScissor() {
    /* Nonmatching */
}

/* 8004A478-8004A6E8       .text mapMoveAll__6dMap_cFffif */
void dMap_c::mapMoveAll(f32, f32, int, f32) {
    /* Nonmatching */
}

/* 8004A6E8-8004A760       .text mapDrawAll__6dMap_cFffif */
void dMap_c::mapDrawAll(f32, f32, int, f32) {
    /* Nonmatching */
}

/* 8004A760-8004A7B4       .text mapDrawIcon__6dMap_cFv */
void dMap_c::mapDrawIcon() {
    /* Nonmatching */
}

/* 8004A7B4-8004A82C       .text draw__6dMap_cFffif */
void dMap_c::draw(f32, f32, int, f32) {
    /* Nonmatching */
}

/* 8004A82C-8004A97C       .text point2Grid__6dMap_cFffPScPSc */
void dMap_c::point2Grid(f32, f32, s8*, s8*) {
    /* Nonmatching */
}

/* 8004A97C-8004ABB0       .text point2GridAndLocal__6dMap_cFffPScPScPsPs */
void dMap_c::point2GridAndLocal(f32, f32, s8*, s8*, s16*, s16*) {
    /* Nonmatching */
}

/* 8004ABB0-8004AC44       .text getCheckPointUseGrid__6dMap_cFScSc */
void dMap_c::getCheckPointUseGrid(s8, s8) {
    /* Nonmatching */
}

/* 8004AC44-8004ACD8       .text getFmapChkPntPrm__6dMap_cFiPScPScPsPsPUc */
void dMap_c::getFmapChkPntPrm(int, s8*, s8*, s16*, s16*, u8*) {
    /* Nonmatching */
}

/* 8004ACD8-8004AD00       .text setFmapChkDtPrm__6dMap_cFv */
void dMap_c::setFmapChkDtPrm() {
    /* Nonmatching */
}

/* 8004AD00-8004ADC8       .text getFmapChkPntDtPnt__6dMap_cFi */
void dMap_c::getFmapChkPntDtPnt(int) {
    /* Nonmatching */
}

/* 8004ADC8-8004AE28       .text initPoint__6dMap_cFv */
void dMap_c::initPoint() {
    /* Nonmatching */
}

/* 8004AE28-8004B148       .text setGbaPoint_ocean__6dMap_cFUcffsUcUcUcUc */
void dMap_c::setGbaPoint_ocean(u8, f32, f32, s16, u8, u8, u8, u8) {
    /* Nonmatching */
}

/* 8004B148-8004B1D0       .text isPointStayInDspNowRoomAgbScr__6dMap_cFss */
void dMap_c::isPointStayInDspNowRoomAgbScr(s16, s16) {
    /* Nonmatching */
}

/* 8004B1D0-8004B33C       .text setCollectPoint__6dMap_cFUcUcfffScsUcUcUcUc */
void dMap_c::setCollectPoint(u8, u8, f32, f32, f32, s8, s16, u8, u8, u8, u8) {
    /* Nonmatching */
}

/* 8004B33C-8004B814       .text setGbaPoint_dungeon__6dMap_cFUcffsUcUcUcUc */
void dMap_c::setGbaPoint_dungeon(u8, f32, f32, s16, u8, u8, u8, u8) {
    /* Nonmatching */
}

/* 8004B814-8004B8A0       .text getPosAgbMapType__6dMap_cFffb */
void dMap_c::getPosAgbMapType(f32, f32, bool) {
    /* Nonmatching */
}

/* 8004B8A0-8004B8DC       .text setGbaPoint__6dMap_cFUcffsUcUcUcUc */
void dMap_c::setGbaPoint(u8, f32, f32, s16, u8, u8, u8, u8) {
    /* Nonmatching */
}

/* 8004B8DC-8004B9C8       .text setArriveInfo__6dMap_cFff */
void dMap_c::setArriveInfo(f32, f32) {
    /* Nonmatching */
}

/* 8004B9C8-8004BA64       .text drawPointPlayer__6dMap_cFffs */
void dMap_c::drawPointPlayer(f32, f32, s16) {
    /* Nonmatching */
}

/* 8004BA64-8004BB0C       .text drawPointEnemy__6dMap_cFff */
void dMap_c::drawPointEnemy(f32, f32) {
    /* Nonmatching */
}

/* 8004BB0C-8004BC94       .text drawPointAgbCursor__6dMap_cFff */
void dMap_c::drawPointAgbCursor(f32, f32) {
    /* Nonmatching */
}

/* 8004BC94-8004BD84       .text drawPointTbox__6dMap_cFffff */
void dMap_c::drawPointTbox(f32, f32, f32, f32) {
    /* Nonmatching */
}

/* 8004BD84-8004BE7C       .text drawPointDoor__6dMap_cFffffsUc */
void dMap_c::drawPointDoor(f32, f32, f32, f32, s16, u8) {
    /* Nonmatching */
}

/* 8004BE7C-8004BF78       .text drawPointRestart__6dMap_cFffsff */
void dMap_c::drawPointRestart(f32, f32, s16, f32, f32) {
    /* Nonmatching */
}

/* 8004BF78-8004C044       .text drawPointFriend__6dMap_cFfff */
void dMap_c::drawPointFriend(f32, f32, f32) {
    /* Nonmatching */
}

/* 8004C044-8004C144       .text drawPointShip__6dMap_cFffsff */
void dMap_c::drawPointShip(f32, f32, s16, f32, f32) {
    /* Nonmatching */
}

/* 8004C144-8004CC7C       .text drawPointGc__6dMap_cFUcfffScsUcUcUcUc */
void dMap_c::drawPointGc(u8, f32, f32, f32, s8, s16, u8, u8, u8, u8) {
    /* Nonmatching */
}

/* 8004CC7C-8004CD68       .text drawPointMain__6dMap_cFUcUcfffScsUcUcUcUc */
void dMap_c::drawPointMain(u8, u8, f32, f32, f32, s8, s16, u8, u8, u8, u8) {
    /* Nonmatching */
}

/* 8004CD68-8004CEF4       .text drawPointAgb__6dMap_cFUcfffScsUcUcUcUc */
void dMap_c::drawPointAgb(u8, f32, f32, f32, s8, s16, u8, u8, u8, u8) {
    /* Nonmatching */
}

/* 8004CEF4-8004CFA4       .text getTypeAgbGcFromTypeAcs__6dMap_cFUcPUcPUc */
void dMap_c::getTypeAgbGcFromTypeAcs(u8, u8*, u8*) {
    /* Nonmatching */
}

/* 8004CFA4-8004D0A4       .text drawPointSingle__6dMap_cFUcfffScsUcUcUc */
void dMap_c::drawPointSingle(u8, f32, f32, f32, s8, s16, u8, u8, u8) {
    /* Nonmatching */
}

/* 8004D0A4-8004D260       .text drawActorPointMiniMap__6dMap_cFP10fopAc_ac_c */
void dMap_c::drawActorPointMiniMap(fopAc_ac_c*) {
    /* Nonmatching */
}

/* 8004D260-8004D364       .text mapBufferSendAGB_commonCursor__6dMap_cFv */
void dMap_c::mapBufferSendAGB_commonCursor() {
    /* Nonmatching */
}

/* 8004D364-8004D4CC       .text mapBufferSendAGB_ocean__6dMap_cFv */
void dMap_c::mapBufferSendAGB_ocean() {
    /* Nonmatching */
}

/* 8004D4CC-8004D4F8       .text mapBufferSendAGB_dungeon__6dMap_cFv */
void dMap_c::mapBufferSendAGB_dungeon() {
    /* Nonmatching */
}

/* 8004D4F8-8004D5F8       .text mapSetPointAll__6dMap_cFv */
void dMap_c::mapSetPointAll() {
    /* Nonmatching */
}

/* 8004D5F8-8004D9BC       .text mapBufferSendAGB__6dMap_cFi */
void dMap_c::mapBufferSendAGB(int) {
    /* Nonmatching */
}

/* 8004D9BC-8004DA54       .text checkFloorMoveImageChangeRoom__19dMap_RoomInfoCtrl_cFUcUcissf */
void dMap_RoomInfoCtrl_c::checkFloorMoveImageChangeRoom(u8, u8, int, s16, s16, f32) {
    /* Nonmatching */
}

/* 8004DA54-8004DBE0       .text init__22dMap_2DMtMapSpcl_tex_cFP7ResTIMGUlRC8_GXColor */
void dMap_2DMtMapSpcl_tex_c::init(ResTIMG*, u32, const _GXColor&) {
    /* Nonmatching */
}

/* 8004DBE0-8004DBF4       .text setScroll__22dMap_2DMtMapSpcl_tex_cFffff */
void dMap_2DMtMapSpcl_tex_c::setScroll(f32, f32, f32, f32) {
    /* Nonmatching */
}

/* 8004DBF4-8004DC08       .text init__18dMap_2DMtMapSpcl_cFiP22dMap_2DMtMapSpcl_tex_c */
void dMap_2DMtMapSpcl_c::init(int, dMap_2DMtMapSpcl_tex_c*) {
    /* Nonmatching */
}

/* 8004DC08-8004DC1C       .text setPos__18dMap_2DMtMapSpcl_cFssss */
void dMap_2DMtMapSpcl_c::setPos(s16, s16, s16, s16) {
    /* Nonmatching */
}

/* 8004DC1C-8004E068       .text draw__18dMap_2DMtMapSpcl_cFv */
void dMap_2DMtMapSpcl_c::draw() {
    /* Nonmatching */
}

/* 8004E068-8004E1CC       .text setImage__18dMap_2DAGBScrDsp_cFP7ResTIMGP8map_dt_c */
void dMap_2DAGBScrDsp_c::setImage(ResTIMG*, map_dt_c*) {
    /* Nonmatching */
}

/* 8004E1CC-8004E264       .text init__18dMap_2DAGBScrDsp_cFP8map_dt_cP7ResTIMGffssssffUc */
void dMap_2DAGBScrDsp_c::init(map_dt_c*, ResTIMG*, f32, f32, s16, s16, s16, s16, f32, f32, u8) {
    /* Nonmatching */
}

/* 8004E264-8004E384       .text getScrnPrm__18dMap_2DAGBScrDsp_cFffifPiPfPf */
void dMap_2DAGBScrDsp_c::getScrnPrm(f32, f32, int, f32, int*, f32*, f32*) {
    /* Nonmatching */
}

/* 8004E384-8004E41C       .text getScrnPrmXY__18dMap_2DAGBScrDsp_cFffffiiffPiPiPfPfPfPf */
void dMap_2DAGBScrDsp_c::getScrnPrmXY(f32, f32, f32, f32, int, int, f32, f32, int*, int*, f32*, f32*, f32*, f32*) {
    /* Nonmatching */
}

/* 8004E41C-8004E698       .text calc_standard_prm__18dMap_2DAGBScrDsp_cFUsUsffssssffPiPiPiPiPfPfPfPfPfPf */
void dMap_2DAGBScrDsp_c::calc_standard_prm(u16, u16, f32, f32, s16, s16, s16, s16, f32, f32, int*, int*, int*, int*, f32*, f32*, f32*, f32*, f32*, f32*) {
    /* Nonmatching */
}

/* 8004E698-8004EE30       .text draw__18dMap_2DAGBScrDsp_cFv */
void dMap_2DAGBScrDsp_c::draw() {
    /* Nonmatching */
}

/* 8004EE30-8004EE44       .text setPos__18dMap_2DAGBScrDsp_cFssss */
void dMap_2DAGBScrDsp_c::setPos(s16, s16, s16, s16) {
    /* Nonmatching */
}

/* 8004EE44-8004EE50       .text setScale__18dMap_2DAGBScrDsp_cFff */
void dMap_2DAGBScrDsp_c::setScale(f32, f32) {
    /* Nonmatching */
}

/* 8004EE50-8004EE88       .text init__12dMap_2DTri_cFssRC8_GXColorffs */
void dMap_2DTri_c::init(s16, s16, const _GXColor&, f32, f32, s16) {
    /* Nonmatching */
}

/* 8004EE88-8004F080       .text draw__12dMap_2DTri_cFv */
void dMap_2DTri_c::draw() {
    /* Nonmatching */
}

/* 8004F080-8004F08C       .text setPos__12dMap_2DTri_cFss */
void dMap_2DTri_c::setPos(s16, s16) {
    /* Nonmatching */
}

/* 8004F08C-8004F0BC       .text init__14dMap_2DPoint_cFssRC8_GXColorUc */
void dMap_2DPoint_c::init(s16, s16, const _GXColor&, u8) {
    /* Nonmatching */
}

/* 8004F0BC-8004F1E4       .text draw__14dMap_2DPoint_cFv */
void dMap_2DPoint_c::draw() {
    /* Nonmatching */
}

/* 8004F1E4-8004F214       .text init__18dMap_2DAGBCursor_cFssRC8_GXColorUc */
void dMap_2DAGBCursor_c::init(s16, s16, const _GXColor&, u8) {
    /* Nonmatching */
}

/* 8004F214-8004F3C0       .text draw__18dMap_2DAGBCursor_cFv */
void dMap_2DAGBCursor_c::draw() {
    /* Nonmatching */
}

/* 8004F3C0-8004F778       .text draw__11dMap_2DT2_cFv */
void dMap_2DT2_c::draw() {
    /* Nonmatching */
}

/* 8004F778-8004F8B4       .text init__11dMap_2DT2_cFP7ResTIMGffffUcUcUcffs */
void dMap_2DT2_c::init(ResTIMG*, f32, f32, f32, f32, u8, u8, u8, f32, f32, s16) {
    /* Nonmatching */
}

/* 8004F8B4-8004FC68       .text setTlut__11dMap_Dmap_cFP16dmap_dmap_tlut_sUcUcUcf */
void dMap_Dmap_c::setTlut(dmap_dmap_tlut_s*, u8, u8, u8, f32) {
    /* Nonmatching */
}

/* 8004FC68-8004FFC8       .text setFloorTextureOne__11dMap_Dmap_cFUc */
void dMap_Dmap_c::setFloorTextureOne(u8) {
    /* Nonmatching */
}

/* 8004FFC8-800504C4       .text init__11dMap_Dmap_cFsssssssssUcUcUcUcUc */
void dMap_Dmap_c::init(s16, s16, s16, s16, s16, s16, s16, s16, s16, u8, u8, u8, u8, u8) {
    /* Nonmatching */
}

/* 800504C4-800517AC       .text draw__11dMap_Dmap_cFv */
void dMap_Dmap_c::draw() {
    /* Nonmatching */
}

/* 800517AC-80051808       .text __dt__11dMap_Dmap_cFv */
dMap_Dmap_c::~dMap_Dmap_c() {
    /* Nonmatching */
}

/* 80051854-800518B0       .text __dt__11dMap_2DT2_cFv */
dMap_2DT2_c::~dMap_2DT2_c() {
    /* Nonmatching */
}

/* 800518B0-8005190C       .text __dt__18dMap_2DAGBCursor_cFv */
dMap_2DAGBCursor_c::~dMap_2DAGBCursor_c() {
    /* Nonmatching */
}

/* 8005190C-80051968       .text __dt__14dMap_2DPoint_cFv */
dMap_2DPoint_c::~dMap_2DPoint_c() {
    /* Nonmatching */
}

/* 80051968-800519C4       .text __dt__12dMap_2DTri_cFv */
dMap_2DTri_c::~dMap_2DTri_c() {
    /* Nonmatching */
}

/* 80051A7C-80051AD8       .text __dt__11dMap_2DSQ_cFv */
dMap_2DSQ_c::~dMap_2DSQ_c() {
    /* Nonmatching */
}

/* 80051DE8-80051E20       .text __arraydtor$4205 */
//void __arraydtor$4205 {}

/* 80051E20-80051E58       .text __arraydtor$4202 */
//void __arraydtor$4202 {}

/* 80051E58-80051E90       .text __arraydtor$4200 */
//void __arraydtor$4200 {}

/* 80051E90-80051EAC       .text __ct__11dMap_2DT2_cFv */
dMap_2DT2_c::dMap_2DT2_c() {
    /* Nonmatching */
}

/* 80051EAC-80051EE4       .text __arraydtor$4196 */
//void __arraydtor$4196 {}

/* 80051EE4-80051F00       .text __ct__14dMap_2DPoint_cFv */
dMap_2DPoint_c::dMap_2DPoint_c() {
    /* Nonmatching */
}

/* 80051F00-80051F38       .text __arraydtor$4188 */
//void __arraydtor$4188 {}

/* 80051F38-80051F74       .text __dt__22dMap_2DMtMapSpcl_tex_cFv */
dMap_2DMtMapSpcl_tex_c::~dMap_2DMtMapSpcl_tex_c() {
    /* Nonmatching */
}

/* 80051F74-80051F78       .text __ct__22dMap_2DMtMapSpcl_tex_cFv */
dMap_2DMtMapSpcl_tex_c::dMap_2DMtMapSpcl_tex_c() {
    /* Nonmatching */
}

/* 80051F78-80051FB0       .text __arraydtor$4186 */
//void __arraydtor$4186 {}

/* 80051FCC-80052004       .text __arraydtor$4184 */
//void __arraydtor$4184 {}

/* 80052004-800520B8       .text __dt__15dMap_RoomInfo_cFv */
dMap_RoomInfo_c::~dMap_RoomInfo_c() {
    /* Nonmatching */
}

/* 800520B8-80052134       .text __ct__15dMap_RoomInfo_cFv */
dMap_RoomInfo_c::dMap_RoomInfo_c() {
    /* Nonmatching */
}
