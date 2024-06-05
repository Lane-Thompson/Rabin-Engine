#pragma once
#include "BehaviorNode.h"
class L_MoveHome :
    public BaseNode<L_MoveHome>
{
public:
    L_MoveHome();
protected:
    Vec3 home;
    void on_enter();
    void on_update(float dt);
};

