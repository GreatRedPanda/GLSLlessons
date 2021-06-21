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

   
float x=floor(uv.x*100.)-50.;
float y=floor(uv.y*100.)-50.;
   
  // x=uv.x;
 //  y=uv.y;
float calcY=sin(iTime)*0.4*x*x*x;
   
//col.x=mod(x, 2.);
    


col.x=smoothstep(calcY-1.,calcY,y)-smoothstep(calcY, calcY+1.,y);
//col.x=sin(iTime);
gl_FragColor=vec4(col.x,0.0,0.0,1.0);
}