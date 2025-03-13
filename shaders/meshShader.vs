#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 aTexCoord;
layout (location = 2) in vec3 aNormal;
layout (location = 3) in vec4 aJoint;
layout (location = 4) in vec4 aWeight;

uniform mat4 uView;
uniform mat4 uProjection;
uniform mat4 uModel;
#define MAX_MATRICES 100
uniform mat4 uJointMat[MAX_MATRICES];
uniform bool uUseJoints;

out vec3 WorldPos;
out vec2 TexCoord;
out vec3 Normal;

void main()
{
    mat4 skinMat = mat4(1.0);
    if (uUseJoints)
    {
        skinMat =
            aWeight.x * uJointMat[int(aJoint.x)] +
            aWeight.y * uJointMat[int(aJoint.y)] +
            aWeight.z * uJointMat[int(aJoint.z)] +
            aWeight.w * uJointMat[int(aJoint.w)];
    }
    WorldPos = vec3(uModel * skinMat * vec4(aPos, 1.0));
    gl_Position = uProjection * uView * vec4(WorldPos, 1.0);
    TexCoord = vec2(aTexCoord.x, aTexCoord.y);
    Normal = mat3(transpose(inverse(uModel))) * aNormal; 
}