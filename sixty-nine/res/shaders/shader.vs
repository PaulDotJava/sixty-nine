#version 330 core

layout(location = 0) in vec4 position;
layout(location = 1) in vec2 uv;

out vec2 uvCoord;

uniform mat4 u_ProjectionMatrix;
uniform mat4 u_ModelMatrix;
uniform mat4 u_ViewMatrix;

void main()
{
    gl_Position = u_ProjectionMatrix * u_ModelMatrix * position;
    uvCoord = uv;
}