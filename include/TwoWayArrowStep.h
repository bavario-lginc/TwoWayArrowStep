#pragma once

#include "syati.h"

class TwoWayArrowStep : public LiveActor {
public:
    TwoWayArrowStep(const char *);
    ~TwoWayArrowStep();
    virtual void init(const JMapInfoIter &);
    virtual void control();
    virtual void attackSensor(HitSensor *, HitSensor *);
    void exeWait();
    void exeMoveLeft();
    void exeMoveRight();
    void exeDisabled();
    void calcRotate(f32 magnitude);
    f32 mMaxSpeed;
    f32 mAcceleration;
    TVec3f mOriginalRotation;
};

namespace NrvTwoWayArrowStep {
    NERVE(NrvWait);
    NERVE(NrvMoveLeft);
    NERVE(NrvMoveRight);
    NERVE(NrvDisabled);
}

namespace pt {
    extern bool isPlayerFrontSide(const LiveActor *pActor);
}