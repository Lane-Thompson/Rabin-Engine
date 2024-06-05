#pragma once
#include "BehaviorNode.h"
class D_IsValidTarget :
    public BaseNode <D_IsValidTarget>
{
public:
    D_IsValidTarget();
protected:
    Agent* target;
    void on_enter();
    void on_update(float dt);
};

