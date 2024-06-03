#include "pch.h"
#include "L_DuplicateTree.h"
#include "TreeInfo.h"

void L_DuplicateTree::on_enter ()
{
	int childrenCount = RNG::d3();

	for (int i = 0; i < childrenCount; i++) // Spawn children in random positions near tree
	{
		float angle = RNG::range(0, 360) * PI/180;
		int distance = RNG::range(8, 25);

		auto child = agents->create_behavior_agent("Tree", BehaviorTreeTypes::Sapling, Agent::AgentModel::Tree);

		Vec3 childDiff = Vec3(distance * cos(angle), 0, distance * sin(angle));
		child->set_position(agent->get_position() + childDiff);
		child->set_pitch(PI / 2);			// Set vertical
		child->set_color(Vec3(0, 0.5, 0));  // Set the tree to green
	}

	BehaviorNode::on_leaf_enter();
}