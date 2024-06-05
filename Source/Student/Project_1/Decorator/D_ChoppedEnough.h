#pragma once
#include "BehaviorNode.h"
class D_ChoppedEnough :
    public BaseNode<D_ChoppedEnough>
{
public:
    D_ChoppedEnough();
protected:
    int treesChopped;
    int spawnRequirement;
    void on_enter();
    void on_update(float dt);
};

