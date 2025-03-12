#include "Model/Animation.hpp"
#include "Time/Time.hpp"
#include <iostream>
#include <glm/gtc/matrix_transform.hpp>
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/quaternion.hpp>
#include <glm/gtx/string_cast.hpp>

Animation::Animation(const Glb::Animation &animation)
{   
    data = animation;
    timer = 0;
    for (size_t i = 0; i < animation.channels.size(); i++)
        nodesTransform[animation.channels[i].node] = glm::mat4(1.0f);
}

Animation::~Animation()
{
}

void Animation::Reset()
{
    timer = 0;
    for (auto it = nodesTransform.begin(); it != nodesTransform.end(); it++)
        it->second = glm::mat4(1.0f);
}

void Animation::Update()
{
    timer += Time::getDeltaTime();
    for (auto it = nodesTransform.begin(); it != nodesTransform.end(); it++)
        it->second = glm::mat4(1.0f);

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
        size_t nextKeyFrame = (keyframe + 1) % (sampler.timecodes.size() - 1);
        size_t previousBufferIndex = keyframe * sampler.nbElement;
        size_t nextBufferIndex = nextKeyFrame * sampler.nbElement;
        float interpolation = (timer - sampler.timecodes[keyframe]) / (sampler.timecodes[nextKeyFrame] - sampler.timecodes[keyframe]);
        auto it = nodesTransform.find(node);
        if (data.channels[i].type == "translation")
        {
            glm::vec3 previousPoint = glm::vec3(sampler.data[previousBufferIndex + 0], sampler.data[previousBufferIndex + 1], sampler.data[previousBufferIndex + 2]);
            if (sampler.interpolation == "STEP")
            {
                it->second *= glm::translate(glm::mat4(1.0f), previousPoint);
            }
            else if (sampler.interpolation == "LINEAR")
            {
                glm::vec3 nextPoint = glm::vec3(sampler.data[nextBufferIndex + 0], sampler.data[nextBufferIndex + 1], sampler.data[nextBufferIndex + 2]);
                it->second *= glm::translate(glm::mat4(1.0f), CalculateLerp(previousPoint, nextPoint, interpolation));
            }
        }
        else if (data.channels[i].type == "rotation")
        {
            glm::quat previousQuat = glm::quat(sampler.data[previousBufferIndex + 3], sampler.data[previousBufferIndex + 0], sampler.data[previousBufferIndex + 1], sampler.data[previousBufferIndex + 2]);
            if (sampler.interpolation == "STEP")
            {
                it->second *= glm::toMat4(previousQuat);
            }
            else if (sampler.interpolation == "LINEAR")
            {
                glm::quat nextQuat = glm::quat(sampler.data[nextBufferIndex + 3], sampler.data[nextBufferIndex + 0], sampler.data[nextBufferIndex + 1], sampler.data[nextBufferIndex + 2]);
                it->second *= glm::toMat4(CalculateSlerp(previousQuat, nextQuat, interpolation));
            }

        }
        else if (data.channels[i].type == "scale")
        {
            glm::vec3 previousPoint = glm::vec3(sampler.data[previousBufferIndex + 0], sampler.data[previousBufferIndex + 1], sampler.data[previousBufferIndex + 2]);
            if (sampler.interpolation == "STEP")
            {
                it->second *= glm::scale(glm::mat4(1.0f), previousPoint);
            }
            else if (sampler.interpolation == "LINEAR")
            {
                glm::vec3 nextPoint = glm::vec3(sampler.data[nextBufferIndex + 0], sampler.data[nextBufferIndex + 1], sampler.data[nextBufferIndex + 2]);
                it->second *= glm::scale(glm::mat4(1.0f), CalculateLerp(previousPoint, nextPoint, interpolation));
            }
        }
    }
}

glm::mat4 Animation::GetNodeTransform(size_t node) const
{
    auto nodeTransform = nodesTransform.find(node);
    if (nodeTransform != nodesTransform.end())
        return (nodeTransform->second);
    else
        return (glm::mat4(1.0f));
}

glm::vec3 Animation::CalculateLerp(const glm::vec3 &previousPoint, const glm::vec3 &nextPoint, float interpolation)
{
    return previousPoint + interpolation * (nextPoint - previousPoint);
}

glm::quat Animation::CalculateSlerp(const glm::quat &previousQuat, const glm::quat &nextQuat, float interpolation)
{
    float dotProduct = glm::dot(previousQuat, nextQuat);
    
    glm::quat nextQuat2 = nextQuat;
    if (dotProduct < 0.0)
    {
        nextQuat2 = -nextQuat2;
        dotProduct = -dotProduct;
    }
        
    if (dotProduct > 0.9995)
        return glm::normalize(previousQuat + interpolation * (nextQuat2 - previousQuat));

    float theta_0 = acos(dotProduct);
    float theta = interpolation * theta_0;
    float sin_theta = sin(theta);
    float sin_theta_0 = sin(theta_0);
    
    float scalePreviousQuat = cos(theta) - dotProduct * sin_theta / sin_theta_0;
    float scaleNextQuat = sin_theta / sin_theta_0;
    return scalePreviousQuat * previousQuat + scaleNextQuat * nextQuat2;
}