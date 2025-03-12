#include "Model/Animator.hpp"

Animator::Animator()
{
}

Animator::~Animator()
{
}

void Animator::LoadAnimation(const Glb::Animation &animation)
{
    Animation anim(animation); 
    std::string name = animation.name;
    animations.insert(std::make_pair(name, Animation(animation)));
    animations[name] = Animation(animation);
}

void Animator::Update()
{
    for (auto animation: animations)
        animation.second.Update();
}