#include "pch.h"
#include "D_AlwaysSuccess.h"

void D_AlwaysSuccess::on_enter()
{
	BehaviorNode::on_enter();
}

void D_AlwaysSuccess::on_update(float dt)
{
	BehaviorNode* child = children.front();

	child->tick(dt);
	set_status(child->get_status());
	if (child->get_result() != NodeResult::IN_PROGRESS)
		set_result(NodeResult::SUCCESS);
}
