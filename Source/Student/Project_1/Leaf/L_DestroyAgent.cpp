#include "pch.h"
#include "L_DestroyAgent.h"

void L_DestroyAgent::on_enter()
{
	BehaviorNode::on_leaf_enter();
	agents->destroy_agent(agent);
	on_success();
}
