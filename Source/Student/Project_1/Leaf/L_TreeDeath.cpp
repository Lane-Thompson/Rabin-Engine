#include "pch.h"
#include "L_TreeDeath.h"

void L_TreeDeath::on_enter()
{
	agent->set_pitch(0);
	BehaviorNode::on_leaf_enter();
	on_success();
}
