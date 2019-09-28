#version 440

struct Material
{
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
	sampler2D diffuseTex;
	sampler2D specularTex;
};

in vec3 vs_position;
in vec3 vs_color;
in vec2 vs_texcoord;
in vec3 vs_normal;
in vec4 FragPosLightSpace;

out vec4 fs_color;

//Uniforms
uniform Material material;
uniform sampler2D texture0;
uniform sampler2D texture1;
uniform sampler2D ShadowTex;

uniform vec3 lightPos0;
uniform vec3 lightColor;
uniform vec3 cameraPos;

vec3 calculateAmbient(Material material)
{
	return material.ambient;
}

vec3 calculateDiffuse(Material material, vec3 vs_position, vec3 vs_normal ,vec3 lightPos0)
{
	vec3 posToLightDirVec = normalize(lightPos0 - vs_position);
	float diffuse = clamp(dot(posToLightDirVec, vs_normal), 0, 1);
	vec3 diffuseFinal = material.diffuse * diffuse;

	return diffuseFinal;
}

vec3 calculateSpecular(Material material, vec3 vs_position, vec3 vs_normal, vec3 lightPos0, vec3 cameraPos)
{
	vec3 lightToPosDirVec = normalize(vs_position - lightPos0);
	vec3 reflectDirVec = normalize(reflect(lightToPosDirVec, normalize(vs_normal)));
	vec3 posToViewDirVec = normalize(cameraPos - vs_position);
	float specularConstant = pow(max(dot(posToViewDirVec, reflectDirVec), 0), 35);
	vec3 specularFinal = material.specular * specularConstant * texture(material.specularTex, vs_texcoord).rgb;

	return specularFinal;
}

float near = 0.1; 
float far  = 100.0; 
  
float LinearizeDepth(float depth) 
{
    float z = depth * 2.0 - 1.0; // back to NDC 
    return (2.0 * near * far) / (far + near - z * (far - near));	
}

//Shadow Calculation
float ShadowCalculation(vec4 fragPosLightSpace)
{
	vec3 projCoords = fragPosLightSpace.xyz / fragPosLightSpace.w;
	projCoords = projCoords * 0.5 + 0.5;
	float closesetDepth = texture(ShadowTex,projCoords.xy).r;
	float currentDepth = projCoords.z;
	float bias = 0.005;
	float shadow = (currentDepth - bias) > closesetDepth ? 1.0 : 0.0;
	if (projCoords.z > 1.0)
		shadow = 0.0;
	return shadow;
}

void main()
{
	//fs_color = vec4(lightColor * vs_color , 1.f);
	//fs_color = texture(texture0, vs_texcoord) * texture(texture1, vs_texcoord);

	//Ambient Light

	vec3 ambientFinal = calculateAmbient(material);

	//Diffuse light

	vec3 diffuseFinal = calculateDiffuse(material, vs_position, vs_normal, lightPos0);

	//Specular Light

	vec3 specularFinal = calculateSpecular(material, vs_position, vs_normal, lightPos0, cameraPos);

	//Attenuation
	float shadow = ShadowCalculation(FragPosLightSpace);
	//Final Color
	vec3 color = texture(material.diffuseTex, vs_texcoord).rgb;
	vec3 Lighting = ((1.0-shadow) * (diffuseFinal + specularFinal) + ambientFinal) * color;
	fs_color = vec4(Lighting,1.0);
}