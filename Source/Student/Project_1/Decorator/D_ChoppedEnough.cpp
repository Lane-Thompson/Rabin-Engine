#include "pch.h"
#include "D_ChoppedEnough.h"

D_ChoppedEnough::D_ChoppedEnough() : treesChopped(0), spawnRequirement(0)
{
}

void D_ChoppedEnough::on_enter()
{
	BehaviorNode::on_enter();

	Blackboard& bb = agent->get_blackboard();
	treesChopped = bb.get_value<int>("trees_chopped");
	spawnRequirement = bb.get_value<int>("spawn_requirement");
}

void D_ChoppedEnough::on_update(float dt)
{
	BehaviorNode* child = children.front();

	if (treesChopped  == spawnRequirement)
	{
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
