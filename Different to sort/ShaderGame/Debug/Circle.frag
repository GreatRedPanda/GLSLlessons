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

float radius=length(uv)*sin(iTime);

float x=uv.x-0.5;
float y=uv.y-0.5;

float tt=atan(y,x);
float rr=cos(tt*3.3)*sin(iTime);


	float l,z=iTime;
		vec2 uv_t=uv;
	vec2 p=uv;
	//	uv=p;
		p-=.5;
		p.x*=ratioWH;
	for(int i=0;i<3;i++) 
	{

		z+=.07;
		l=length(p);
		uv_t+=p/l*(sin(z)+1.)*abs(sin(l*9.-z*2.));
		col[i]=.01/length(abs(mod(uv_t,1.)-.5));
	}
//col=vec3(rr,1.-rr,0);
gl_FragColor=vec4(col/l,iTime);
}