#pragma once
#include "BehaviorNode.h"
class L_MoveToTargetPosition :
    public BaseNode<L_MoveToTargetPosition>
{
private:
    Vec3 targetPos;
protected:
    void on_enter();
    void on_update(float dt);
};

