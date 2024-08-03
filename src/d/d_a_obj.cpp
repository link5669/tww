//
// Generated by dtk
// Translation Unit: d_a_obj.cpp
//

#include "d/d_a_obj.h"
#include "d/d_bg_s_gnd_chk.h"
#include "d/d_com_inf_game.h"
#include "d/d_procname.h"
#include "d/d_kankyo_wether.h"
#include "d/d_path.h"
#include "m_Do/m_Do_mtx.h"
#include "JSystem/JParticle/JPAEmitter.h"

namespace daObj {
    /* 800666EC-800668BC       .text make_land_effect__5daObjFP10fopAc_ac_cP11dBgS_GndChkf */
    void make_land_effect(fopAc_ac_c* ac, dBgS_GndChk* chk, f32 scale) {
        cXyz scaleV;
        switch (dComIfG_Bgsp()->GetAttributeCode(*chk)) {
        case dBgS_Attr_WATER_e:
            {
                scaleV.setall(scale * 0.85f);
                JPABaseEmitter* pEmtr = dComIfGp_particle_set(0x23, &ac->current.pos, NULL, &scaleV);
                if (pEmtr != NULL) {
                    pEmtr->setRate(20.0f);
                    pEmtr->setMaxFrame(1);
                    pEmtr->setSpread(1.0f);
                    pEmtr->setDirectionalSpeed(12.0f);
                }
            }
            break;
        case dBgS_Attr_GRASS_e:
            {
                scaleV.setall(scale);
                JPABaseEmitter* pEmtr = dComIfGp_particle_set(0x24, &ac->current.pos, NULL, &scaleV);
                if (pEmtr != NULL) {
                    pEmtr->setLifeTime(20);
                    pEmtr->setRate(50.0f);
                    pEmtr->setMaxFrame(1);
                    pEmtr->setSpread(1.0f);
                    pEmtr->setDirectionalSpeed(20.0f);
                    JGeometry::TVec3<f32> trans(0.0f, 5.0f, 0.0f);
                    pEmtr->setEmitterTranslation(trans);
                }
            }
            break;
        case dBgS_Attr_GIANT_FLOWER_e:
        case dBgS_Attr_CARPET_e:
        case dBgS_Attr_ICE_e:
        case dBgS_Attr_UNK1B_e:
            break;
        default:
            {
                cXyz scaleV;
                scaleV.setall(scale);
                scaleV *= (5.0f / 3.0f);
                fopAcM_create(PROC_Obj_Eff, 3, &ac->current.pos, -1, NULL, &scaleV);
            }
            break;
        }
    }

    /* 800668BC-800669E8       .text get_wind_spd__5daObjFP10fopAc_ac_cf */
    cXyz& get_wind_spd(fopAc_ac_c* param_1, f32 param_2) {
        static cXyz total_spd(cXyz::Zero);

        cXyz wind = *dKyw_get_wind_vec() * dKyw_get_wind_pow();

        cXyz pntVec;
        f32 pntPow;
        dKyw_pntwind_get_info(&param_1->current.pos, &pntVec, &pntPow);
        pntVec *= pntPow;

        total_spd = (wind + pntVec) * (param_2 * 0.5f);
        return total_spd;
    }

    /* 800669E8-80066B0C       .text get_path_spd__5daObjFR13cBgS_PolyInfof */
    cXyz& get_path_spd(cBgS_PolyInfo& param_1, f32 param_2) {
        static cXyz path_spd(cXyz::Zero);

        int temp;
        if(dPath_GetPolyRoomPathVec(param_1, &path_spd, &temp)) {
            if(path_spd.normalizeRS()) {
                path_spd *= param_2 * temp * (1.0f / 255.0f);
            }
            else {
                path_spd = cXyz::Zero;
            }
        }

        return path_spd;
    }

    /* 80066B0C-80066B3C       .text posMoveF_stream__5daObjFP10fopAc_ac_cPC4cXyzPC4cXyzff */
    void posMoveF_stream(fopAc_ac_c* param_1, const cXyz* param_2, const cXyz* param_3, f32 param_4, f32 param_5) {
        posMoveF_grade(param_1, param_2, param_3, param_4, param_5, 0, 0.0f, 0.0f, 0);
    }

    namespace {
        /* 80066B3C-80066C38       .text posMoveF_resist_acc__Q25daObj21@unnamed@d_a_obj_cpp@FP4cXyzPC10fopAc_ac_cPC4cXyzff */
        void posMoveF_resist_acc(cXyz* pDst, const fopAc_ac_c* pActor, const cXyz* pStreamSpd, f32 param_4, f32 param_5) {
            cXyz delta = pActor->speed - *pStreamSpd;

            f32 dx = delta.x;
            f32 dy = delta.y;
            f32 dz = delta.z;

            cXyz result(dx * param_4, dy * param_4, dz * param_4);
            result.x += fabsf(dx) * dx * param_5;
            result.y += fabsf(dy) * dy * param_5;
            result.z += fabsf(dz) * dz * param_5;

            result *= -1.0f;
            *pDst = result;
        }

