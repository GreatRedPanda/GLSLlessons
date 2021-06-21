#version 130
#ifdef GL_ES
precision mediump float;
#endif

uniform vec2 iResolution;
uniform float iTime;
uniform sampler2D neonTexture;
//uniform vec2 lightDir;
void main()
{
 float ratioWH = iResolution.x / iResolution.y;
 float ratioHW = iResolution.y / iResolution.x;
    vec2 adjMul = vec2(ratioWH, 1.);
    vec2 adjSub = vec2((1.-ratioHW) * 0.5 * ratioWH, 0.);
    vec2 uv = (gl_FragCoord.xy/iResolution.xy) * adjMul - adjSub;
	
	
vec3 normal=vec3(0.5,0.5,0.5);
vec3 lightDir=vec3(1.,0.5,0.);
vec3 col =vec3(0.);


vec3 diffuse=vec3(1.)*reflect(normal,lightDir);
vec4 tex=texture2D(neonTexture, uv);


if(gl_FrontFacing)
col.xyz=mix(tex.xyz*dot(normal,lightDir),tex.xyz,tex.a);


gl_FragColor=vec4(col,1.0);
}