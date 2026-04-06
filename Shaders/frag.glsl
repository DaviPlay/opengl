#version 330 core

out vec4 FragColor;

in vec2 textureCoords;

uniform sampler2D textureImage1;
uniform sampler2D textureImage2;
uniform float mixAmount;

void main()
{
   FragColor = mix(texture(textureImage1, textureCoords), texture(textureImage2, textureCoords), mixAmount);
}