#include "pch.h"
#include "D_IsValidTarget.h"

D_IsValidTarget::D_IsValidTarget() : target(nullptr)
{
}

void D_IsValidTarget::on_enter()
{
	BehaviorNode::on_enter();
	target = agent->get_blackboard().get_value<Agent*>("Target");
}

void D_IsValidTarget::on_update(float dt)
{
	std::vector<Agent*> trees = agents->get_all_agents_by_type("Tree");
	if (std::find(trees.begin(), trees.end(), target) == trees.end())
	{
		//printf("Invalid\n");
		on_failure();
		return;
	}

	BehaviorNode* child = children.front();
	child->tick(dt);

	// assume same status as child
	set_status(child->get_status());
	set_result(child->get_result());
}
