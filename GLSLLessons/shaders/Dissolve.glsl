#version 430 
in vec3 Position;
in vec3 Normal;
in vec2 UV;


uniform vec3 lightPos;
uniform vec3 lightColor;
uniform vec3 viewPos;
uniform vec3 lightDistance;

uniform sampler2D  MainTex;
uniform sampler2D  DissolveTex;
uniform float Time;
void main()
{
vec4 dissolveColor=texture(DissolveTex, UV);
int even=int(mod(int(UV.x*100),2));
vec3 dd=normalize( Position);
float avr=(dd.x+dd.y+dd.z)/3.0;
float dissolve=step(dissolveColor.x,clamp(0, 1,sin(Time/10)));
float dissolveDD=(smoothstep(dissolveColor.x-0.03, dissolveColor.x,clamp(0, 1,sin(Time/10)))
-smoothstep( dissolveColor.x,dissolveColor.x+0.03,clamp(0, 1,sin(Time/10))));

vec3 emissionColor=vec3(0,0.5,1.0)*dissolveDD;

vec4 colorTex=texture(MainTex, UV);
//ambient
float ambientStrength=0.1;
vec3 ambient=ambientStrength*lightColor;



//diffuse
vec3 normal=normalize(Normal);
vec3 dir=normalize(lightPos-Position);
vec3 difuse =max(dot( Normal, dir), 0.0)*lightColor;


//specular
float specularStrength = 0.5;
vec3 viewDir = normalize(viewPos - Position);
vec3 reflectDir = reflect(-dir, normal); 
float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);
vec3 specular = specularStrength * spec * lightColor;  

vec3 color=vec3(1,1,1);
color=(ambient+difuse+specular)*color;
   gl_FragColor = vec4(colorTex.xyz,dissolve);
};