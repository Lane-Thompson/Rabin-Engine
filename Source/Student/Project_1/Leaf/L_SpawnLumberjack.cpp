#include "pch.h"
#include "L_SpawnLumberjack.h"
#include "TreeInfo.h"

void L_SpawnLumberjack::on_enter()
{
	BehaviorNode::on_leaf_enter();

	Blackboard& bb = agent->get_blackboard();
	bb.set_value("trees_chopped", 0);

	BehaviorAgent* newJack = agents->create_behavior_agent("Lumberjack", BehaviorTreeTypes::Lumberjack);

    newJack->set_position(bb.get_value<Vec3>("Home"));
	newJack->set_color(RNG::color().ToVector3());
	newJack->set_scaling(0);

    Blackboard& newBB = newJack->get_blackboard();
    newBB.set_value("MinTrees", bb.get_value<int>("MinTrees"));
    newBB.set_value("Home", bb.get_value<Vec3>("Home"));
    newBB.set_value("Scale", bb.get_value<Vec3>("Scale"));
    newBB.set_value("TargetBuffer", bb.get_value<float>("TargetBuffer")); // The distance from the target the agent should stand
    newBB.set_value("TargetType", bb.get_value<const char*>("TargetType"));
    newBB.set_value("chop_speed", bb.get_value<float>("chop_speed"));
    newBB.set_value("chop_time", bb.get_value<float>("chop_time"));
    newBB.set_value("spawn_requirement", bb.get_value<int>("spawn_requirement"));
    newBB.set_value("trees_chopped", 0);
    on_success();
}