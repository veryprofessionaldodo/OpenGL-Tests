#version 330 core
out vec4 FragColor;

in vec2 texCoords;
in vec3 shaderColor;

uniform sampler2D texture1;
uniform sampler2D texture2;

void main() 
{
    FragColor = mix(texture(texture1, vec2(texCoords.s*2.0f, texCoords.t*2.0f)), texture(texture2, vec2(1.0f- texCoords.s*2.0f, texCoords.t*2.0f)), 0.2f);    
}