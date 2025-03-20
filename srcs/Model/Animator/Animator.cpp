#include "Model/Animator/Animator.hpp"

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

ml::mat4 Animator::GetNodeTransform(size_t node) const
{
    auto animation = animations.find(currentAnimation);
    if (animation != animations.end())
    {
        return (animation->second.GetNodeTransform(node));
    }
    else
    {
        ml::mat4 matrix;
        matrix.identity();
        return (matrix);
    }
}

std::vector<std::string> Animator::GetAnimationsName() const
{
    std::vector<std::string> animationsName;

    for (auto it = animations.begin(); it != animations.end(); it++)
        animationsName.push_back(it->first);

    return (animationsName);
}

void Animator::Play(const std::string &name)
{
    auto animation = animations.find(name);
    if (animation != animations.end())
    {
        currentAnimation = name;
        animation->second.Reset();
    }
}