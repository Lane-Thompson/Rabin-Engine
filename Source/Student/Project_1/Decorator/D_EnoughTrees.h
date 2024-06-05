#pragma once
#include "BehaviorNode.h"
class D_EnoughTrees :
    public BaseNode< D_EnoughTrees>
{
    void on_enter();
    void on_update(float dt);
};

