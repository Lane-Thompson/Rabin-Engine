#include "pch.h"
#include "L_Disappear.h"

void L_Disappear::on_enter()
{
	BehaviorNode::on_leaf_enter();
	agent->set_scaling(Vec3(0.f));
	BehaviorNode::on_success();
}
