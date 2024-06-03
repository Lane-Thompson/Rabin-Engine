#pragma once
#include "BehaviorNode.h"
class L_Grow :
    public BaseNode<L_Grow>
{
public:
    L_Grow();

protected:
    float age;
    float deltaGrowth;

    virtual void on_enter() override;
    virtual void on_update(float dt) override;
};

