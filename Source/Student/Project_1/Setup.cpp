#include <pch.h>
#include "Projects/ProjectOne.h"
#include "Agent/CameraAgent.h"

void ProjectOne::setup()
{
    // Create an agent (using the default "Agent::AgentModel::Man" model)
    auto man = agents->create_behavior_agent("Lumberjack", BehaviorTreeTypes::Lumberjack);
    // You can change properties here or at runtime from a behavior tree leaf node
    // Look in Agent.h for all of the setters, like these:
    auto& manBB = man->get_blackboard();
    manBB.set_value("MinTrees", 2);
    Vec3 manHome = Vec3(2.5f,0,2.5f);
    manBB.set_value("Home", manHome);
    man->set_position(manHome);
    Vec3 manScale = Vec3(3);
    //Vec3 manScale = Vec3(1);
    manBB.set_value("Scale", manScale);

    float targetBuffer = 10.f;
    manBB.set_value("TargetBuffer", targetBuffer); // The distance from the target the agent should stand

    const char* targetType = "Tree";
    manBB.set_value("TargetType", targetType);

    float chop_speed = 2.f;
    float chop_time = 2.f;
    manBB.set_value("chop_speed", chop_speed);
    manBB.set_value("chop_time", chop_time);

    int spawn_requirement = 5;
    manBB.set_value("spawn_requirement", spawn_requirement);
    manBB.set_value("trees_chopped", 0);
    
    man->set_color(Vec3(1, 0, 1));
    man->set_scaling(0.f);

    ///// Home /////
    auto home = agents->create_behavior_agent("Home", BehaviorTreeTypes::Idle, Agent::AgentModel::Hut);
    home->set_scaling(Vec3(0.5));
    home->set_yaw(PI);
    home->set_position(Vec3(3.75, 0, 1));
    home->set_color(Vec3(160.f/255, 82.f/255, 45.f/255));

    // Create an agent with a different 3D model:
    // 1. (optional) Add a new 3D model to the framework other than the ones provided:
    //    A. Find a ".sdkmesh" model or use https://github.com/walbourn/contentexporter
    //       to convert fbx files (many end up corrupted in this process, so good luck!)
    //    B. Add a new AgentModel enum for your model in Agent.h (like the existing Man or Tree).
    // 2. Register the new model with the engine, so it associates the file path with the enum
    //    A. Here we are registering all of the extra models that already come in the package.
    Agent::add_model("Assets\\tree.sdkmesh", Agent::AgentModel::Tree);
    Agent::add_model("Assets\\car.sdkmesh", Agent::AgentModel::Car);
    Agent::add_model("Assets\\bird.sdkmesh", Agent::AgentModel::Bird);
    Agent::add_model("Assets\\ball.sdkmesh", Agent::AgentModel::Ball);
    Agent::add_model("Assets\\hut.sdkmesh", Agent::AgentModel::Hut);
    // 3. Create the agent, giving it the correct AgentModel type.
    auto tree = agents->create_behavior_agent("Tree", BehaviorTreeTypes::Sapling, Agent::AgentModel::Tree);
    // 4. (optional) You can also set the pitch of the model, if you want it to be rotated differently
    tree->set_pitch(PI / 2);
    // 5. (optional) Set other aspects to make it start out correctly
    tree->set_color(Vec3(0, 0.5, 0));   // Set the tree to green
    tree->set_position(Vec3(50, 0, 50));

    Blackboard& treeBB = tree->get_blackboard();
    Vec2  adult_age_range = Vec2(3.f, 15.f);
    float adult_age = RNG::range(adult_age_range.x, adult_age_range.y);
    float adult_growth_rate = 0.03f;
    Vec2  death_age_range = Vec2(10.f, 20.f);
    float death_age = RNG::range(death_age_range.x, death_age_range.y);
    float start_scale = 0.2f;
    float adult_scale = 1.5f;
    treeBB.set_value("adult_age_range",     adult_age_range);
    treeBB.set_value("adult_age",           adult_age);
    treeBB.set_value("adult_growth_rate",   adult_growth_rate);
    treeBB.set_value("start_scale",         start_scale);
    treeBB.set_value("adult_scale",         adult_scale);
    treeBB.set_value("death_age_range",     death_age_range);
    treeBB.set_value("death_age",           death_age);

    
    // You can technically load any map you want, even create your own map file,
    // but behavior agents won't actually avoid walls or anything special, unless you code
    // that yourself (that's the realm of project 2)
    terrain->goto_map(0);

    // You can also enable the pathing layer and set grid square colors as you see fit.
    // Works best with map 0, the completely blank map
    terrain->pathLayer.set_enabled(true);
    terrain->pathLayer.set_value(0, 0, Colors::Red);

    // Camera position can be modified from this default
    auto camera = agents->get_camera_agent();
    camera->set_position(Vec3(-62.0f, 100.0f, terrain->mapSizeInWorld * 0.5f));
    camera->set_pitch(0.72); // 35 degrees

    // Sound control (these sound functions can be kicked off in a behavior tree node - see the example in L_PlaySound.cpp)
    audioManager->SetVolume(0.5f);
    audioManager->PlaySoundEffect(L"Assets\\Audio\\retro.wav");
    // Uncomment for example on playing music in the engine (must be .wav)
    // audioManager->PlayMusic(L"Assets\\Audio\\motivate.wav");
    // audioManager->PauseMusic(...);
    // audioManager->ResumeMusic(...);
    // audioManager->StopMusic(...);
}