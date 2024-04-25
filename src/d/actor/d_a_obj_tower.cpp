//
// Generated by dtk
// Translation Unit: d_a_obj_tower.cpp
//

#include "d/actor/d_a_obj_tower.h"
#include "d/res/res_x_tower.h"
#include "d/d_com_inf_game.h"
#include "d/d_procname.h"
#include "dolphin/mtx/mtx.h"
#include "f_op/f_op_actor_mng.h"
#include "m_Do/m_Do_printf.h"

/* 00000078-00000098       .text CheckCreateHeap__FP10fopAc_ac_c */
static BOOL CheckCreateHeap(fopAc_ac_c* i_this) {
    return ((daObjTower_c*)i_this)->CreateHeap();
}

/* 00000098-0000020C       .text CreateHeap__12daObjTower_cFv */
BOOL daObjTower_c::CreateHeap() {
    J3DModelData* modelData = (J3DModelData*)dComIfG_getObjectRes("X_tower", X_TOWER_BDL_X_TOWER);

    JUT_ASSERT(0x56, modelData != 0);

    mpModel = mDoExt_J3DModel__create(modelData, 0, 0x11020203);

    if (mpModel == NULL)
        return FALSE;

    mDoMtx_stack_c::transS(current.pos);
    mDoMtx_stack_c::YrotM(shape_angle.y);
    mDoMtx_stack_c::scaleM(scale.x, scale.y, scale.z);
    mDoMtx_copy(mDoMtx_stack_c::get(), mMtx);

    mpBgW = new dBgW();

    if (mpBgW == NULL)
        return FALSE;

    cBgD_t* pData = (cBgD_t*)dComIfG_getObjectRes("X_tower", X_TOWER_DZB_X_TOWER);

    return mpBgW->Set(pData, cBgW::MOVE_BG_e, &mMtx) ? FALSE : TRUE;
}

/* 0000020C-0000028C       .text CreateInit__12daObjTower_cFv */
void daObjTower_c::CreateInit() {
    fopAcM_SetMtx(this, mpModel->getBaseTRMtx());
    fopAcM_setCullSizeBox(this, -30000.0f, -5000.0f, -30000.0f, 30000.0f, 40000.0f, 30000.0f);
    dComIfG_Bgsp()->Regist(mpBgW, this);
    field_0x2d0 = 1;
    set_mtx();
}

/* 0000028C-0000030C       .text set_mtx__12daObjTower_cFv */
void daObjTower_c::set_mtx() {
    mpModel->setBaseScale(scale);
    mDoMtx_stack_c::transS(current.pos);
    mDoMtx_stack_c::YrotM(current.angle.y);
    mpModel->setBaseTRMtx(mDoMtx_stack_c::get());
}

cPhs__Step daObjTower_c::_create() {
    cPhs__Step PVar3;

    fopAcM_SetupActor(this, daObjTower_c);

    field_0x2d0 = 0;

    if (!dComIfGs_isEventBit(0x1e40)) {
        PVar3 = cPhs_UNK3_e;
    } else {
        PVar3 = (cPhs__Step)dComIfG_resLoad(&mPhs, "X_tower");
        if (PVar3 == cPhs_COMPLEATE_e) {
            if (!fopAcM_entrySolidHeap(this, CheckCreateHeap, 0x1c6c0)) {
                PVar3 = cPhs_ERROR_e;
            } else {
                CreateInit();
            }
        }
    }

    return PVar3;
}
/* 0000030C-000003DC       .text daObjTower_Create__FPv */
static cPhs__Step daObjTower_Create(void* i_this) {
    return ((daObjTower_c*)i_this)->_create();
}

BOOL daObjTower_c::_delete() {
    if (field_0x2d0 == 1) {
        dComIfG_Bgsp()->Release(mpBgW);
    }

    if (fpcM_CreateResult(this) != cPhs_UNK3_e) {
        dComIfG_resDelete(&(mPhs), "X_tower");
    }

    return TRUE;
}

/* 000003DC-00000444       .text daObjTower_Delete__FPv */
static BOOL daObjTower_Delete(void* i_this) {
    return ((daObjTower_c*)i_this)->_delete();
}

BOOL daObjTower_c::_draw() {
    g_env_light.settingTevStruct(TEV_TYPE_BG0, &current.pos, &tevStr);
    g_env_light.setLightTevColorType(mpModel, &tevStr);

    dComIfGd_setListBG();
    mDoExt_modelUpdateDL(mpModel);
    dComIfGd_setList();

    return TRUE;
}

/* 00000444-000004E8       .text daObjTower_Draw__FPv */
static BOOL daObjTower_Draw(void* i_this) {
    return ((daObjTower_c*)i_this)->_draw();
}

BOOL daObjTower_c::_execute() {
    if (demoActorID != 0) {
        dDemo_actor_c* pdVar1 = dComIfGp_demo_getActor(demoActorID);
        if (pdVar1 != NULL && pdVar1->checkEnable(0x02)) {
            current.pos = pdVar1->mTranslation;
        }
    }

    set_mtx();
    return TRUE;
}

/* 000004E8-00000568       .text daObjTower_Execute__FPv */
static BOOL daObjTower_Execute(void* i_this) {
    return ((daObjTower_c*)i_this)->_execute();
}

/* 00000568-00000570       .text daObjTower_IsDelete__FPv */
static BOOL daObjTower_IsDelete(void*) {
    return TRUE;
}

static actor_method_class daObj_TowerMethodTable = {
    (process_method_func)daObjTower_Create,
    (process_method_func)daObjTower_Delete,
    (process_method_func)daObjTower_Execute,
    (process_method_func)daObjTower_IsDelete,
    (process_method_func)daObjTower_Draw,
};

actor_process_profile_definition g_profile_Obj_Tower = {
    /* LayerID      */ fpcLy_CURRENT_e,
    /* ListID       */ 0x0007,
    /* ListPrio     */ fpcPi_CURRENT_e,
    /* ProcName     */ PROC_Obj_Tower,
    /* Proc SubMtd  */ &g_fpcLf_Method.base,
    /* Size         */ sizeof(daObjTower_c),
    /* SizeOther    */ 0,
    /* Parameters   */ 0,
    /* Leaf SubMtd  */ &g_fopAc_Method.base,
    /* Priority     */ 0x01C4,
    /* Actor SubMtd */ &daObj_TowerMethodTable,
    /* Status       */ fopAcStts_UNK40000_e,
    /* Group        */ fopAc_ACTOR_e,
    /* CullType     */ fopAc_CULLBOX_CUSTOM_e,
};
