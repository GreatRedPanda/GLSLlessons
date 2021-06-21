#version 130
#ifdef GL_ES
precision mediump float;
#endif

uniform vec2 iResolution;
uniform float iTime;


void main()
{
 float ratioWH = 320.0 / 320.0;
 float ratioHW = 320.0/ 320.0;
    vec2 adjMul = vec2(ratioWH, 1.);
    vec2 adjSub = vec2((1.-ratioHW) * 0.5 * ratioWH, 0.);
    vec2 uv = (gl_FragCoord.xy/vec2(320.0,320.0)) * adjMul - adjSub;
	
	

  vec3 col =vec3(0);

float r1=0.4;

vec2 c1=vec2(0.2,-0.4)*sin(iTime);
vec2 c2=vec2(0.3,-0.1)*cos(iTime*0.2);
vec2 c3=vec2(0.1,0.1)*sin(iTime*1.2);


float x=uv.x-0.5;
float y=uv.y-0.5;
vec2 coord=vec2(x,y);
vec2 calc1=coord-c1;
vec2 calc2=coord-c2;
vec2 calc3=coord-c3;
float time=iTime;
vec3 l=vec3(0);
l.x=1.-smoothstep(length(calc1),length(calc1)+0.02,r1);
l.y=1.-smoothstep(length(calc2),length(calc2)+0.02,r1);
l.z=1.-smoothstep(length(calc3),length(calc3)+0.02,r1);

for(int i=0; i<3;i++)
{
time+=0.07;

		for(int j=0; j<3;j++)
		{
          col[i]+=mod(l[j],ratioWH)*l[i]*sin(time*atan(y,x));
			}
	    

}

gl_FragColor=vec4(col,iTime);
}