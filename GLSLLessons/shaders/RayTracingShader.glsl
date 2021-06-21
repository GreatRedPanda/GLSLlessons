#version 430


struct Mesh
{
mat4 Model;
int vertexIndexOffset;
};

layout( local_size_x = 32, local_size_y =32,local_size_z = 1 ) in;
layout (rgba32f, binding = 0) uniform image2D destTex;
layout (rgba32f, binding = 1) uniform sampler2D skybox;
layout (std430, binding = 2) buffer Meshes
{

 mat4 meshes[];
};

layout (std430, binding = 3) buffer Vertex
{

 vec4 vertecies[];
};
layout (std430, binding = 4) buffer Normals
{

 vec4 normals[];
};
const float pos_infinity = uintBitsToFloat(0x7F800000);
const float neg_infinity = uintBitsToFloat(0x7F800000);
const float PI = 3.14159265f;




struct Ray
{
vec3 origin;
vec3 direction;
vec3 energy;

};
struct Camera
{
vec3 Position;
vec3 Forward;
mat4 View;
mat4 Projection;
mat4 Model;
};
struct RayHit
{
    vec3 position;
    float distance;
    vec3 normal;
	
};

struct Object
{
mat4 Model;
};



uniform Camera camera;


RayHit CreateRayHit()
{
    RayHit hit;
    hit.position = vec3(0.0f, 0.0f, 0.0f);
    hit.distance = pos_infinity;
    hit.normal = vec3(0.0f, 0.0f, 0.0f);
    return hit;
}
Ray createRay(vec3 origin, vec3 direction)
{
    Ray ray;
    ray.origin = origin;
    ray.direction = direction;
	ray.energy=vec3(1,1,1);
    return ray;
}
bool intersectSphere(Ray ray, inout RayHit bestHit, vec4 sphere)
{

     vec3 d = ray.origin - sphere.xyz;
    float p1 = -dot(ray.direction, d);
    float p2sqr = p1 * p1 - dot(d, d) + sphere.w * sphere.w;
    if (p2sqr < 0)
        return false;
    float p2 = sqrt(p2sqr);
    float t = p1 - p2 > 0 ? p1 - p2 : p1 + p2;
    if (t > 0 && t < bestHit.distance)
    {
        bestHit.distance = t;
        bestHit.position = ray.origin + t * ray.direction;
        bestHit.normal = normalize(bestHit.position - sphere.xyz);
		return true;
    }

return false;
}


bool rayTriangleIntersect(Ray ray, inout RayHit bestHit, vec3 center, int idx, int objectID)
{

	 vec3 inverseDir=vec3(1)/ray.direction;
	vec3 raySign=vec3(sign(inverseDir));
float epsilon=0.000000008;
vec3 v0=(meshes[objectID]*vertecies[idx]).xyz;
vec3 v1=(meshes[objectID]*vertecies[idx+1]).xyz;
vec3 v2=(meshes[objectID]*vertecies[idx+2]).xyz;
vec3 e1=v1-v0;
vec3 e2=v2-v0;
vec3 normal=cross(e1,e2);



normal= normals[idx+1].xyz;

float nDotRayDir=dot(normal, ray.direction);

if(abs(nDotRayDir)<epsilon)
{

return false;
}

float d=-dot(normal, v0);

float t=-(dot(ray.origin, normal)+d)/nDotRayDir;

if(t<0)
return false;

vec3 p=ray.origin+t*ray.direction;

vec3 edge1=v1-v0;
vec3 c1=cross(edge1, p-v0);
if(dot( c1,cross(e1,e2))<0) return false;

vec3 edge2=v2-v1;
vec3 c2=cross(edge2, p-v1);
if(dot( c2,cross(e1,e2))<0) return false;

vec3 edge3=v0-v2;
vec3 c3=cross(edge3, p-v2);
if(dot( c3, cross(e1,e2))<0) return false;



if(t<bestHit.distance)
{
bestHit.distance = t;
bestHit.position = p;
bestHit.normal = normal;
}
return true;

}


bool  checkRayCubes(Ray ray,inout RayHit hit)
{
     bool totalRes=false;
     for(int i=0; i<meshes.length(); i++)
      {
	       vec4 pos=vec4(0,0,0,1);
		   for(int j=0; j<vertecies.length(); j+=3)
		   {		   
		    bool result=rayTriangleIntersect(ray, hit, pos.xyz, j,i);
		     if(result)
		     totalRes= true;		   
		   }
	     
	  }
     return totalRes;
}

bool  checkRay(Ray ray,inout RayHit hit)
{
bool totalRes=false;
     for(int i=0; i<meshes.length(); i++)
      {
	  vec4 pos=vec4(0,0,0,1);
	       bool result=intersectSphere(ray, hit, vec4(pos.xyz, 0.5));
		   if(result)
		   totalRes= true;
	  }
     return totalRes;
}
vec3 trace(Ray ray)
{
vec3 result=vec3(1,0,0);

		for(int i=0; i<3; i++)
		{
		    RayHit hit=CreateRayHit();
           bool rayResult=checkRayCubes(ray, hit);
		   
		   if(rayResult)
		   {
		    
		   // reflect, reduce energy
		     vec3 specular = vec3(0.6f, 0.6f, 0.6f);
       
               ray.origin = hit.position + hit.normal * 0.001f;
               ray.direction = reflect(ray.direction, hit.normal);
               ray.energy *= specular;
			   if(i==0)
		       result= normalize(hit.normal * 0.001f)*hit.distance;//*vec3(0.7);
			   else //if(i==1)
			   result+=ray.energy*vec3(0.7);
			   //result=vec3(1,1,0);
			   
		 
		   }
		   else
		   {
		   ray.energy=vec3(0);
		     float theta = acos(ray.direction.y) / -PI;
              float phi = atan(ray.direction.x, -ray.direction.z) / -PI * 0.5f;
			  //if(result!=vec3(1,0,0))
			  result+=texture(skybox, vec2(theta, phi)).xyz;
			  //result=vec3(1,1,1);
		   // ray goes to Sky, get skybox coords
		   break;
		   }

		}
		return result;
}


void main() 
{
    uint idx = gl_GlobalInvocationID.x;
	
	ivec2 wgID=ivec2(gl_WorkGroupID);
	ivec2 threadXY=ivec2(gl_LocalInvocationID);
	
    ivec2 pos=ivec2(gl_GlobalInvocationID.xy);
	
	//pos=wgID*32+threadXY;
	
	vec4 color=vec4(pos.x/720.0,pos.y/720.0,0,1);
	vec2 uv=vec2(pos.x/1920.0,pos.y/1280.0);
float width=1920.0;
float height=1280.0;
 // uv = vec2((pos.xy + vec2(0.5f, 0.5f)) / vec2(width, height) * 2.0f - 1.0f);

    vec3 origin = ((camera.Model)*vec4(1,1,1, 1.0f)).xyz;
    vec3 direction = (inverse(camera.Projection)*vec4(uv, 0.0f, 1.0f)).xyz;
    direction = (inverse(camera.Model)* vec4(direction, 0.0f)).xyz;
    direction = normalize(direction);	
	Ray r=createRay(camera.Position,direction);
	
	color.xyz=trace(r);

	imageStore(destTex, pos, color);
}