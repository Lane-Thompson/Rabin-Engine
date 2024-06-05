#include "pch.h"
#include "L_TreeDeath.h"

void L_TreeDeath::on_enter()
{
	agent->set_yaw (RNG::range(0.f, 360.f));
	agent->set_pitch(0);
	BehaviorNode::on_leaf_enter();
	on_success();
	display_leaf_text();
}
