#version 330
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoord;
out vec3 FragPosition;
out vec3 Normal;
out vec2 UV;
out vec4 FragPosLightSpace;
uniform mat4 view;
uniform mat4 projection;
uniform mat4 model;
uniform mat4 lightSpaceMatrix;
void main()
{
UV=aTexCoord;
FragPosition=vec3(model * vec4(aPos, 1.0));
Normal = mat3(transpose(inverse(model))) * aNormal; 
FragPosLightSpace = lightSpaceMatrix * vec4(FragPosition, 1.0);
gl_Position=projection*view*model*vec4(aPos.xyz, 1.0);

 



}