#pragma once
#include "BehaviorNode.h"
class L_ChopTree :
    public BaseNode<L_ChopTree>
{
public:
    L_ChopTree();
protected:
    Agent* target;
    float startYaw;
    float totalTime;
    float chopSpeed;
    float chopTime;
    void on_enter();
    void on_update(float dt);
    void on_exit ();
};

