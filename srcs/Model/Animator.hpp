#pragma once

#include <map>
#include "Model/Animation.hpp"
#include "GlbParser/GlbParser.hpp"

class Animator
{
    public:
        Animator();
        ~Animator();

        void LoadAnimation(const Glb::Animation &animation);
        
        void Play(const std::string &animation) { currentAnimation = animation; }
        void Update();
        glm::mat4 GetNodeTransform(size_t node) const;
    
    private:
        std::map<std::string, Animation> animations;
        std::string currentAnimation;
};