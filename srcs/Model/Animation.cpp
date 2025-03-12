#include "Model/Animation.hpp"
#include "Time/Time.hpp"
#include <iostream>
#include <glm/gtc/matrix_transform.hpp>
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/quaternion.hpp>

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
    if (timer > 5.5f)
        timer -= 5.5f;
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

        auto it = nodesTransform.find(node);
        if (data.channels[i].type == "translation")
        {
            size_t buffeIndex = keyframe * sampler.nbElement;
            it->second *= glm::translate(it->second, glm::vec3(sampler.data[buffeIndex + 0], sampler.data[buffeIndex + 1], sampler.data[buffeIndex + 2]));
        }
        else if (data.channels[i].type == "rotation")
        {
            size_t buffeIndex = keyframe * sampler.nbElement;
            it->second *= glm::toMat4(glm::quat(sampler.data[buffeIndex + 3], sampler.data[buffeIndex + 0], sampler.data[buffeIndex + 1], sampler.data[buffeIndex + 2]));
        }
        else if (data.channels[i].type == "scale")
        {
            size_t buffeIndex = keyframe * sampler.nbElement;
            it->second *= glm::scale(it->second,  glm::vec3(sampler.data[buffeIndex + 0], sampler.data[buffeIndex + 1], sampler.data[buffeIndex + 2]));
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
