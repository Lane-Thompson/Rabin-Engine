#include "pch.h"
#include "L_DuplicateTree.h"
#include "TreeInfo.h"

void L_DuplicateTree::on_enter ()
{
	Blackboard& bb = agent->get_blackboard();
	int minSpawn = bb.get_value<int>("min_spawn");
	int maxSpawn = bb.get_value<int>("max_spawn");
	int childrenCount = RNG::range(minSpawn, maxSpawn);
	for (int i = 0; i < childrenCount; i++) // Spawn children in random positions near tree
	{
		float angle = RNG::range(0, 360) * PI/180;
		int distance = RNG::range(8, 25);

		auto child = agents->create_behavior_agent("Tree", BehaviorTreeTypes::Sapling, Agent::AgentModel::Tree);

		Blackboard& childBB = child->get_blackboard();
		Vec2 adult_age_range = bb.get_value<Vec2>("adult_age_range");
		float adult_age = RNG::range(adult_age_range.x, adult_age_range.y);
		float adult_growth_rate = bb.get_value<float>("adult_growth_rate");
		Vec2  death_age_range = bb.get_value<Vec2>("death_age_range");
		float death_age = RNG::range(death_age_range.x, death_age_range.y);
		float color_range = bb.get_value<float>("color_range");

		float start_scale = bb.get_value<float>("start_scale");
		float adult_scale = bb.get_value<float>("adult_scale");
		childBB.set_value("adult_age_range",	adult_age_range);
		childBB.set_value("adult_age",			adult_age);
		childBB.set_value("adult_growth_rate",	adult_growth_rate);
		childBB.set_value("start_scale",		start_scale);
		childBB.set_value("adult_scale",		adult_scale);
		childBB.set_value("death_age_range",	death_age_range);
		childBB.set_value("death_age",			death_age);
		childBB.set_value("min_spawn",			2);
		childBB.set_value("max_spawn",			4);
		childBB.set_value("color_range",  		color_range);

		Vec3 childDiff = Vec3(distance * cos(angle), 0, distance * sin(angle));
		child->set_position(agent->get_position() + childDiff);
		child->set_pitch(PI / 2);			// Set vertical

		float red = rando_color(agent->get_color().x, color_range);
		float green = rando_color(agent->get_color().y, color_range);
		float blue = rando_color(agent->get_color().z, color_range);
		Vec3 newColor = Vec3(red, green, blue);
		child->set_color(newColor);
	}

	BehaviorNode::on_leaf_enter();
}

float L_DuplicateTree::rando_color(float start, float range)
{
	float change = RNG::range<float>(-range, range);
	float newColor = std::clamp((start * 255) + change, 0.f, 255.f);
	return newColor / 255.f;
}