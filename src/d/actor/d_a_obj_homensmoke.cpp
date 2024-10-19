//
// Generated by dtk
// Translation Unit: d_a_obj_homensmoke.cpp
//

#include "d/actor/d_a_obj_homensmoke.h"
#include "d/d_procname.h"
#include "d/d_particle.h"
#include "m_Do/m_Do_mtx.h"
#include "d/d_com_inf_game.h"

#include "weak_bss_936_to_1036.h" // IWYU pragma: keep

namespace daObjHomensmoke {
    /* 000000EC-00000230       .text set_mtx__Q215daObjHomensmoke5Act_cFv */
    void Act_c::set_mtx() {
        if (param_get_axis() != 0) {
            cXyz backOffset(0.0f, 0.0f, -200.0f);
            cXyz forwardOffset(0.0f, 0.0f, 200.0f);
            mDoMtx_stack_c::transS(current.pos);
            mDoMtx_stack_c::transM(forwardOffset);
            cMtx_ZrotM(mDoMtx_stack_c::get(), shape_angle.z);
            cMtx_YrotM(mDoMtx_stack_c::get(), shape_angle.y);
            cMtx_XrotM(mDoMtx_stack_c::get(), shape_angle.x);
            mDoMtx_stack_c::transM(backOffset);
            mDoMtx_copy(mDoMtx_stack_c::get(), mMtx);
        } else {
            mDoMtx_stack_c::transS(current.pos);
            cMtx_ZrotM(mDoMtx_stack_c::get(), shape_angle.z);
            cMtx_YrotM(mDoMtx_stack_c::get(), shape_angle.y);
            cMtx_XrotM(mDoMtx_stack_c::get(), shape_angle.x);
            mDoMtx_copy(mDoMtx_stack_c::get(), mMtx);
        }
    }

    /* 0000026C-0000048C       .text _create__Q215daObjHomensmoke5Act_cFv */
    s32 Act_c::_create() {
        fopAcM_SetupActor(this, Act_c);
        
        mbInitialized = FALSE;
        
        set_mtx();
        fopAcM_SetMtx(this, mMtx);
        
        mType = param_get_arg0();
        
        struct daObjHomensmoke__cullbox {
            /* 0x0 */ Vec mMin;
            /* 0xC */ Vec mMax;
        };
        static daObjHomensmoke__cullbox culling_dat[2] = {
            {-300.0f, -30.0f, -100.0f, 300.0f, 600.0f, 300.0f},
            {-100.0f, -30.0f, -100.0f, 100.0f, 250.0f, 120.0f},
        };
        fopAcM_setCullSizeBox(this,
            culling_dat[mType].mMin.x, culling_dat[mType].mMin.y, culling_dat[mType].mMin.z,
            culling_dat[mType].mMax.x, culling_dat[mType].mMax.y, culling_dat[mType].mMax.z
        );
        
        fopAc_ac_c* parent = fopAcM_SearchByID(parentActorID);
        if (parent) {
            tevStr = parent->tevStr;
        }
        
        return cPhs_COMPLEATE_e;
    }

    /* 0000048C-000004C8       .text _delete__Q215daObjHomensmoke5Act_cFv */
    bool Act_c::_delete() {
        if (mSmokeCb.getEmitter()) {
            mSmokeCb.end();
        }
        return true;
    }

