#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoords;

out VS_OUT {
    vec4 FragPos;
	vec4 PosRaw;
	vec4 FragPosP;
    vec3 Normal;
    vec2 TexCoords;
	vec4 FragPosLightSpace;
} vs_out;

uniform mat4 projection;
uniform mat4 view;
uniform mat4 model;
uniform mat4 lightSpaceMatrix;
uniform mat4 cameraModel;

void main()
{
    vs_out.FragPos = vec4(model * vec4(aPos, 1.0));
    vs_out.Normal = transpose(inverse(mat3(model))) * aNormal;
	// vs_out.Normal=(model * vec4(aNormal, 1.0)).xyz;
    vs_out.TexCoords = aTexCoords;
	 vs_out.FragPosLightSpace = lightSpaceMatrix * vec4(vs_out.FragPos.xyz, 1.0);
	 vs_out.PosRaw=vec4(aPos, 1.0);
    gl_Position = projection * view * model * vec4(aPos, 1.0);
	vs_out.FragPosP=view * model * vec4(aPos, 1.0);
}