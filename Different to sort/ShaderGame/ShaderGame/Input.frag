#version 130
#ifdef GL_ES
precision mediump float;
#endif
uniform vec4 iInput;
uniform vec2 iResolution;
uniform float iTime;

float drawSquare(vec2 size, vec2 center, vec2 uv)
{
vec2 rect=smoothstep(center-size, center, uv)-smoothstep(center, center+size, uv);

   if(rect.x>0 && rect.y>0)
	  return 1.;
	  else
	  return 0.;
}
void main()
{
 float ratioWH = iResolution.x / iResolution.y;
 float ratioHW = iResolution.y / iResolution.x;
    vec2 adjMul = vec2(ratioWH, 1.);
    vec2 adjSub = vec2((1.-ratioHW) * 0.5 * ratioWH, 0.);
    vec2 uv = (gl_FragCoord.xy/iResolution.xy) * adjMul - adjSub;
	
	
float time=(mod(iTime,iResolution.y));

vec2 dir=vec2(-0.5,0.7);
float mag=pow( dir.x*dir.x+dir.y*dir.y,0.5);
float cosA=abs(dir.x)/mag;
float sinA=abs(dir.y)/mag;

float x=dir.x*cosA*iTime;
float y=dir.y*sinA*iTime-9*iTime*iTime/2;

vec3 col =vec3(0.);
//uv.x+=iInput.x+iInput.y;
//uv.y+=iInput.z+iInput.w;


col.x+=drawSquare(vec2(0.01,0.01),vec2(0.1,0.1),vec2(x,y)+uv);

gl_FragColor=vec4(col,1.0);
}