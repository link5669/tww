//
// Generated by dtk
// Translation Unit: JPAField.cpp
//

#include "JSystem/JParticle/JPAField.h"
#include "JSystem/JParticle/JPAEmitter.h"
#include "JSystem/JParticle/JPAParticle.h"
#include "JSystem/JParticle/JPAFieldBlock.h"
#include "JSystem/JUtility/JUTAssert.h"
#include "JSystem/JMath/JMATrigonometric.h"
#include "dolphin/mtx/mtxvec.h"

JPAFieldContainer fc;
JPAEmitterInfo * JPAFieldData::pEmtrInfo;

/* 80259EE8-8025A0D8       .text loadFieldData__12JPABaseFieldFP12JPAFieldDataP13JPAFieldBlock */
void JPABaseField::loadFieldData(JPAFieldData* data, JPAFieldBlock* block) {
    data->mType = block->getType();
    data->mID = block->getID();
    data->mVelType = block->getVelType();
    data->mCycle = block->getCycle();
    data->mSttFlag = block->getSttFlag();
    data->mMag = block->getMag();
    data->mMagRndm = block->getMagRndm();
    data->mMaxDist = block->getMaxDist();
    block->getPos(data->mPos);
    block->getDir(data->mDir);
    data->mVal1 = block->getVal1();
    data->mVal2 = block->getVal2();
    data->mVal3 = block->getVal3();
    data->mFadeIn = block->getFadeIn();
    data->mFadeOut = block->getFadeOut();
    data->mEnTime = block->getEnTime();
    data->mDisTime = block->getDisTime();
    if (!(data->mSttFlag & 0x10))
        data->mDisTime = 1.0f;
    data->mVel.zero();
}

/* 8025A0D8-8025A21C       .text calcVel__12JPABaseFieldFP12JPAFieldDataP15JPABaseParticle */
void JPABaseField::calcVel(JPAFieldData* data, JPABaseParticle* ptcl) {
    JGeometry::TVec3<f32> vel = data->mVel;

    if (!(ptcl->mStatus & 0x04)) {
        f32 fadeAffect = calcFadeAffect(data, ptcl->mCurNormTime);
        vel.scale(fadeAffect);
    }

    switch (data->mVelType) {
    case 0: ptcl->mFieldAccel.add(vel); break;
    case 1: ptcl->mBaseVel += vel; break;
    case 2: ptcl->mFieldVel += vel; break;
    }
}

/* 8025A21C-8025A2B0       .text calcFadeAffect__12JPABaseFieldFP12JPAFieldDataf */
f32 JPABaseField::calcFadeAffect(JPAFieldData* data, f32 time) {
    f32 affect = 1.0f;
    if (((data->mSttFlag & 0x08) && time < data->mEnTime) || ((data->mSttFlag & 0x10) && time >= data->mDisTime)) {
        affect = 0.0f;
    } else {
        if ((data->mSttFlag & 0x40) && time >= data->mFadeOut)
            affect = data->mFadeOutRate * (data->mDisTime - time);
        else if ((data->mSttFlag & 0x20) && time < data->mFadeIn)
            affect = data->mFadeInRate * (time - data->mEnTime);
    }
    return affect;
}

/* 8025A2B0-8025A330       .text preCalc__12JPABaseFieldFP12JPAFieldData */
void JPABaseField::preCalc(JPAFieldData* data) {
    data->mMaxDistSq = data->mMaxDist * data->mMaxDist;

    data->mFadeOutRate = data->mDisTime - data->mFadeOut;
    if (data->mFadeOutRate == 0.0f) {
        data->mFadeOutRate = 1.0f;
    } else {
        data->mFadeOutRate = 1.0f / data->mFadeOutRate;
    }
    data->mFadeInRate = data->mFadeIn - data->mEnTime;
    if (data->mFadeInRate == 0.0f) {
        data->mFadeInRate = 1.0f;
    } else {
        data->mFadeInRate = 1.0f / data->mFadeInRate;
    }
}

/* 8025A330-8025A344       .text isItinRange__12JPABaseFieldFP12JPAFieldDataf */
bool JPABaseField::isItinRange(JPAFieldData* data, f32 v) {
    return v < data->mMaxDistSq;
}

/* 8025A344-8025A3E4       .text preCalc__15JPAGravityFieldFP12JPAFieldData */
void JPAGravityField::preCalc(JPAFieldData* data) {
    JPABaseField::preCalc(data);
    if (data->mSttFlag & 0x02) {
        data->mVel.scale(data->mMag, data->mDir);
    } else {
        JGeometry::TVec3<f32> rotDir;
        MTXMultVec(JPAFieldData::pEmtrInfo->mGlobalRot, data->mDir, rotDir);
        data->mVel.scale(data->mMag, rotDir);
    }
}

