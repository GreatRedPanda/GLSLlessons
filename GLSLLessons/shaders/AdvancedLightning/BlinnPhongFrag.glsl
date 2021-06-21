#version 330 
in vec3 FragPosition;
in vec3 Normal;
in vec2 UV;
in vec4 FragPosLightSpace;



struct LightSource
{
   vec3 Position;
   vec3 Color;

};


uniform sampler2D diffuseT;
uniform sampler2D shadowMap;
uniform vec3 viewPos;
uniform LightSource light;

float ShadowCalculation(vec4 fragPosLightSpace, float bias)
{
    // perform perspective divide
    vec3 projCoords = fragPosLightSpace.xyz / fragPosLightSpace.w;
    // transform to [0,1] range
    projCoords = projCoords * 0.5 + 0.5;
    // get closest depth value from light's perspective (using [0,1] range fragPosLight as coords)
    float closestDepth = texture(shadowMap, projCoords.xy).r; 
    // get depth of current fragment from light's perspective
    float currentDepth = projCoords.z;
    // check whether current frag pos is in shadow
    float shadow = currentDepth-bias > closestDepth  ? 1.0 : 0.0;

    return shadow;
}



void main()
{
vec3 testingMap = texture(shadowMap, UV).rgb;
vec3 color = texture(diffuseT, UV).rgb;
    vec3 normal = normalize(Normal);
    vec3 lightColor = vec3(1.0);
    // ambient
    vec3 ambient = 0.15 * color;
    // diffuse
    vec3 lightDir = normalize(light.Position - FragPosition);
    float diff = max(dot(lightDir, normal), 0.0);
    vec3 diffuse = diff * lightColor;
    // specular
    vec3 viewDir = normalize(viewPos - FragPosition);
    float spec = 0.0;
    vec3 halfwayDir = normalize(lightDir + viewDir);  
    spec = pow(max(dot(normal, halfwayDir), 0.0), 64.0);
    vec3 specular = spec * lightColor;    
    // calculate shadow
	float bias = max(0.05 * (1.0 - dot(normal, lightDir)), 0.005);  
    float shadow = ShadowCalculation(FragPosLightSpace, bias);       
    vec3 lighting = (ambient + (1.0 - shadow) * (diffuse + specular)) * color;    
    
gl_FragColor=vec4(lighting, 1);
}