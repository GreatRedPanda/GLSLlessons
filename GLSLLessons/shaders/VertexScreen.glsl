#version 430 
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 aTexCoord;
out vec3 Position;
out vec2 UV;


void main()
{

  gl_Position = vec4(aPos.xyz, 1.0);
  Position=aPos.xyz;
  UV=aTexCoord;
};