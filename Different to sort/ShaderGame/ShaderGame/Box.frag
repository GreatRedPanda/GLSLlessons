#version 130
#ifdef GL_ES
precision mediump float;
#endif

uniform vec2 iResolution;
uniform float iTime;

mat2 rotation2D(float angle)
{

mat2 m=mat2(cos(angle), -sin(angle), sin(angle), cos(angle));
return m;
}

		float box2(vec2 center, vec2 size, vec2 coord)
		{

        vec2 b=smoothstep(center-size, center, coord)-smoothstep(center, center+size, coord);
		
      if(b.x>0 && b.y>0)
	  return 1.;
	  else
	  return 0.;

		}
		float box(float x, float y, float funcValue, float xRegion)
		{

		float line=smoothstep(funcValue-12.,funcValue,y)-smoothstep(funcValue, funcValue+12.,y);
	    float def=step(-1.*xRegion,x)-step(xRegion,x);
	
			if(def!=1.0)
			{
			line=0.;
			}
		return line;
		}
void main()
{
 float ratioWH = 320.0 / 320.0;
 float ratioHW = 320.0/ 320.0;
    vec2 adjMul = vec2(ratioWH, 1.);
    vec2 adjSub = vec2((1.-ratioHW) * 0.5 * ratioWH, 0.);
    vec2 uv = (gl_FragCoord.xy/vec2(320.0,320.0)) * adjMul - adjSub;
	
	

  vec3 col =vec3(0);


float x=floor(uv.x*100.)-50.;
float y=floor(uv.y*100.)-50.;

vec2 center=vec2(-20.,20.);

vec2 coord =vec2(x,y);
coord -=center;
coord=rotation2D(3.14*iTime/10.)*coord;
coord+=center;
col.x=box2(center, vec2(10,20),coord );
col.x+=box2(center, vec2(20,10),coord);
gl_FragColor=vec4(col.x,0.0,0.0,1.0);
}