#include "pch.h"
#include "L_MoveHome.h"
 


L_MoveHome::L_MoveHome() : home(Vec3(0.f))
{}

void L_MoveHome::on_enter()
{
	BehaviorNode::on_leaf_enter();
	home = agent->get_blackboard().get_value<Vec3>("Home");
}

void L_MoveHome::on_update(float dt)
{
	display_leaf_text();
	if (agent->move_toward_point(home, dt)) on_success();
}
