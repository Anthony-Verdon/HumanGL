#include "Model/Animator.hpp"

Animator::Animator()
{
}

Animator::~Animator()
{
}

void Animator::LoadAnimation(const Glb::Animation &animation)
{
    animations.insert(std::make_pair(animation.name, Animation(animation)));
}

void Animator::Update()
{
    for (auto &animation: animations)
        animation.second.Update();
}