#include "pch.h"
#include "D_EnoughTrees.h"

void D_EnoughTrees::on_enter()
{
	BehaviorNode::on_enter();
}

void D_EnoughTrees::on_update(float dt)
{
	size_t numberOfTrees = agents->get_all_agents_by_type("Tree").size();
	int minimumTrees = agent->get_blackboard().get_value<int>("MinTrees");
	if (numberOfTrees > minimumTrees)
	{
		BehaviorNode* child = children.front();

		child->tick(dt);

		// assume same status as child
		set_status(child->get_status());
		set_result(child->get_result());
	}
	else
	{
		on_failure();
	}
}
