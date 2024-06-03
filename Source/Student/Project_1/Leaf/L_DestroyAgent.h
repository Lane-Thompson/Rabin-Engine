#pragma once
#include "BehaviorNode.h"
class L_DestroyAgent :
    public BaseNode<L_DestroyAgent>
{

protected:
    virtual void on_enter() override;
};

