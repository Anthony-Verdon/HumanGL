#include "Model/Animator/Animation/Animation.hpp"
#include "Time/Time.hpp"
#include <iostream>
#include "geometry/geometry.hpp"

Animation::Animation(const std::vector<Glb::Node> &nodes, const Glb::Animation &animation)
{   
    data = animation;
    timer = 0;
    for (size_t i = 0; i < animation.channels.size(); i++)
        nodesTransform[animation.channels[i].node].identity();
    for (size_t i = 0; i < nodes.size(); i++)
        nodesInitTransform[i] = nodes[i].transform;
}

Animation::~Animation()
{
}

void Animation::Reset()
{
    timer = 0;
    for (auto it = nodesTransform.begin(); it != nodesTransform.end(); it++)
        it->second.identity();
}

void Animation::Update()
{
    timer += Time::getDeltaTime();
    for (auto it = nodesTransform.begin(); it != nodesTransform.end(); it++)
        it->second.identity();

    for (size_t i = 0; i < data.channels.size(); i++)
    {
        size_t node = data.channels[i].node;
        size_t samplerIndex = data.channels[i].sampler;

        auto sampler = data.samplers[samplerIndex];
        size_t keyframe = 0;
        for (; keyframe < sampler.timecodes.size() - 1; keyframe++)
        {
            if (sampler.timecodes[keyframe + 1] > timer)
                break;
        }
        if (keyframe == sampler.timecodes.size() - 1)
        {
            keyframe = 0;
            timer = 0;
        }
        size_t nextKeyFrame = (keyframe + 1) % sampler.timecodes.size();
        size_t previousBufferIndex = keyframe * sampler.nbElement;
        size_t nextBufferIndex = nextKeyFrame * sampler.nbElement;
        float totalTime = sampler.timecodes[nextKeyFrame] - sampler.timecodes[keyframe];
        float interpolation = 0;
        if (totalTime > 0)
            interpolation = (timer - sampler.timecodes[keyframe]) / totalTime;
        auto it = nodesTransform.find(node);
        if (data.channels[i].type == "translation")
        {
            ml::vec3 previousPoint = ml::vec3(sampler.data[previousBufferIndex + 0], sampler.data[previousBufferIndex + 1], sampler.data[previousBufferIndex + 2]);
            if (sampler.interpolation == "STEP")
            {
                it->second *= ml::translate(previousPoint);
            }
            else if (sampler.interpolation == "LINEAR")
            {
                ml::vec3 nextPoint = ml::vec3(sampler.data[nextBufferIndex + 0], sampler.data[nextBufferIndex + 1], sampler.data[nextBufferIndex + 2]);
                it->second *= ml::translate(CalculateLerp(previousPoint, nextPoint, interpolation));
            }
        }
        else if (data.channels[i].type == "rotation")
        {
            ml::vec4 previousQuat = ml::vec4(sampler.data[previousBufferIndex + 0], sampler.data[previousBufferIndex + 1], sampler.data[previousBufferIndex + 2], sampler.data[previousBufferIndex + 3]);
            if (sampler.interpolation == "STEP")
            {
                it->second *= ml::rotate(previousQuat);
            }
            else if (sampler.interpolation == "LINEAR")
            {
                ml::vec4 nextQuat = ml::vec4(sampler.data[nextBufferIndex + 0], sampler.data[nextBufferIndex + 1], sampler.data[nextBufferIndex + 2], sampler.data[nextBufferIndex + 3]);
                it->second *= ml::rotate(CalculateSlerp(previousQuat, nextQuat, interpolation));
            }

        }
        else if (data.channels[i].type == "scale")
        {
            ml::vec3 previousPoint = ml::vec3(sampler.data[previousBufferIndex + 0], sampler.data[previousBufferIndex + 1], sampler.data[previousBufferIndex + 2]);
            if (sampler.interpolation == "STEP")
            {
                it->second *= ml::scale(previousPoint);
            }
            else if (sampler.interpolation == "LINEAR")
            {
                ml::vec3 nextPoint = ml::vec3(sampler.data[nextBufferIndex + 0], sampler.data[nextBufferIndex + 1], sampler.data[nextBufferIndex + 2]);
                it->second *= ml::scale(CalculateLerp(previousPoint, nextPoint, interpolation));
            }
        }
    }
}

ml::mat4 Animation::GetNodeTransform(size_t node) const
{
    auto nodeTransform = nodesTransform.find(node);
    auto nodeInitTransform = nodesInitTransform.find(node);
    if (nodeTransform != nodesTransform.end())
    {
        return (nodeTransform->second);
    }
    else if (nodeInitTransform != nodesInitTransform.end())
    {
        return (nodeInitTransform->second);
    }
    else
    {
        ml::mat4 matrix;
        matrix.identity();
        return (matrix);
    }
}

ml::vec3 Animation::CalculateLerp(const ml::vec3 &previousPoint, const ml::vec3 &nextPoint, float interpolation)
{
    return (previousPoint * (1 - interpolation) + nextPoint * interpolation);

}

ml::vec4 Animation::CalculateSlerp(const ml::vec4 &previousQuat, const ml::vec4 &nextQuat, float interpolation)
{
    float dotProduct = ml::dotProduct(previousQuat, nextQuat);
    
    ml::vec4 nextQuat2 = nextQuat;
    if (dotProduct < 0.0)
    {
        nextQuat2 = -nextQuat2;
        dotProduct = -dotProduct;
    }
        
    if (dotProduct > 0.9995)
        return ml::normalize(previousQuat * (1 - interpolation) + nextQuat2 * interpolation);

    float theta_0 = acos(dotProduct);
    float theta = interpolation * theta_0;
    float sin_theta = sin(theta);
    float sin_theta_0 = sin(theta_0);
    
    float scalePreviousQuat = cos(theta) - dotProduct * sin_theta / sin_theta_0;
    float scaleNextQuat = sin_theta / sin_theta_0;
    return scalePreviousQuat * previousQuat + scaleNextQuat * nextQuat2;
}