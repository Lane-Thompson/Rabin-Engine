#include "pch.h"
#include "L_Age.h"

L_Age::L_Age() : age(0.0f)
{}

void L_Age::on_enter()
{
    float death_age = RNG::range(10.0f, 25.0f);
    auto& bb = agent->get_blackboard();
    bb.set_value<float>("death_age", death_age);
    BehaviorNode::on_leaf_enter();
}

void L_Age::on_update(float dt)
{
    age += dt;

    auto& bb = agent->get_blackboard();
    const float death_age = bb.get_value<float>("death_age");
    float adult_growth = 0.01f;
    agent->set_scaling(agent->get_scaling() + Vec3(adult_growth * dt));

    display_leaf_text();
    if (age > death_age)
    {
        on_success();
    }
}
