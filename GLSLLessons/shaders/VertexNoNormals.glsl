#version 430 
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 aTexCoord;
out vec3 Position;
out vec3 Normal;
out vec2 UV;


uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{

  gl_Position =projection*view*model* vec4(aPos.xyz, 1.0);
  Normal = mat3(transpose(inverse(model))) * aNormal;  
//view*
  Position=vec3(model * vec4(aPos, 1.0));
  UV=aTexCoord;
};