/* 8025A3E4-8025A404       .text calc__15JPAGravityFieldFP12JPAFieldDataP15JPABaseParticle */
void JPAGravityField::calc(JPAFieldData* data, JPABaseParticle* ptcl) {
    calcVel(data, ptcl);
}

/* 8025A404-8025A510       .text preCalc__11JPAAirFieldFP12JPAFieldData */
void JPAAirField::preCalc(JPAFieldData* data) {
    JPABaseField::preCalc(data);
    if (data->mSttFlag & 0x02) {
        data->mWork[2].set(data->mDir);
    } else {
        MTXMultVec(JPAFieldData::pEmtrInfo->mGlobalRot, data->mDir, data->mWork[2]);
    }

    data->mVel.scale(data->mMag, data->mWork[2]);
    if (data->mSttFlag & 0x01) {
        data->mWork[1].x = JMASCos(data->mVal1 * 0xFFFF);
        if (data->mSttFlag & 0x02) {
            data->mWork[0].set(data->mPos);
        } else {
            MTXMultVec(JPAFieldData::pEmtrInfo->mGlobalRot, data->mWork[0], data->mPos);
        }
    }
}

/* 8025A510-8025A6EC       .text calc__11JPAAirFieldFP12JPAFieldDataP15JPABaseParticle */
void JPAAirField::calc(JPAFieldData* data, JPABaseParticle* ptcl) {
    /* Nonmatching */
    if (data->mSttFlag & 0x01) {
        JGeometry::TVec3<f32> vel;
        if (data->mSttFlag & 0x02) {
            vel.sub(ptcl->mPosition, data->mWork[0]);
        } else {
            vel.sub(ptcl->mLocalPosition, data->mWork[0]);
        }
        vel.normalize();

        if (data->mWork[2].dot(vel) >= data->mWork[1].x) {
            JPABaseField::calcVel(data, ptcl);
        }
    } else {
        JPABaseField::calcVel(data, ptcl);
    }

    if (data->mSttFlag & 0x04) {
        f32 len = ptcl->mBaseVel.length();
        if (len > data->mMagRndm) {
            ptcl->mBaseVel.scale(data->mMagRndm / len);
        }
    }
}

/* 8025A6EC-8025A788       .text preCalc__14JPAMagnetFieldFP12JPAFieldData */
void JPAMagnetField::preCalc(JPAFieldData* data) {
    JPABaseField::preCalc(data);
    if (data->mSttFlag & 0x02) {
        data->mWork[0].set(data->mPos);
    } else {
        data->mWork[0].sub(data->mPos, JPAFieldData::pEmtrInfo->mEmitterTranslation);
        MTXMultVec(JPAFieldData::pEmtrInfo->mGlobalRot, data->mWork[0], data->mWork[0]);
    }
}

/* 8025A788-8025A8AC       .text calc__14JPAMagnetFieldFP12JPAFieldDataP15JPABaseParticle */
void JPAMagnetField::calc(JPAFieldData* data, JPABaseParticle* ptcl) {
    if (data->mSttFlag & 0x02) {
        data->mVel.sub(data->mWork[0], ptcl->mPosition);
    } else {
        data->mVel.sub(data->mWork[0], ptcl->mLocalPosition);
    }
    data->mVel.setLength(data->mMag);
    calcVel(data, ptcl);
}

/* 8025A8AC-8025A954       .text preCalc__14JPANewtonFieldFP12JPAFieldData */
void JPANewtonField::preCalc(JPAFieldData* data) {
    JPABaseField::preCalc(data);
    if (data->mSttFlag & 0x02) {
        data->mWork[0].set(data->mPos);
    } else {
        data->mWork[0].sub(data->mPos, JPAFieldData::pEmtrInfo->mEmitterTranslation);
        MTXMultVec(JPAFieldData::pEmtrInfo->mGlobalRot, data->mWork[0], data->mWork[0]);
    }
    data->mVal2 = data->mVal1 * data->mVal1;
}

/* 8025A954-8025ABB8       .text calc__14JPANewtonFieldFP12JPAFieldDataP15JPABaseParticle */
void JPANewtonField::calc(JPAFieldData* data, JPABaseParticle* ptcl) {
    /* Nonmatching */
}

