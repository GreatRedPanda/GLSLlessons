#version 130
#ifdef GL_ES
precision mediump float;
#endif

uniform vec2 iResolution;
uniform float iTime;

float randNumber(float seed, float min, float max)
{
   float r1= mix( 0.,1.,mod(sin(seed)*100.,2.));
   float r2=  mix(0.,1.,mod(cos(seed)*100.,3.));
   float r3=  mix(0.,1.,mod(cos(seed)*100.,5.));
   float r4=  mix(0.,1.,mod(sin(seed)*100.,7.));

    float f2=sin(r1+r2-r3-r4);
  //f2=f2*max-min;
    return f2;
}

vec3 randVec(float seed)
{
    vec3 rand=vec3(0);
    rand.x=randNumber(seed,0.,1.);
      rand.y=randNumber(seed+133.,0.,1.);
      rand.z=randNumber(seed+2312.,0.,1.);
    return rand;
}

float getRandShift(int timeCeloe)
{
if(timeCeloe<1)
return 0.5;
if(timeCeloe==1)
return 0.1;

if(timeCeloe==2)
return 0.2;
if(timeCeloe==3)
return 0.3;
if(timeCeloe==4)
return 0.14;
if(timeCeloe==5)
return 0.7;
if(timeCeloe==6)
return 0.6;
if(timeCeloe==7)
return 0.232;
if(timeCeloe==8)
return 23;
if(timeCeloe==9)
return 34453;

}
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

int timePart=int(mod(int(mod(iTime,100)),10));
float even=getRandShift(timePart);
vec3 col =vec3(0.);
uv.y+=fract(time);
for(int i=0; i<14; i++)
{
col.x+=drawSquare(vec2(0.01,0.01),vec2(clamp(randVec(even*i).x+0.03*i, 0,0.99),1),uv);
}
gl_FragColor=vec4(col,1.0);
}