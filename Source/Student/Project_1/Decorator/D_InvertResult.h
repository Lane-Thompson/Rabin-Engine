#pragma once
#include "BehaviorNode.h"
class D_InvertResult : public BaseNode<D_InvertResult>
{
protected:
    virtual void on_update(float dt) override;
};

