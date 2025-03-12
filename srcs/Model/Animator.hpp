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
        
        void Update();
    
    private:
        std::map<std::string, Animation> animations;
};