#include "pch.h"
#include "L_ChopTree.h"

L_ChopTree::L_ChopTree() 
	: target(nullptr), startYaw(0), totalTime(1.f), chopSpeed(0), chopTime(4.f) {}

void L_ChopTree::on_enter()
{
	Blackboard& bb = agent->get_blackboard();
	target = bb.get_value<Agent*>("Target");
	startYaw = agent->get_yaw();
	chopSpeed = bb.get_value<float>("chop_speed");
	chopTime  = bb.get_value<float>("chop_time");
	BehaviorNode::on_leaf_enter();
	totalTime = 0;
}

void L_ChopTree::on_update(float dt)
{
	display_leaf_text();
	totalTime += dt;
	if (totalTime > chopTime)
	{
		if (totalTime > chopTime + 2.f)
		{
			on_success();
			return;
		}
		target->set_yaw(startYaw);
		target->set_pitch(0);
		agent->set_yaw(startYaw);
	}
	else
	{
		agent->set_yaw(startYaw + (PI * 0.5f) * sin(totalTime * chopSpeed));
	}
}

void L_ChopTree::on_exit()
{
	agents->destroy_agent(target);
	
	Blackboard& bb = agent->get_blackboard();

	int treesChopped = bb.get_value<int>("trees_chopped");
	bb.set_value("trees_chopped", ++treesChopped);

	bb.set_value("Target", nullptr);
}
