#version 440

struct Material
{
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
	sampler2D diffuseTex;
	sampler2D specularTex;
};
struct
CnLightInfo{
	vec3 Ambient;
	vec3 Diffuse;
	vec3 Specular;
	vec3 LightPos;
};

struct DirLightsInfo
{
	vec3 Ambient;
	vec3 Diffuse;
	vec3 Specular;
	vec3 LightPos;
	vec3 LightColor;
	mat4 LightMatrix;
	sampler2D LightShadow; 
};

#define MAX_LIGHTS 45

in vec3 vs_position;
in vec3 vs_color;
in vec2 vs_texcoord;
in vec3 vs_normal;
in float visibility;

out vec4 fs_color;

//Uniforms
uniform Material material;
uniform DirLightsInfo AllDirInfo[MAX_LIGHTS];
uniform CnLightInfo AllCnInfo[MAX_LIGHTS];
uniform vec3 cameraPos;
uniform vec3 SkyColor;

uniform int DirLightCount;
uniform int CnLightCount;
//Functions
vec3 calculateAmbient(Material material)
{
	return material.ambient;
}

vec3 calculateDiffuse(Material material, vec3 vs_position, vec3 vs_normal, vec3 lightPos)
{
	vec3 norm = normalize(vs_normal);
	vec3 posToLightDirVec = normalize(lightPos - vs_position);
	float diffuse = max(dot(norm , posToLightDirVec), 0.0);
	vec3 diffuseFinal = material.diffuse * diffuse;

	return diffuseFinal;
}

vec3 calculateSpecular(Material material, vec3 vs_position, vec3 vs_normal, vec3 lightPos, vec3 cameraPos)
{ 
	vec3 lightToPosDirVec = normalize(vs_position - lightPos);
	vec3 reflectDirVec = normalize(reflect(lightToPosDirVec, normalize(vs_normal)));
	vec3 posToViewDirVec = normalize(cameraPos - vs_position);
	float specularConstant = pow(max(dot(posToViewDirVec, reflectDirVec), 0), 35);
	vec3 specularFinal = material.specular * specularConstant * texture(material.specularTex, vs_texcoord).rgb;

	return specularFinal;
}
float ShadowCalculation(DirLightsInfo LightToUse,vec3 Normal)
{
	vec4 FragPosLightSpace = LightToUse.LightMatrix * vec4(vs_position,1.f);
	float shadow = 0.f;
	float bias = max(0.05 * (1.0 - dot(Normal,LightToUse.LightPos)),0.005);
	vec3 projCoords = FragPosLightSpace.xyz / FragPosLightSpace.w;
	projCoords = projCoords * 0.5 + 0.5;
	float closesetDepth = texture(LightToUse.LightShadow,projCoords.xy).r;
	float currentDepth = projCoords.z;

		vec2 TexeSize = 1.0 / textureSize(LightToUse.LightShadow,0);
		for(int x = -1; x <=1;++x)
		{
			for(int y = -1; y <=1;++y)
			{
				float pcfDepth = texture(LightToUse.LightShadow,projCoords.xy + vec2(x,y) * TexeSize).r;
				shadow += currentDepth - bias > pcfDepth ? 1.0 : 0.0;
			}
		}
		shadow /=9.0;
		
	if (projCoords.z > 1.0)
	{
		shadow = 0.0;
		return shadow;
	}
	return shadow;
}
void main()
{
	vec4 result = vec4(0.f);
	for(int ii = 0; ii < DirLightCount ; ii++)
	{
	//Ambient light
		vec3 ambientFinal = AllDirInfo[ii].Ambient * calculateAmbient(material);

	//Diffuse light
		vec3 diffuseFinal = AllDirInfo[ii].Diffuse * calculateDiffuse(material, vs_position, vs_normal,AllDirInfo[ii].LightPos);

	//Specular light
		vec3 specularFinal = AllDirInfo[ii].Specular * calculateSpecular(material, vs_position, vs_normal, AllDirInfo[ii].LightPos, cameraPos);

	//Attenuation
		float shadow = ShadowCalculation(AllDirInfo[ii],vs_normal);
	//Final light
		result += vec4(ambientFinal, 1.f) + (1.0 - shadow) * (vec4(diffuseFinal, 1.f) + vec4(specularFinal, 1.f));
	}
	
 
	fs_color = texture(material.diffuseTex, vs_texcoord);
	fs_color = (result) * fs_color;
	fs_color = mix(vec4(SkyColor, 1.0),fs_color, visibility);
}
