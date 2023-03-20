#version 330 core

layout(location = 0) out vec4 FragColor;

in vec2 uvCoord;

uniform sampler2D u_Sampler;

uniform vec4 u_Color;

void main()
{
    vec4 texColor = texture(u_Sampler, uvCoord);
    texColor.x *= u_Color.x;
    texColor.y *= u_Color.y;
    texColor.z *= u_Color.z;
    texColor.a *= u_Color.a;
    FragColor = texColor;
}