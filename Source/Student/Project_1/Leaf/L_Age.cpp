#include "pch.h"
#include "L_Age.h"

L_Age::L_Age() : age(0.0f)
{}

void L_Age::on_enter()
{

    auto& bb = agent->get_blackboard();
    bb.set_value<float>("death_age", 5.f);
    BehaviorNode::on_leaf_enter();
}

void L_Age::on_update(float dt)
{
    age += dt;

    auto& bb = agent->get_blackboard();
    const float death_age = bb.get_value<float>("death_age");
    display_leaf_text();
    if (age > death_age)
    {
        on_success();
    }
}
