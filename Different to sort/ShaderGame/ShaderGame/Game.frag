#version 130
#ifdef GL_ES
precision mediump float;
#endif

uniform vec2 iResolution;
uniform float iTime;

uniform sampler2D iTexture;
uniform sampler2D iFallTexture;

void main()
{
 float ratioWH = iResolution.x / iResolution.y;
 float ratioHW = iResolution.y / iResolution.x;
    vec2 adjMul = vec2(ratioWH, 1.);
    vec2 adjSub = vec2((1.-ratioHW) * 0.5 * ratioWH, 0.);
    vec2 uv = (gl_FragCoord.xy/iResolution.xy) * adjMul - adjSub;
	

vec3 col =vec3(0.);
vec4 texFallRColor=texture2D(iFallTexture, vec2(uv.x,1-uv.y));


vec4 texColor=texture2D(iTexture, vec2(uv.x,1-uv.y));
vec4 texColorNeighbourDown=texture2D(iTexture, vec2(uv.x,1-uv.y+0.02));
col.x+=texColor.x;
//float lTOColor=length();
if( ( uv.y<0.02 || texColorNeighbourDown.x>0 )  && texFallRColor.x>0)
{
col.x=texFallRColor.x;
}
if (uv.y>0.04 && texColorNeighbourDown.x==0  )
{

  if(texFallRColor.x>0)
  col.x=1;
  else
  {
		col.x=0;
  }
  
}
gl_FragColor=vec4(col,1.0);
}