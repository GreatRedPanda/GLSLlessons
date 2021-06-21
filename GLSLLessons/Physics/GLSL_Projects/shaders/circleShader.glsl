#version 430 
in vec2 Position;

void main()
{
vec2 lightPos=vec2(-1, 0.6);
float radius=1;

float radius2=0.3;

float l=length(vec2(0.0)- Position);
float alpha=1-smoothstep(radius-0.001, radius+0.001, l);


float l2=length(lightPos- Position);
float dark=1-smoothstep(radius2-0.001, radius2+0.001, l2);
dark=1-mix(0, radius, l2);
dark=clamp(0.1, 0.5, dark);
   gl_FragColor = vec4(0.7+dark, 0.6+dark, 0.1+dark, alpha );
};