/* 8025ABB8-8025ACA0       .text preCalc__14JPAVortexFieldFP12JPAFieldData */
void JPAVortexField::preCalc(JPAFieldData* data) {
    JPABaseField::preCalc(data);
    MTXMultVec(JPAFieldData::pEmtrInfo->mEmitterGlobalRot, &data->mDir, &data->mWork[0]);
    data->mWork[0].normalize();
    data->mVal1 = data->mPos.z * data->mPos.z;
    data->mVal2 = 1.0f / data->mVal1;
}

/* 8025ACA0-8025AE04       .text calc__14JPAVortexFieldFP12JPAFieldDataP15JPABaseParticle */
void JPAVortexField::calc(JPAFieldData* data, JPABaseParticle* ptcl) {
    /* Nonmatching */

    JGeometry::TVec3<f32> force;
    f32 dot = data->mWork[0].dot(ptcl->mLocalPosition);
    force.scale(dot, data->mWork[0]);
    force.sub(ptcl->mLocalPosition, force);
    f32 sqDist = force.squared();

    f32 power = sqDist;
    if (sqDist > data->mVal1)
        power = data->mVal1;
    power *= data->mVal2;

    f32 speed = (1.0f - power) * data->mMag + power * data->mMagRndm;
    force.normalize();

    data->mVel.cross(force, data->mWork[0]);
    data->mVel.scale(speed);
    JPABaseField::calcVel(data, ptcl);
}

/* 8025AE04-8025B114       .text preCalc__18JPAConvectionFieldFP12JPAFieldData */
void JPAConvectionField::preCalc(JPAFieldData* data) {
    /* Nonmatching */
}

/* 8025B114-8025B3CC       .text calc__18JPAConvectionFieldFP12JPAFieldDataP15JPABaseParticle */
void JPAConvectionField::calc(JPAFieldData* data, JPABaseParticle* ptcl) {
    /* Nonmatching */
}

/* 8025B3CC-8025B50C       .text calc__14JPARandomFieldFP12JPAFieldDataP15JPABaseParticle */
void JPARandomField::calc(JPAFieldData* data, JPABaseParticle* ptcl) {
    /* Nonmatching */
    s32 frame = ptcl->mCurFrame;
    if (frame != 0) {
        if (data->mCycle == 0)
            return;

        if ((frame % data->mCycle) != 0)
            return;
    }

    data->mVel.set(
        JPAFieldData::pEmtrInfo->mpCurEmitter->getRandomSF(),
        JPAFieldData::pEmtrInfo->mpCurEmitter->getRandomSF(),
        JPAFieldData::pEmtrInfo->mpCurEmitter->getRandomSF()
    );
    data->mVel.scale(data->mMag);
    JPABaseField::calcVel(data, ptcl);
}

/* 8025B50C-8025B584       .text init__12JPADragFieldFP12JPAFieldDataP15JPABaseParticle */
void JPADragField::init(JPAFieldData* data, JPABaseParticle* ptcl) {
    ptcl->mFieldDrag = data->mMag + data->mMagRndm * JPAFieldData::pEmtrInfo->mpCurEmitter->getRandomSF();
    if (ptcl->mFieldDrag > 1.0f)
        ptcl->mFieldDrag = 1.0f;
}

/* 8025B584-8025B5F4       .text calc__12JPADragFieldFP12JPAFieldDataP15JPABaseParticle */
void JPADragField::calc(JPAFieldData* data, JPABaseParticle* ptcl) {
    if (ptcl->checkStatus(0x04)) {
        ptcl->mDrag *= ptcl->mFieldDrag;
    } else {
        f32 affect = JPABaseField::calcFadeAffect(data, ptcl->mCurNormTime);
        ptcl->mDrag *= (1.0f - (affect * (1.0f - ptcl->mFieldDrag)));
    }
}

/* 8025B5F4-8025B718       .text preCalc__12JPASpinFieldFP12JPAFieldData */
void JPASpinField::preCalc(JPAFieldData* data) {
    JPABaseField::preCalc(data);
    MTXMultVec(JPAFieldData::pEmtrInfo->mEmitterGlobalRot, data->mDir, data->mPos);
    data->mPos.normalize();

    Mtx mtx;
    MTXRotAxisRad(mtx, data->mPos, data->mMag);
    data->mWork[0].set(mtx[0][0], mtx[1][0], mtx[2][0]);
    data->mWork[1].set(mtx[0][1], mtx[1][1], mtx[2][1]);
    data->mWork[2].set(mtx[0][2], mtx[1][2], mtx[2][2]);
}

