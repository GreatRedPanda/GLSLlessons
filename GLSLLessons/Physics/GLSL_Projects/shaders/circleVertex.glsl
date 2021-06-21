#version 430 
layout (location = 0) in vec2 aPos;
out vec2 Position;

uniform mat4 model;

void main()
{

   gl_Position = model*vec4(aPos.x, aPos.y, 0.0, 1.0);
   Position=vec2(aPos.x, aPos.y);

};