#include "TwoWayArrowStep.h"

TwoWayArrowStep::TwoWayArrowStep(const char *pName) : LiveActor(pName) {
    mMaxSpeed = 30.0f;
    mAcceleration = 1.0f;
}
TwoWayArrowStep::~TwoWayArrowStep() {

}

void TwoWayArrowStep::init (const JMapInfoIter &rIter) {
    MR::processInitFunction(this, rIter, false);
    MR::getJMapInfoArg0NoInit(rIter, &mMaxSpeed);
    MR::getJMapInfoArg1NoInit(rIter, &mAcceleration);
    MR::useStageSwitchReadA(this, rIter);
    MR::useStageSwitchWriteB(this, rIter);
    makeActorAppeared();
    initNerve(&NrvTwoWayArrowStep::NrvWait::sInstance, 0);
    mOriginalRotation = TVec3f(mRotation);
}

namespace pt {
    void initRailToNearestAndRepositionWithGravity(LiveActor *pActor);
}

void TwoWayArrowStep::control () {
    mRailRider->move();
    MR::moveTransToCurrentRailPos(this);
    pt::initRailToNearestAndRepositionWithGravity(this);
    MR::setBckRate(this, abs(mRailRider->mSpeed / 10));
    if (mRailRider->isReachedGoal() || mRailRider->isReachedEdge()) 
        mRailRider->mSpeed = 0;
    
    setNerve(&NrvTwoWayArrowStep::NrvWait::sInstance);
    if (MR::isValidSwitchA(this) && !MR::isOnSwitchA(this)) 
        setNerve(&NrvTwoWayArrowStep::NrvDisabled::sInstance);
}

void TwoWayArrowStep::attackSensor (HitSensor *pReceiever, HitSensor *pSender) {
    if (!isNerve(&NrvTwoWayArrowStep::NrvDisabled::sInstance) && MR::isSensorPlayerOrYoshiOrRide(pSender) && MR::isOnGroundPlayer()) {
        if (MR::isValidSwitchB(this)) 
            MR::onSwitchB(this);
        if (!pt::isPlayerFrontSide(this)) 
            setNerve(&NrvTwoWayArrowStep::NrvMoveLeft::sInstance);
        else 
            setNerve(&NrvTwoWayArrowStep::NrvMoveRight::sInstance);
    }
}

void TwoWayArrowStep::exeWait () {
    MR::startBtk(this, "DirNone");
    calcRotate(0);
    if (mRailRider->mSpeed < 0) 
        mRailRider->mSpeed += mAcceleration;
    else if (mRailRider->mSpeed > 0) 
        mRailRider->mSpeed -= mAcceleration;
}

void TwoWayArrowStep::exeMoveLeft () {
    MR::tryStartActionNotPlaying(this, "MoveLeft");
    MR::startBtk(this, "DirLeft");
    calcRotate(-5);
    mRailRider->mSpeed -= mAcceleration;
    if (mRailRider->mSpeed < -mMaxSpeed) 
        mRailRider->mSpeed = -mMaxSpeed;
}

void TwoWayArrowStep::exeMoveRight () {
    MR::tryStartActionNotPlaying(this, "MoveRight");
    MR::startBtk(this, "DirRight");
    calcRotate(5);
    mRailRider->mSpeed += mAcceleration;
    if (mRailRider->mSpeed > mMaxSpeed) 
        mRailRider->mSpeed = mMaxSpeed;
}

void TwoWayArrowStep::exeDisabled () {
    MR::startBtk(this, "Disabled");
    calcRotate(0);
    if (mRailRider->mSpeed < 0) 
        mRailRider->mSpeed += mAcceleration;
    else if (mRailRider->mSpeed > 0) 
        mRailRider->mSpeed -= mAcceleration;
}

void TwoWayArrowStep::calcRotate (f32 magnitude) {
    TVec3f sideVec;
    MR::calcSideVec(&sideVec, this);
    mRotation = mOriginalRotation + sideVec * magnitude;
}

namespace NrvTwoWayArrowStep {
    void NrvWait::execute (Spine *pSpine) const {
        TwoWayArrowStep *pActor = (TwoWayArrowStep *)pSpine->mExecutor;
        pActor->exeWait();
    }
    void NrvMoveLeft::execute (Spine *pSpine) const {
        TwoWayArrowStep *pActor = (TwoWayArrowStep *)pSpine->mExecutor;
        pActor->exeMoveLeft();
    }
    void NrvMoveRight::execute (Spine *pSpine) const {
        TwoWayArrowStep *pActor = (TwoWayArrowStep *)pSpine->mExecutor;
        pActor->exeMoveRight();
    }
    void NrvDisabled::execute (Spine *pSpine) const {
        TwoWayArrowStep *pActor = (TwoWayArrowStep *)pSpine->mExecutor;
        pActor->exeDisabled();
    }

    NrvWait(NrvWait::sInstance);
    NrvMoveLeft(NrvMoveLeft::sInstance);
    NrvMoveRight(NrvMoveRight::sInstance);
    NrvDisabled(NrvDisabled::sInstance);
}