    /* 000004C8-00000738       .text _execute__Q215daObjHomensmoke5Act_cFv */
    bool Act_c::_execute() {
        if (!mbInitialized) {
            static cXyz norse_offsetL(0.0f, 300.0f, 20.0f);
            static cXyz norse_offsetS(0.0f, 70.0f, 20.0f);
            if (mType == 0) {
                cMtx_multVec(mMtx, &norse_offsetL, &mSmokePos);
            } else {
                cMtx_multVec(mMtx, &norse_offsetS, &mSmokePos);
            }
            
            JPABaseEmitter* smokeEmitter = dComIfGp_particle_setToon(0x2027, &mSmokePos, NULL, NULL, 0xFF, &mSmokeCb, fopAcM_GetRoomNo(this));
            if (smokeEmitter) {
                static f32 rate_table[2] = {1.0f, 0.5f};
                f32 rate = rate_table[mType & 1];
                smokeEmitter->setGlobalAlpha(0xB4);
                smokeEmitter->setRate(50.0f);
                smokeEmitter->setMaxFrame(1);
                JGeometry::TVec3<f32> scale;
                scale.set(rate, 0.0f, rate);
                smokeEmitter->setEmitterScale(scale);
                scale.set(rate*5.0f, rate*5.0f, rate*5.0f);
                smokeEmitter->setGlobalDynamicsScale(scale);
                scale.set(rate*6.0f, rate*6.0f, rate*6.0f);
                smokeEmitter->setGlobalParticleScale(scale);
            }
            
            JPABaseEmitter* rubbleEmitter = dComIfGp_particle_setToon(0x81B1, &current.pos);
            if (rubbleEmitter) {
                rubbleEmitter->setGlobalPrmColor(tevStr.mColorK0.r, tevStr.mColorK0.g, tevStr.mColorK0.b);
                if (mType == 1) {
                    JGeometry::TVec3<f32> scale(0.6f, 0.6f, 0.6f);
                    rubbleEmitter->setEmitterScale(scale);
                    rubbleEmitter->setGlobalDynamicsScale(scale);
                    rubbleEmitter->setGlobalParticleScale(scale);
                }
                rubbleEmitter->setGlobalRTMatrix(mMtx);
            }
            
            mbInitialized = TRUE;
        } else if (mSmokeCb.isEnd()) {
            fopAcM_delete(this);
        }
        return true;
    }

    /* 00000738-00000740       .text _draw__Q215daObjHomensmoke5Act_cFv */
    bool Act_c::_draw() {
        return true;
    }

    namespace {
        /* 00000740-00000760       .text Mthd_Create__Q215daObjHomensmoke32@unnamed@d_a_obj_homensmoke_cpp@FPv */
        s32 Mthd_Create(void* i_this) {
            return ((Act_c*)i_this)->_create();
        }

        /* 00000760-00000784       .text Mthd_Delete__Q215daObjHomensmoke32@unnamed@d_a_obj_homensmoke_cpp@FPv */
        BOOL Mthd_Delete(void* i_this) {
            return ((Act_c*)i_this)->_delete();
        }

        /* 00000784-000007A8       .text Mthd_Execute__Q215daObjHomensmoke32@unnamed@d_a_obj_homensmoke_cpp@FPv */
        BOOL Mthd_Execute(void* i_this) {
            return ((Act_c*)i_this)->_execute();
        }

        /* 000007A8-000007CC       .text Mthd_Draw__Q215daObjHomensmoke32@unnamed@d_a_obj_homensmoke_cpp@FPv */
        BOOL Mthd_Draw(void* i_this) {
            return ((Act_c*)i_this)->_draw();
        }

        /* 000007CC-000007D4       .text Mthd_IsDelete__Q215daObjHomensmoke32@unnamed@d_a_obj_homensmoke_cpp@FPv */
        BOOL Mthd_IsDelete(void* i_this) {
            return TRUE;
        }
        
        static actor_method_class Mthd_Table = {
            (process_method_func)Mthd_Create,
            (process_method_func)Mthd_Delete,
            (process_method_func)Mthd_Execute,
            (process_method_func)Mthd_IsDelete,
            (process_method_func)Mthd_Draw,
        };
    };
};

actor_process_profile_definition g_profile_Obj_Homensmk = {
    /* LayerID      */ fpcLy_CURRENT_e,
    /* ListID       */ 0x0003,
    /* ListPrio     */ fpcPi_CURRENT_e,
    /* ProcName     */ PROC_Obj_Homensmk,
    /* Proc SubMtd  */ &g_fpcLf_Method.base,
    /* Size         */ sizeof(daObjHomensmoke::Act_c),
    /* SizeOther    */ 0,
    /* Parameters   */ 0,
    /* Leaf SubMtd  */ &g_fopAc_Method.base,
    /* Priority     */ 0x0043,
    /* Actor SubMtd */ &daObjHomensmoke::Mthd_Table,
    /* Status       */ fopAcStts_NOCULLEXEC_e | fopAcStts_CULL_e | fopAcStts_UNK4000_e | fopAcStts_UNK40000_e,
    /* Group        */ fopAc_ACTOR_e,
    /* CullType     */ fopAc_CULLBOX_CUSTOM_e,
};
