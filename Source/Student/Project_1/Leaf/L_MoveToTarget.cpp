#include "pch.h"
#include "L_MoveToTarget.h"

void L_MoveToTarget::on_enter()
{
	Agent* target = agent->get_blackboard().get_value<Agent*>("Target");
	Vec3 targetToPlayer = agent->get_position() - target->get_position();
	targetToPlayer.Normalize();

	float targetBuffer = agent->get_blackboard().get_value<float>("TargetBuffer");
	targetPos = target->get_position() + (targetToPlayer * targetBuffer);

	BehaviorNode::on_leaf_enter();
}

void L_MoveToTarget::on_update(float dt)
{
	display_leaf_text();
	if (agent->get_blackboard().get_value<Agent*>("Target") == nullptr)
	{
		printf("AJSDKFJASKDFJLKASDJFLKASJKLJASDF\n");
		on_failure();
		return;
	}
	if (agent->move_toward_point(targetPos, dt)) 
	{
		BehaviorNode::on_success();
		return;
	}

}