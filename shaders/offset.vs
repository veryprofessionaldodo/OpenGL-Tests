#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 vertexColor;

uniform vec3 offset;

out vec3 shaderColor;

void main() 
{
    gl_Position = vec4(aPos.x + offset.x, aPos.y + offset.y, aPos.z + offset.z, 1.0);
    shaderColor = vertexColor;
}