        /* 80066C38-80066D6C       .text posMoveF_grade_acc__Q25daObj21@unnamed@d_a_obj_cpp@FP4cXyzPC10fopAc_ac_cPC4cXyzffPC4cXyzPC4cXyz */
        void posMoveF_grade_acc(cXyz* pDst, const fopAc_ac_c* pActor, const cXyz* pNorm, f32 friction, f32 noGradeCos, const cXyz* pAccel0, const cXyz* pAccel1) {
            *pDst = cXyz::Zero;

            if(pNorm) {
                cXyz accel = *pAccel0;
                accel.y = pAccel0->y + pActor->gravity;
                if(pAccel1) {
                    accel += *pAccel1;
                }

                if(accel.getDotProduct(*pNorm) < 0.0f) {
                    if(pNorm->y <= noGradeCos) {
                        cXyz temp;
                        cM3d_CrawVec(*pNorm, accel, &temp);
                        *pDst += temp;
                    }

                    cXyz temp;
                    cM3d_CrawVec(*pNorm, pActor->speed, &temp);
                    *pDst -= temp * friction;
                }
            }
        }
    }

    /* 80066D6C-8006700C       .text posMoveF_grade__5daObjFP10fopAc_ac_cPC4cXyzPC4cXyzffPC4cXyzffPC4cXyz */
    void posMoveF_grade(fopAc_ac_c* pActor, const cXyz* pAddVel, const cXyz* stream_spd, f32 param_4, f32 param_5, const cXyz* pNorm, f32 friction, f32 no_grade_cos, const cXyz* pAddAccel) {
        /* Nonmatching */
        JUT_ASSERT(0x118, stream_spd != NULL)

        cXyz resist_accel;
        posMoveF_resist_acc(&resist_accel, pActor, stream_spd, param_4, param_5);

        JUT_ASSERT(0x121, (friction >= 0.0f) && (friction < 1.0f) && (no_grade_cos >= 0.0f) && (no_grade_cos <= 1.0f))

        cXyz vel;
        posMoveF_grade_acc(&vel, pActor, pNorm, param_5, param_4, &resist_accel, pAddAccel);

        f32 spf = pActor->speedF;
        f32 grav = pActor->gravity;

        f32 x = spf * cM_ssin(pActor->current.angle.y) + resist_accel.x + vel.x;
        f32 y = pActor->speed.y + grav + resist_accel.y + vel.y;
        f32 z = spf * cM_scos(pActor->current.angle.y) + resist_accel.z + vel.z;
        if(pAddAccel) {
            x += pAddAccel->x;
            y += pAddAccel->y;
            z += pAddAccel->z;
        }

        if(y < pActor->maxFallSpeed) {
            y = pActor->maxFallSpeed;
        }

        pActor->speed.set(x, y, z);
        pActor->speedF = sqrtf(x * x + z * z);
        pActor->current.angle.y = cM_atan2s(spf, grav);
        fopAcM_posMove(pActor, pAddVel);
    }

    /* 8006700C-800671D4       .text quat_rotBaseY__5daObjFP10QuaternionRC4cXyz */
    void quat_rotBaseY(Quaternion*, const cXyz&) {
        /* Nonmatching */
    }

    /* 800671D4-8006737C       .text quat_rotBaseY2__5daObjFP10QuaternionRC4cXyz */
    void quat_rotBaseY2(Quaternion*, const cXyz&) {
        /* Nonmatching */
    }

    /* 8006737C-80067524       .text quat_rotBaseZ__5daObjFP10QuaternionRC4cXyz */
    void quat_rotBaseZ(Quaternion*, const cXyz&) {
        /* Nonmatching */
    }

    /* 80067524-800676EC       .text quat_rotVec__5daObjFP10QuaternionRC4cXyzRC4cXyz */
    void quat_rotVec(Quaternion*, const cXyz&, const cXyz&) {
        /* Nonmatching */
    }

    /* 800676EC-80067734       .text SetCurrentRoomNo__5daObjFP10fopAc_ac_cP11dBgS_GndChk */
    void SetCurrentRoomNo(fopAc_ac_c* ac, dBgS_GndChk* chk) {
        s32 roomId = dComIfG_Bgsp()->GetRoomId(*chk);
        if (roomId >= 0)
            fopAcM_SetRoomNo(ac, roomId);
    }

    /* 80067734-800678A8       .text HitSeStart__5daObjFPC4cXyziPC12dCcD_GObjInfUl */
    void HitSeStart(const cXyz*, int, const dCcD_GObjInf*, unsigned long) {
        /* Nonmatching */
    }

    /* 800678A8-800679FC       .text HitEff_sub_kikuzu__5daObjFPC4cXyzPC4cXyzPC12dKy_tevstr_c */
    void HitEff_sub_kikuzu(const cXyz*, const cXyz*, const dKy_tevstr_c*) {
        /* Nonmatching */
    }

    /* 800679FC-80067AE0       .text HitEff_kikuzu__5daObjFPC10fopAc_ac_cPC8dCcD_Cyl */
    void HitEff_kikuzu(const fopAc_ac_c*, const dCcD_Cyl*) {
        /* Nonmatching */
    }

    /* 80067AF0-80067C40       .text HitEff_hibana__5daObjFPC4cXyzPC4cXyz */
    void HitEff_hibana(const cXyz*, const cXyz*) {
        /* Nonmatching */
    }

    /* 80067C40-80067D1C       .text HitEff_hibana__5daObjFPC10fopAc_ac_cPC8dCcD_Cyl */
    void HitEff_hibana(const fopAc_ac_c*, const dCcD_Cyl*) {
        /* Nonmatching */
    }
}
