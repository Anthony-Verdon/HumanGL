#pragma once

#include <map>
#include "Model/Animator/Animation/Animation.hpp"
#include "GlbParser/GlbParser.hpp"

class Animator
{
    public:
        Animator();
        ~Animator();

        void LoadAnimation(const Glb::Animation &animation);
        
        void Play(const std::string &animation);
        void Update();
        ml::mat4 GetNodeTransform(size_t node) const;
        std::vector<std::string> GetAnimationsName() const;
    
    private:
        std::map<std::string, Animation> animations;
        std::string currentAnimation;
};