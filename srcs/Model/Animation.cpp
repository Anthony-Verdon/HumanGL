#include "Model/Animation.hpp"
#include "Time/Time.hpp"

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

#include <iostream>
void Animation::Update()
{
    timer += Time::getDeltaTime();
    for (size_t i = 0; i < data.channels.size(); i++)
    {
        //size_t node = data.channels[i].node;
        size_t samplerIndex = data.channels[i].sampler;

        auto sampler = data.samplers[samplerIndex];
        size_t keyframe = 0;
        for (; keyframe < sampler.timecodes.size() - 1; keyframe++)
        {
            if (sampler.timecodes[keyframe + 1] > timer)
                break;
        }

        std::cout << keyframe << " " << sampler.timecodes[keyframe] << " < " << timer << " < " << sampler.timecodes[keyframe + 1] << std::endl;
    }
    std::cout << "-------" << std::endl;
}