/* 8025B718-8025B7F8       .text calc__12JPASpinFieldFP12JPAFieldDataP15JPABaseParticle */
void JPASpinField::calc(JPAFieldData* data, JPABaseParticle* ptcl) {
    Mtx mtx;
    mtx[0][0] = data->mWork[0].x;
    mtx[1][0] = data->mWork[0].y;
    mtx[2][0] = data->mWork[0].z;
    mtx[0][1] = data->mWork[1].x;
    mtx[1][1] = data->mWork[1].y;
    mtx[2][1] = data->mWork[1].z;
    mtx[0][2] = data->mWork[2].x;
    mtx[1][2] = data->mWork[2].y;
    mtx[2][2] = data->mWork[2].z;
    mtx[0][3] = mtx[1][3] = mtx[2][3] = 0.0f;

    JGeometry::TVec3<f32> newPos;
    MTXMultVecSR(mtx, ptcl->mLocalPosition, newPos);
    // is this a variant of sub()?
    data->mVel.set(
        newPos.x - ptcl->mLocalPosition.x,
        newPos.y - ptcl->mLocalPosition.y,
        newPos.z - ptcl->mLocalPosition.z
    );
    JPABaseField::calcVel(data, ptcl);
}

/* 8025B7F8-8025B960       .text initField__15JPAFieldManagerFP20JPADataBlockLinkInfoP14JPAEmitterInfo */
void JPAFieldManager::initField(JPADataBlockLinkInfo* dataInfo, JPAEmitterInfo* emtrInfo) {
    u8 fieldNum = dataInfo->getFieldNum();
    JPAFieldBlock** fieldBlock = dataInfo->getField();
    for (u8 i = 0; i < fieldNum; i++) {
        if (mVacList->getNumLinks() != 0) {
            JPAFieldData* field = mVacList->getFirst()->getObject();
            mVacList->remove(field->getLinkBufferPtr());
            mList.append(field->getLinkBufferPtr());

            switch (fieldBlock[i]->getType()) {
            case 0: field->mpBaseField = &fc.mGravity; break;
            case 1: field->mpBaseField = &fc.mAir; break;
            case 2: field->mpBaseField = &fc.mMagnet; break;
            case 3: field->mpBaseField = &fc.mNewton; break;
            case 4: field->mpBaseField = &fc.mVortex; break;
            case 5: field->mpBaseField = &fc.mConvection; break;
            case 6: field->mpBaseField = &fc.mRandom; break;
            case 7: field->mpBaseField = &fc.mDrag; break;
            case 8: field->mpBaseField = &fc.mSpin; break;
            default: break;
            }

            JPAFieldData::pEmtrInfo = emtrInfo;
            field->mpBaseField->loadFieldData(field, fieldBlock[i]);
        } else {
            JUT_WARN(0x25c, "%s", "JPariticle::Entry Over!!! (Field Number)");
        }
    }
}

/* 8025B960-8025B9C0       .text init__15JPAFieldManagerFP15JPABaseParticle */
void JPAFieldManager::init(JPABaseParticle* ptcl) {
    for (JSULink<JPAFieldData>* link = mList.getFirst(); link != NULL; link = link->getNext()) {
        JPAFieldData* data = link->getObject();
        data->mpBaseField->init(data, ptcl);
    }
}

/* 8025B9C0-8025BA10       .text preCalc__15JPAFieldManagerFv */
void JPAFieldManager::preCalc() {
    for (JSULink<JPAFieldData>* link = mList.getFirst(); link != NULL; link = link->getNext()) {
        JPAFieldData* data = link->getObject();
        data->mpBaseField->preCalc(data);
    }
}

/* 8025BA10-8025BAD8       .text calc__15JPAFieldManagerFP15JPABaseParticle */
void JPAFieldManager::calc(JPABaseParticle* ptcl) {
    for (JSULink<JPAFieldData>* link = mList.getFirst(); link != NULL; link = link->getNext()) {
        JPAFieldData* data = link->getObject();
        if (data->mSttFlag & 0x80) {
            if (!data->mpBaseField->isItinRange(data, ptcl->mPosition.squared(data->mPos)))
                continue;
        }
        data->mpBaseField->calc(data, ptcl);
    }
}

/* 8025BAD8-8025BB20       .text deleteField__15JPAFieldManagerFP12JPAFieldData */
void JPAFieldManager::deleteField(JPAFieldData* data) {
    mList.remove(data->getLinkBufferPtr());
    mVacList->prepend(data->getLinkBufferPtr());
}

/* 8025BB20-8025BB74       .text deleteAllField__15JPAFieldManagerFv */
void JPAFieldManager::deleteAllField() {
    for (JSULink<JPAFieldData>* link = mList.getFirst(); link != NULL;) {
        JSULink<JPAFieldData>* next = link->getNext();
        JPAFieldData* data = (JPAFieldData*)link->getObject();
        deleteField(data);
        link = next;
    }
}
