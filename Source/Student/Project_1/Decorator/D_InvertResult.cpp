#include "pch.h"
#include "D_InvertResult.h"

void D_InvertResult::on_update(float dt)
{
    BehaviorNode* child = children.front();

    child->tick(dt);

    if (child->succeeded() == true)
    {
        on_failure();
    }
    else if (child->failed() == true)
    {
        on_success();
    }
}
