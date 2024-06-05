#include "pch.h"
#include "L_SetNearestTarget.h"

void L_SetNearestTarget::on_enter()
{
	BehaviorNode::on_leaf_enter();

	Blackboard& bb = agent->get_blackboard();
	const char* targetType = bb.get_value<const char*>("TargetType");
	std::vector<Agent*> targets = agents->get_all_agents_by_type(targetType);
	
	if (targets.size() < bb.get_value<int>("MinTrees"))
	{
		on_failure();
		return;
	}
	
	Vec3 position = agent->get_position();
	Agent* bestTarget = targets[0];
	float bestDistance = Vec3::Distance(bestTarget->get_position(), agent->get_position());
	for (int i = 1; i < targets.size(); ++i)
	{
		Agent* target = targets[i];

		float newDistance = Vec3::Distance(target->get_position(), position);
		if (newDistance < bestDistance)
		{
			target = bestTarget;
			bestDistance = newDistance;
		}
	}

	bb.set_value("Target", bestTarget);
	on_success();
}
