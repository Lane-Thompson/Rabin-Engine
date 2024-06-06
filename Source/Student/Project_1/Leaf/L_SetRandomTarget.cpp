#include "pch.h"
#include "L_SetRandomTarget.h"

void L_SetRandomTarget::on_enter()
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
	int last = static_cast<int>(targets.size() - 1);
	Agent* target = targets[RNG::range(0, last)];

	bb.set_value("Target", target);
	on_success();
}
