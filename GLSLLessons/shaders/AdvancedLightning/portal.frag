#version 330 core
in VS_OUT {
    vec4 FragPos;
	vec4 PosRaw;
	vec4 FragPosP;
    vec3 Normal;
    vec2 TexCoords;
     vec4 FragPosLightSpace;
} fs_in;

uniform sampler2D diffuseTexture;
uniform sampler2D shadowMap;


uniform vec3 lightPos;

uniform vec3 viewPos;

float f=100.0;
float n=0.1;

float _Shininess=0.5;
vec3 _LightColor0=vec3(1);
vec3 _SpecColor=vec3(0.5);
float linearizeDepth(float depth, float near_plane, float far_plane )
{
    float z = depth * 2.0 - 1.0; // Back to NDC 
    return (2.0 * near_plane * far_plane) / (far_plane + near_plane - z * (far_plane - near_plane));	
}


float linearEyeDepth(float depth, vec4 zBufferParam)
{
    return 1.0 / (zBufferParam.z * depth + zBufferParam.w);
}

float LinearDepthToNonLinear(float linear01Depth, vec4 zBufferParam){
	// Inverse of Linear01Depth
	return (1.0 - (linear01Depth * zBufferParam.y)) / (linear01Depth * zBufferParam.x);
}

float EyeDepthToNonLinear(float eyeDepth, vec4 zBufferParam){
	// Inverse of LinearEyeDepth
	return (1.0 - (eyeDepth * zBufferParam.w)) / (eyeDepth * zBufferParam.z);
}

vec3 fresnelSchlick(float cosTheta, vec3 F0)
{
    return F0 + (1.0 - F0) * pow(1.0 - cosTheta, 5.0);
}
float fresnelEffect(float cosTheta, float F0)
{
    return F0 + (1.0 - F0) * pow(1.0 - cosTheta, 8.0);
}

vec3 frEff(vec3 normalDirection,vec3 lightDirection, vec3 viewDirection)
{
  vec3 specularReflection;
            if (dot(normalDirection, lightDirection) < 0.0) 
            {
               specularReflection = vec3(0.0, 0.0, 0.0); 
            }
            else 
            {
               vec3 halfwayDirection = normalize(lightDirection + viewDirection);
               float w = pow(1.0 - max(0.0, dot(halfwayDirection, viewDirection)), 5.0);
			   
               specularReflection =  _LightColor0  * mix(_SpecColor, vec3(1.0), w) 
			   * pow(max(0.0, dot(reflect(-lightDirection, normalDirection), viewDirection)), _Shininess);
            }
			
			return specularReflection;
}

void main()
{

    vec4 fragPosLightSpace=fs_in.FragPosLightSpace;
    vec3 projCoords = fragPosLightSpace.xyz/ fragPosLightSpace.w; 
    vec3 normal = normalize(fs_in.Normal);
	
	if(gl_FrontFacing)
	normal=-normal;
    vec3 lightDir = normalize(lightPos - fs_in.FragPos.xyz);
    vec3 viewDir = normalize(viewPos - fs_in.FragPos.xyz);

   vec3 fr2=frEff(normal, lightDir, viewDir);
   
vec4 zBuffer=vec4(1.0-f/n, f/n, (1.0-f/n)/f, (f/n)/f);
   vec4 pos=fs_in.FragPosP;
   float coord=texture(shadowMap, gl_FragCoord.xy/vec2(1920,1280)).r;
  // coord=linearizeDepth(coord,  0.1, 100.0)/100.0;
  coord=linearEyeDepth(coord, zBuffer);
  float iOffset=0.5;
  float iMultiplier=1;
  float ss=(-pos.z)-iOffset;
 
  float ss2=1 - clamp((coord - ss), 0.0, 1.0);
  
  
	vec4 emission=vec4(vec3( ss2),1);
	
	vec4 color=vec4(0.43,0.32, 0.8,1);
	//if(coord<1.0)
	//color.r=1;
	//abs(pos.z)
float fragmentEyeDepth = -pos.z;
float depthDifferenceExample = 1 - clamp((coord - fragmentEyeDepth), 0.0, 1.0);

vec3 F0 = vec3(0.01);
//F0 = mix(F0, color.rgb, 0.4);
vec3 fr1=fresnelSchlick(dot(normal, viewDir),F0)*2.0;
float  fr= pow(  1-clamp(dot(normal, viewDir), 0.0, 1.0), 5.0);


float sum=fr+ss2;



gl_FragColor=color*sum;

}