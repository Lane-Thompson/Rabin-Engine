#pragma once
#include "BehaviorNode.h"
class L_DuplicateTree :
    public BaseNode<L_DuplicateTree>
{
protected:
    virtual void on_enter() override;
    float rando_color(float start, float range);
};

