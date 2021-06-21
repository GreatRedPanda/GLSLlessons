#version 430 
in vec3 Position;
in vec3 Normal;
in vec2 UV;

struct Material {
    sampler2D diffuse;
    sampler2D specular;
    float shininess;
}; 
  
  struct Light
  { 
   vec3 position;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
   
  };
uniform Material material;
uniform Light light;
uniform vec3 viewPos;  




void main()
{
vec2 circleCoords=vec2( 0, 0);
float cc=length(circleCoords-Position.xy);
float radius =0.3;
float Alpha=step(radius, cc);




vec3 emissionColor=vec3(0.0, 0.3, 0.8);

//ambient
vec3 ambient=light.ambient*texture(material.diffuse, UV).xyz;


//diffuse
vec3 normal=normalize(Normal);
vec3 dir=normalize(light.position-Position);
float diff=max(dot( normal, dir), 0.0);
vec3 diffuse =light.diffuse*diff*texture(material.diffuse, UV).xyz;


//specular
vec3 viewDir = normalize(viewPos - Position);
vec3 reflectDir = reflect(-dir, normal); 
float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
vec3 specular = texture(material.specular, UV).xyz * spec * light.specular;  

//total
vec3 color=(ambient+diffuse+specular)+step(0.6,texture(material.specular, UV).x)*emissionColor;
 gl_FragColor = vec4(color,Alpha);
};