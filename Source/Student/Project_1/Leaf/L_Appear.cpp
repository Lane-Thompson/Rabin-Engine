#include "pch.h"
#include "L_Appear.h"

void L_Appear::on_enter()
{
	BehaviorNode::on_leaf_enter();
	agent->set_scaling(agent->get_blackboard().get_value<Vec3>("Scale"));
	BehaviorNode::on_success();
}
