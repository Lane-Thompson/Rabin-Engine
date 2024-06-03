#pragma once
#include "BehaviorNode.h"
class L_TreeDeath :
    public BaseNode<L_TreeDeath>
{
protected:

    virtual void on_enter() override;
};

