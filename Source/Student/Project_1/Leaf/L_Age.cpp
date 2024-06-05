#include "pch.h"
#include "L_Age.h"

L_Age::L_Age() : age(0.0f), deathAge(0), adultGrowthRate(0)
{}

void L_Age::on_enter()
{
    BehaviorNode::on_leaf_enter();
    auto& bb = agent->get_blackboard();
    deathAge = bb.get_value<float>("death_age");
    adultGrowthRate = bb.get_value<float>("adult_growth_rate");
}

void L_Age::on_update(float dt)
{
    age += dt;


    agent->set_scaling(agent->get_scaling() + Vec3(adultGrowthRate * dt));

    display_leaf_text();
    if (age > deathAge)
    {
        on_success();
    }
}
