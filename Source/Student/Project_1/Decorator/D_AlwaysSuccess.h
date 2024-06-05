#pragma once
#include "BehaviorNode.h"
class D_AlwaysSuccess :
    public BaseNode<D_AlwaysSuccess>
{
    void on_enter();
    void on_update(float dt);
};

