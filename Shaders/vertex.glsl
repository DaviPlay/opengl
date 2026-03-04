#version 410 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTextureCoords;
layout (location = 3) in vec2 aTangent;
layout (location = 4) in vec2 aBitangent;
layout (location = 5) in vec2 aBonesIDs;
layout (location = 6) in vec2 aWheight;

out vec2 textureCoords;
out vec3 fragPos;
out vec3 normal;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
   gl_Position = projection * view * model * vec4(aPos, 1.0);
   textureCoords = aTextureCoords;
   fragPos = vec3((model) * vec4(aPos, 1.0));
   normal = mat3(transpose(inverse(model))) * aNormal;
}