#include "pch.h"
#include "L_Grow.h"

L_Grow::L_Grow() : age(0.0f), deltaGrowth(0.f)
{}

void L_Grow::on_enter()
{
    auto& bb = agent->get_blackboard();
    float adult_age = 5.0f;
    bb.set_value<float>("adult_age", adult_age);
    float startScale = 0.2f;
    float adultScale = 4.0f;
    agent->set_scaling(0.2);
    deltaGrowth = (adultScale - startScale) / adult_age;
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