#version 130
#ifdef GL_ES
precision mediump float;
#endif


uniform vec2 iResolution;
uniform sampler2D iFall;
uniform sampler2D iPrints;

void main()
{
 float ratioWH = iResolution.x / iResolution.y;
 float ratioHW = iResolution.y / iResolution.x;
    vec2 adjMul = vec2(ratioWH, 1.);
    vec2 adjSub = vec2((1.-ratioHW) * 0.5 * ratioWH, 0.);
    vec2 uv = (gl_FragCoord.xy/iResolution.xy) * adjMul - adjSub;
	

vec3 col =vec3(0.);
vec4 tf=texture2D(iFall, uv);


vec4 tp=texture2D(iPrints, vec2(uv.x, 1-uv.y));

col.x=tf.x+tp.x;
gl_FragColor=vec4(col,1.0);
}