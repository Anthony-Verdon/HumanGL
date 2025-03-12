#include "Model/Animator.hpp"

Animator::Animator()
{
    currentAnimation = "";
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
    auto animation = animations.find(currentAnimation);
    if (animation != animations.end())
        animation->second.Update();
}

glm::mat4 Animator::GetNodeTransform(size_t node) const
{
    auto animation = animations.find(currentAnimation);
    if (animation != animations.end())
        return (animation->second.GetNodeTransform(node));
    else
        return (glm::mat4(1.0f));
}
