#version 430

layout( local_size_x = 1000, local_size_x = 2 ) in;

uniform float SCREEN_WIDTH          = 4.0;
uniform float SCREEN_HEIGHT          = 4.0;
uniform float deltaT          = 0.0005;
uniform float damp=0.95;
uniform float extra_damp_limit=0.95*0.4;
uniform float gravity=0.4;
uniform float speed=0.8;

layout(std430, binding = 0) buffer Pos 
{
    vec4 position [];
};
layout(std430, binding = 1) buffer Vel 
{
    vec4 velocity [];
};

uvec3 getCoords(uint idx, uint size)
{

uint z= idx/(size*size);
uint zz=idx-z*size*size;

uint y=zz/size;

uint yy=zz-y*size;

uint x=yy;
return uvec3 (x,y,z);

}

uint getIdx(uvec3 coords, uint size)
{

return coords.x+coords.y*size+coords.z*size*size;
}

void main() 
{
   
    uint idx = gl_GlobalInvocationID.x;
	vec3 p   = position [idx].xyz;
	vec3 v   = velocity [idx].xyz;
	v=vec3(0);
	v.x*=damp;
	v.y+=gravity;
	v.y*=damp;
	
	float x_off = 0.0f, y_off = 0.0f;
	float dist_sum = 0.0f;
	
	
	
	for(int i=0; i<position.length(); i++)
	{
	        float dx=p.x-position[i].x;
	        float dy=p.y-position[i].y;
			float dist = dx * dx + dy * dy; 
		
		if(dist > 1.0f) {
		
		   float dy1 = dy - 1.0f;
			dist*= dist;
			dist_sum+= 1.0f / dist;
			
			dist/= speed;
			 float mult = dist * 0.01f; 
		
			 float offset_x = dx / mult;
			x_off+= clamp(offset_x, -0.4f, 0.4f);
			
		 float offset_y = dy1 / mult;
			
			y_off+= clamp(offset_y, -0.4f, 0.4f);
			
			dist*= 0.0002f; 
			
			 float speed_x = dx / dist;
			 
			v.x+= clamp(speed_x, -0.2f, 0.2f);
			
			 float speed_y = dy1 / dist;
			
			v.y+= clamp(speed_y, -0.2f, 0.2f);
		}
	}
	
	float extra_damp = 1.0f + (damp - 1.0f) * dist_sum * 20000.0f;

	if(extra_damp < extra_damp_limit) 
	 extra_damp = extra_damp_limit;
	 
	v.x*= extra_damp;
	v.y*= extra_damp;
	//Actually move the particle
	p.x+= x_off + v.x;
	p.y+= y_off + v.y;


	if(p.x < 0.0f) {
		v.x = -v.x * 0.5f;
		p.x = -p.x;
	}
	 if(p.x > SCREEN_WIDTH) 
	{
		v.x = -v.x * 0.5f;
		p.x = SCREEN_WIDTH * 2.0f - p.x;
	}
	if(p.y > SCREEN_HEIGHT) {
		v.y = -v.y * 0.5f;
		p.y = SCREEN_HEIGHT * 2.0f - p.y;
	}
    if(p.y <0.0f) {
		v.y =0;
		p.y =  0;
	}
   velocity [idx]= vec4(v,1.0);
    position [idx] =vec4(p, 1.0);
}