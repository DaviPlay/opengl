#version 410 core
out vec4 FragColor;

in vec3 ourColor;
in vec2 TexCoord;

// texture samplers
//uniform sampler2D texture1;
//uniform sampler2D texture2;

void main()
{
    FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);
}