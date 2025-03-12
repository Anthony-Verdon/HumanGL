#pragma once

#include "GlbParser/GlbParser.hpp"

class Animation
{
    public:
        Animation(const Glb::Animation &animation);
        ~Animation();

        void Reset();
        void Update();
        glm::mat4 GetNodeTransform(size_t node) const;

    private:
        Glb::Animation data;
        std::map<int, glm::mat4> nodesTransform;
        float timer;
};