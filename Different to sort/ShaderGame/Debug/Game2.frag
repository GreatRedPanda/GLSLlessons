#version 130
#ifdef GL_ES
precision mediump float;
#endif

uniform vec2 iResolution;
uniform float iSpace;

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
vec4 texFallRColor=texture2D(iFallTexture, uv);


vec4 texColor=texture2D(iTexture, vec2(uv.x, 1-uv.y));

col.x+=texColor.x;
if( texFallRColor.x>0 && iSpace>0)
{


col.x+=1;
}

gl_FragColor=vec4(col,1.0);
}