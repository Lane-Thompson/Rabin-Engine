#include "pch.h"
#include "L_MoveToTargetPosition.h"
void L_MoveToTargetPosition::on_enter()
{
	targetPos = agent->get_blackboard().get_value<Vec3>("Target");

	BehaviorNode::on_leaf_enter();
}

void L_MoveToTargetPosition::on_update(float dt)
{
	display_leaf_text();
	if (agent->move_toward_point(targetPos, dt))
	{
		BehaviorNode::on_success();
		return;
	}
}