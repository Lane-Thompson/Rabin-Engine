#pragma once
#include "BehaviorNode.h"
class L_Age :
    public BaseNode<L_Age>
{
public:
    L_Age();

protected:
    float age;
    float deathAge;
    float adultGrowthRate;

    virtual void on_enter() override;
    virtual void on_update(float dt) override;
};

