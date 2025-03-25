#pragma once

#include "GlbParser/GlbParser.hpp"

class Animation
{
    public:
        Animation(const std::vector<Glb::Node> &nodes, const Glb::Animation &animation);
        ~Animation();

        void Reset();
        void Update();
        ml::mat4 GetNodeTransform(size_t node) const;

    private:
        Glb::Animation data;
        std::map<int, ml::mat4> nodesTransform;
        std::map<int, ml::mat4> nodesInitTransform;
        float timer;

        ml::vec3 CalculateLerp(const ml::vec3 &previousPoint, const ml::vec3 &nextPoint, float interpolation);
        ml::vec4 CalculateSlerp(const ml::vec4 &previousPoint, const ml::vec4 &nextPoint, float interpolation);
};