#include "pch.h"
#include "L_Grow.h"

L_Grow::L_Grow() : age(0.0f), deltaGrowth(0.f)
{}

void L_Grow::on_enter()
{
    auto& bb = agent->get_blackboard();
    const float adult_age = bb.get_value<float>("adult_age");
    const float start_scale = bb.get_value<float>("start_scale");
    const float adult_scale = bb.get_value<float>("adult_scale");
    agent->set_scaling(start_scale);
    deltaGrowth = (adult_scale - start_scale) / adult_age;
    BehaviorNode::on_leaf_enter();
}

void L_Grow::on_update(float dt)
{
    age += dt;

    auto& bb = agent->get_blackboard();
    const float adult_age = bb.get_value<float>("adult_age");
    display_leaf_text();
    agent->set_scaling(agent->get_scaling() + Vec3(deltaGrowth * dt));
    if (age > adult_age)
    {
        on_success();
    }
}