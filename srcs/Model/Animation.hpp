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

        glm::vec3 CalculateLerp(const glm::vec3 &previousPoint, const glm::vec3 &nextPoint, float interpolation);
        glm::quat CalculateSlerp(const glm::quat &previousPoint, const glm::quat &nextPoint, float interpolation);
};