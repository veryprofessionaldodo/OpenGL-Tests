#version 330 core
out vec4 FragColor;

in vec3 shaderColor;
in vec2 texCoords;

uniform sampler2D texture1;
uniform sampler2D texture2;

void main()
{
    FragColor = mix(texture(texture1, texCoords) * vec4(shaderColor, 1.0f), texture(texture2, texCoords), 0.2f);
}
