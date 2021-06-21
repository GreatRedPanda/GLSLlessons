#version 430 
in vec3 Position;
in vec3 Normal;

uniform vec3 LightSourceColor;
void main()
{



   gl_FragColor = vec4(LightSourceColor, 1.0 );
};