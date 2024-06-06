#include "pch.h"
#include "L_GetRandomPosition.h"

void L_GetRandomPosition::on_enter()
{
	on_leaf_enter();
	float x = RNG::range<float>(0, 100);
	float y = agent->get_position().y;
	float z = RNG::range<float>(0, 100);

	agent->get_blackboard().set_value("Target", Vec3(x, y, z));
	on_success();
}
