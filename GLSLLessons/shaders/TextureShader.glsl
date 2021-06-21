#version 430 
in vec3 Position;
in vec3 Normal;
in vec2 UV;

uniform sampler2D Main;

float f=100.0;
float n=0.1;
float linearizeDepth(float depth, float near_plane, float far_plane )
{
    float z = depth * 2.0 - 1.0; // Back to NDC 
    return (2.0 * near_plane * far_plane) / (far_plane + near_plane - z * (far_plane - near_plane));	
}

float linearEyeDepth(float depth)
{
    return 1.0/( ((f-n)/(n*f))*depth  + 1.0/f);
}
void main()
{

//inversion
 gl_FragColor = vec4(vec3(texture(Main, UV)), 1);
 gl_FragColor = texture(Main, UV);
//deth testing
 //gl_FragColor = vec4(vec3(texture(Main, UV).r),1.0);
 //gl_FragColor = vec4(vec3(linearEyeDepth((texture(Main, UV)).r)),1.0);
   //gl_FragColor=vec4(0.1,0.1,0.1,1.0);
};


