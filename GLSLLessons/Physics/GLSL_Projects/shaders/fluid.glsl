#version 430

layout( local_size_x = 1000 ) in;


layout(std430, binding = 0) buffer  Pos 
{
    vec2 position [];
};
layout(std430, binding = 1) buffer  Vel 
{
    vec2 velocity [];
};

layout(std430, binding = 2) buffer  Type 
{
    float type [];
};

uniform float SCREEN_WIDTH          = 1;
uniform float SCREEN_HEIGHT          = 1;

uniform float damp=0.9999999999995;
uniform float extra_damp_limit=0.995*0.4;
uniform float gravity=-0.009;
uniform float speed=0.8;
uniform float deltaT          = 0.00005;
uniform uint N=100;

uvec2 getCoords(uint idx, uint size)
{


uint x=idx/size;

uint xx=idx-x*size;

uint y=xx;
return uvec2 (x,y);

}

uint getIdx(uint x, uint y, uint N)
{
    x = (x < 0) ? 0 : x;
    y= (y < 0) ? 0 : y;
    x = (x >=N) ? N-1 : x;
    y = (y >= N) ? N - 1 : y;
return x*N+y;
}

void main() 
{
    uint idx = gl_GlobalInvocationID.x;
	
	
	vec2 v= velocity [idx];
	
	uvec2 coords=getCoords(idx, N);

	
	uint x=coords.x;
	uint y=coords.y;


if(type[idx]==1)
{
uvec2 nCoords=uvec2(0);

nCoords=uvec2(coords+sign(v));
uint idx2=getIdx(nCoords.x,nCoords.y,N);

      if( coords.y>0 && type[idx2]!=1)
      {
          type[idx]=0;
          type[idx2]=1;
          velocity[idx]=vec2(0.0,0.0);
          velocity[idx2]=v+vec2(v.x*gravity, gravity);
      }
	  if(type[idx2]==1 && sign(v.x)!=0.0)
	  {
	  
	    velocity[idx2]=vec2( -velocity[idx2].x, velocity[idx2].y);
	  }


     if(nCoords.y<0)
         velocity[idx]=vec2(0,0);

}
 

}