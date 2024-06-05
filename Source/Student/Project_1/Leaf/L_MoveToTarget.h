#pragma once
#include "BehaviorNode.h"
class L_MoveToTarget :
    public BaseNode<L_MoveToTarget>
{
private:
    Vec3 targetPos;
protected:
    void on_enter();
    void on_update(float dt);
};

