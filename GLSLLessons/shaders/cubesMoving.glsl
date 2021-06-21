#version 430

layout( local_size_x = 1000 ) in;


uniform float deltaT          = 0.0005;
layout(std430, binding = 0) buffer Pos 
{
    vec4 position [];
};
layout(std430, binding = 1) buffer Vel 
{
    vec4 velocity [];
};
layout(std430, binding = 2) buffer Vel0 
{
    vec4 velocity0 [];
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
void setBNDVelStart(int c, uint index, uint N)
{

	uvec3 coords=getCoords(index, N);
	uint i=coords.x; uint j=coords.y; uint k=coords.z;
	
	if(coords.z==0)
	{
	   if(c==2)
	    velocity0[index][c]=-velocity0[getIdx(uvec3(i,j,1), N)][c];
     else
	    velocity0[index][c]= velocity0[getIdx(uvec3(i,j,1), N)][c];	
	}
	
	if(coords.z==N-1)
	{
	   if(c==2)
	    velocity0[index][c]=-velocity0[getIdx(uvec3(i,j,N-2), N)][c];
     else
	    velocity0[index][c]= velocity0[getIdx(uvec3(i,j,N-2), N)][c];	
	}
	//
	if(j==0)
	{
	   if(c==1)
	    velocity0[index][c]=-velocity0[getIdx(uvec3(i,1,k), N)][c];
     else
	    velocity0[index][c]= velocity0[getIdx(uvec3(i,1,k), N)][c];	
	}
	
	if(j==N-1)
	{
	   if(c==1)
	    velocity0[index][c]=-velocity0[getIdx(uvec3(i,N-2,k), N)][c];
     else
	    velocity0[index][c]= velocity0[getIdx(uvec3(i,N-2,k), N)][c];	
	}
	//
		if(i==0)
	{
	   if(c==0)
	    velocity0[index][c]=-velocity0[getIdx(uvec3(1,j,k), N)][c];
     else
	    velocity0[index][c]= velocity0[getIdx(uvec3(1,j,k), N)][c];	
	}
	
	if(i==N-1)
	{
	   if(c==0)
	    velocity0[index][c]=-velocity0[getIdx(uvec3(N-2,j,k), N)][c];
     else
	    velocity0[index][c]= velocity0[getIdx(uvec3(N-2,j,k), N)][c];	
	}

	if(coords.x==0 && coords.y==0 && coords.z==0)
	  velocity0[index][c]       = 0.33f * (velocity0[getIdx(uvec3(1,0,0), N)][c]
                                  + velocity0[getIdx(uvec3(0,1,0), N)][c]
                                  + velocity0[getIdx(uvec3(0,0,1), N)][c]								  
								  );
	//2							  
	if(coords.x==0 && coords.y==N-1 && coords.z==0)
	  velocity0[index][c]       = 0.33f * (velocity0[getIdx(uvec3(1,N-1,0), N)][c]
                                  +       velocity0[getIdx(uvec3(0,N-2,0), N)][c]
                                  +        velocity0[getIdx(uvec3(0,N-1,1), N)][c]								  
								  );	

//3							  
	if(coords.x==0 && coords.y==0 && coords.z==N-1)
	  velocity0[index][c]       = 0.33f * (velocity0[getIdx(uvec3(1,0,N-1), N)][c]
                                  + velocity0[getIdx(uvec3(0,1,N-1), N)][c]
                                  + velocity0[getIdx(uvec3(0,0,N), N)][c]								  
								  );
		//4						  
	if(coords.x==0 && coords.y==N-1 && coords.z==N-1)
	  velocity0[index][c]       = 0.33f * (velocity0[getIdx(uvec3(1,N-1,N-1), N)][c]
                                  + velocity0[getIdx(uvec3(0,N-2,N-1), N)][c]
                                  + velocity0[getIdx(uvec3(0,N-1,N-2), N)][c]								  
								  );	

	//5						  
	if(coords.x==N-1 && coords.y==0 && coords.z==0)
	  velocity0[index][c]       = 0.33f * (velocity0[getIdx(uvec3(N-2,0,0), N)][c]
                                  + velocity0[getIdx(uvec3(N-1,1,0), N)][c]
                                  + velocity0[getIdx(uvec3(N-1,0,1), N)][c]								  
								  );
		//6						  
	if(coords.x==N-1 && coords.y==N-1 && coords.z==0)
	  velocity0[index][c]       = 0.33f * (velocity0[getIdx(uvec3(N-2,N-1,0), N)][c]
                                  + velocity0[getIdx(uvec3(N-1,N-2,0), N)][c]
                                  + velocity0[getIdx(uvec3(N-1,N-1,1), N)][c]								  
								  );
		//7						  
 	if(coords.x==N-1 && coords.y==0 && coords.z==N-1)
	  velocity0[index][c]       = 0.33f * (velocity0[getIdx(uvec3(N-2,0,N-1), N)][c]
                                  + velocity0[getIdx(uvec3(N-1,1,N-1), N)][c]
                                  + velocity0[getIdx(uvec3(N-1,0,N-2), N)][c]								  
								  );
		//8						  
	if(coords.x==N-1 && coords.y==N-1 && coords.z==N-1)
	  velocity0[index][c]       = 0.33f * (velocity0[getIdx(uvec3(N-2,N-1,N-1), N)][c]
                                  + velocity0[getIdx(uvec3(N-1,N-2,N-1), N)][c]
                                  + velocity0[getIdx(uvec3(N-1,N-1,N-2), N)][c]								  
								  );								  
}

void setBNDVel(int b, int c, uint index, uint N)
{

	uvec3 coords=getCoords(index, N);
	uint i=coords.x; uint j=coords.y; uint k=coords.z;
	
	if(coords.z==0)
	{
	   if(b==2)
	    velocity[index][c]=-velocity[getIdx(uvec3(i,j,1), N)][c];
     else
	    velocity[index][c]= velocity[getIdx(uvec3(i,j,1), N)][c];	
	}
	
	if(coords.z==N-1)
	{
	   if(b==2)
	    velocity[index][c]=-velocity[getIdx(uvec3(i,j,N-2), N)][c];
     else
	    velocity[index][c]= velocity[getIdx(uvec3(i,j,N-2), N)][c];	
	}
	//
	if(j==0)
	{
	   if(b==1)
	    velocity[index][c]=-velocity[getIdx(uvec3(i,1,k), N)][c];
     else
	    velocity[index][c]= velocity[getIdx(uvec3(i,1,k), N)][c];	
	}
	
	if(j==N-1)
	{
	   if(b==1)
	    velocity[index][c]=-velocity[getIdx(uvec3(i,N-2,k), N)][c];
     else
	    velocity[index][c]= velocity[getIdx(uvec3(i,N-2,k), N)][c];	
	}
	//
		if(i==0)
	{
	   if(b==0)
	    velocity[index][c]=-velocity[getIdx(uvec3(1,j,k), N)][c];
     else
	    velocity[index][c]= velocity[getIdx(uvec3(1,j,k), N)][c];	
	}
	
	if(i==N-1)
	{
	   if(b==0)
	    velocity[index][c]=-velocity[getIdx(uvec3(N-2,j,k), N)][c];
     else
	    velocity[index][c]= velocity[getIdx(uvec3(N-2,j,k), N)][c];	
	}

	if(coords.x==0 && coords.y==0 && coords.z==0)
	  velocity[index][c]       = 0.33f * (velocity[getIdx(uvec3(1,0,0), N)][c]
                                  + velocity[getIdx(uvec3(0,1,0), N)][c]
                                  + velocity[getIdx(uvec3(0,0,1), N)][c]								  
								  );
	//2							  
	if(coords.x==0 && coords.y==N-1 && coords.z==0)
	  velocity[index][c]       = 0.33f * (velocity[getIdx(uvec3(1,N-1,0), N)][c]
                                  +       velocity[getIdx(uvec3(0,N-2,0), N)][c]
                                  +        velocity[getIdx(uvec3(0,N-1,1), N)][c]								  
								  );	

//3							  
	if(coords.x==0 && coords.y==0 && coords.z==N-1)
	  velocity[index][c]       = 0.33f * (velocity[getIdx(uvec3(1,0,N-1), N)][c]
                                  + velocity[getIdx(uvec3(0,1,N-1), N)][c]
                                  + velocity[getIdx(uvec3(0,0,N), N)][c]								  
								  );
		//4						  
	if(coords.x==0 && coords.y==N-1 && coords.z==N-1)
	  velocity[index][c]       = 0.33f * (velocity[getIdx(uvec3(1,N-1,N-1), N)][c]
                                  + velocity[getIdx(uvec3(0,N-2,N-1), N)][c]
                                  + velocity[getIdx(uvec3(0,N-1,N-2), N)][c]								  
								  );	

	//5						  
	if(coords.x==N-1 && coords.y==0 && coords.z==0)
	  velocity[index][c]       = 0.33f * (velocity[getIdx(uvec3(N-2,0,0), N)][c]
                                  + velocity[getIdx(uvec3(N-1,1,0), N)][c]
                                  + velocity[getIdx(uvec3(N-1,0,1), N)][c]								  
								  );
		//6						  
	if(coords.x==N-1 && coords.y==N-1 && coords.z==0)
	  velocity[index][c]       = 0.33f * (velocity[getIdx(uvec3(N-2,N-1,0), N)][c]
                                  + velocity[getIdx(uvec3(N-1,N-2,0), N)][c]
                                  + velocity[getIdx(uvec3(N-1,N-1,1), N)][c]								  
								  );
		//7						  
 	if(coords.x==N-1 && coords.y==0 && coords.z==N-1)
	  velocity[index][c]       = 0.33f * (velocity[getIdx(uvec3(N-2,0,N-1), N)][c]
                                  + velocity[getIdx(uvec3(N-1,1,N-1), N)][c]
                                  + velocity[getIdx(uvec3(N-1,0,N-2), N)][c]								  
								  );
		//8						  
	if(coords.x==N-1 && coords.y==N-1 && coords.z==N-1)
	  velocity[index][c]       = 0.33f * (velocity[getIdx(uvec3(N-2,N-1,N-1), N)][c]
                                  + velocity[getIdx(uvec3(N-1,N-2,N-1), N)][c]
                                  + velocity[getIdx(uvec3(N-1,N-1,N-2), N)][c]								  
								  );								  
}


// d - velCurrent, d0 -velCur0,  
void advect(int c, uint index,  float dt, uint N)
{
    float i0, i1, j0, j1, k0, k1;
    
    float dtx = dt * (N - 2);
    float dty = dt * (N - 2);
    float dtz = dt * (N - 2);
    
    float s0, s1, t0, t1, u0, u1;
    float tmp1, tmp2, tmp3, x, y, z;
    
    float Nfloat = float(N);
    float ifloat, jfloat, kfloat;

	uvec3 coords=getCoords(index, N);
	ifloat=float(coords.x);
	jfloat=float(coords.y);
	kfloat=float(coords.z);

                tmp1 = dtx * velocity0[index].x;
                tmp2 = dty * velocity0[index].y;
                tmp3 = dtz * velocity0[index].z;
                x    = ifloat - tmp1; 
                y    = jfloat - tmp2;
                z    = kfloat - tmp3;
                
                if(x < 0.5f) x = 0.5f; 
                if(x > Nfloat + 0.5f) x = Nfloat + 0.5f; 
                i0 = floor(x); 
                i1 = i0 + 1.0f;
                if(y < 0.5f) y = 0.5f; 
                if(y > Nfloat + 0.5f) y = Nfloat + 0.5f; 
                j0 = floor(y);
                j1 = j0 + 1.0f; 
                if(z < 0.5f) z = 0.5f;
                if(z > Nfloat + 0.5f) z = Nfloat + 0.5f;
                k0 = floor(z);
                k1 = k0 + 1.0f;
                
                s1 = x - i0; 
                s0 = 1.0f - s1; 
                t1 = y - j0; 
                t0 = 1.0f - t1;
                u1 = z - k0;
                u0 = 1.0f - u1;
                
                int i0i = int(i0);
                int i1i = int(i1);
                int j0i = int(j0);
                int j1i = int(j1);
                int k0i = int(k0);
                int k1i = int(k1);
                
				
				vec3 d0N1=velocity0[getIdx(uvec3(i0i, j0i, k0i), N)].xyz;
				vec3 d0N2=velocity0[getIdx(uvec3(i0i, j0i, k1i), N)].xyz;
				vec3 d0N3=velocity0[getIdx(uvec3(i0i, j1i, k0i), N)].xyz;
				vec3 d0N4=velocity0[getIdx(uvec3(i0i, j1i, k1i), N)].xyz;
				vec3 d0N5=velocity0[getIdx(uvec3(i1i, j0i, k0i), N)].xyz;
				vec3 d0N6=velocity0[getIdx(uvec3(i1i, j0i, k1i), N)].xyz;
				vec3 d0N7=velocity0[getIdx(uvec3(i1i, j1i, k0i), N)].xyz;
				vec3 d0N8=velocity0[getIdx(uvec3(i1i, j1i, k1i), N)].xyz;
                	velocity[index][c]=        
                    s0 * ( t0 * (u0 * d0N1[c]+u1 * d0N2[c])  +( t1 * (u0 * d0N3[c]+u1 * d0N4[c])))
                   +s1 * ( t0 * (u0 * d0N5[c]+u1 * d0N6[c])+( t1 * (u0 * d0N7[c]+u1 * d0N8[c])));
								
setBNDVel(c,c,index,N);
}
void diffuse (int component, uint index,  float diff, float dt, int iter, uint N)
{
	uvec3 coords=getCoords(index, N);
	uint i=coords.x; uint j=coords.y; uint m= coords.z;
	vec3 d0N1=velocity0[getIdx(uvec3(i+1, j, m), N)].xyz;
	vec3 d0N2=velocity0[getIdx(uvec3(i-1, j, m), N)].xyz;
	vec3 d0N3=velocity0[getIdx(uvec3(i, j+1, m), N)].xyz;
	vec3 d0N4=velocity0[getIdx(uvec3(i, j-1, m), N)].xyz;
	vec3 d0N5=velocity0[getIdx(uvec3(i, j, m+1), N)].xyz;
	vec3 d0N6=velocity0[getIdx(uvec3(i, j, m-1), N)].xyz;
    float a = dt * diff * (N - 2) * (N - 2);
	float c=1 + 6 * a;
	 float cRecip = 1.0 / c;
	 
	  for (int k = 0; k < iter; k++) 
	  {
	   velocity0[index][component] =(    velocity[index][component] + a*(    d0N1[component]
	   + d0N2[component]
	   + d0N3[component]
	   +d0N4[component]
	   + d0N5[component]
	   +d0N6[component])) * cRecip;
	  }
	  setBNDVelStart(component,index,N);
}

void project(uint index,int iter, uint N)
{
uvec3 coords=getCoords(index, N);
	uint i=coords.x; uint j=coords.y; uint m= coords.z;
	vec3 d0N1=velocity0[getIdx(uvec3(i+1, j, m), N)].xyz;
	vec3 d0N2=velocity0[getIdx(uvec3(i-1, j, m), N)].xyz;
	vec3 d0N3=velocity0[getIdx(uvec3(i, j+1, m), N)].xyz;
	vec3 d0N4=velocity0[getIdx(uvec3(i, j-1, m), N)].xyz;
	vec3 d0N5=velocity0[getIdx(uvec3(i, j, m+1), N)].xyz;
	vec3 d0N6=velocity0[getIdx(uvec3(i, j, m-1), N)].xyz;
  
               velocity[index].y = -0.5f*(
                        d0N1.x
                        -d0N2.x
                        +d0N3.y
                        -d0N4.y
                        +d0N5.z
                        -d0N6.z
                    )/N;
                velocity[index].x = 0;
    
setBNDVel(0,1,index,N);
//setBNDVel(0,0,index,N);
float a=1; float c=6;
  float cRecip = 1.0 / c;
    for (int k = 0; k < iter; k++) 
	{
	 velocity[index].x =(   velocity[index].y + a*
	                              (  
								  velocity[getIdx(uvec3(i+1, j, m), N)].x
								  +velocity[getIdx(uvec3(i-1, j, m), N)].x
								  +velocity[getIdx(uvec3(i, j+1, m), N)].x
								  +velocity[getIdx(uvec3(i, j-1, m), N)].x
								  +velocity[getIdx(uvec3(i, j, m+1), N)].x
								  +velocity[getIdx(uvec3(i, j, m-1), N)].x
								  )
	 
	                    ) * cRecip;
	}
   
    
  
             velocity0[index].x -= 0.5f * (   velocity[getIdx(uvec3(i+1, j, m), N)].x -velocity[getIdx(uvec3(i-1, j, m), N)].x) * N;
             velocity0[index].y -= 0.5f * ( velocity[getIdx(uvec3(i, j+1, m), N)].x-velocity[getIdx(uvec3(i, j-1, m), N)].x) * N;
             velocity0[index].z -= 0.5f * (  velocity[getIdx(uvec3(i, j, m+1), N)].x-velocity[getIdx(uvec3(i, j, m-1), N)].x) * N;
         
setBNDVelStart(0, index, N);
setBNDVelStart(1, index, N);
setBNDVelStart(2, index, N);
}
void main() 
{
   
    uint idx = gl_GlobalInvocationID.x;
	

  uint n=16;
	
	for(int i=0; i<3; i++)
    {
   diffuse(i,idx,2, deltaT, 4, n);
    }
     project(idx,4, n);
    for(int i=0; i<3; i++)
   {
     advect(i,idx,  deltaT, n);
    }
	  vec3 p   = position [idx].xyz;
	vec3 v   = velocity [idx].xyz;
	vec3 v0=velocity0[idx].xyz;
	
   velocity0 [idx]= vec4(v0,1.0);
   velocity [idx]= vec4(v,1.0);
    position [idx] =vec4(v, 1.0);
}