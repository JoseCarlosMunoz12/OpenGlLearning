#version 440

struct Material
{
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
	sampler2D diffuseTex;
	sampler2D specularTex;
};
struct CnLightInfo{
	vec3 Ambient;
	vec3 Diffuse;
	vec3 Specular;
	vec3 LightPos;
	vec3 LightDirection;
	float ConeAngle;
	mat4 LightMatrix;
	sampler2D LightShadow; 
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
 struct AreaLightInfo
 {
	vec3 Ambient;
	vec3 Diffuse;
	vec3 Specular;
	vec3 LightPos;
	vec3 LightColor;
	mat4 LightMatrix;
	vec3 LightDirection;
	sampler2D LightShadow; 	
	float ConeAngle;
	float UmbraAngle;
	float Linear;
	float Constant;
	float Quadratic;
 };

#define MAX_LIGHTS 10

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
uniform AreaLightInfo AllArInfo[MAX_LIGHTS];
uniform int ArLightCount;

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
float ShadowCalculation(sampler2D LightShadow,vec3 Normal,mat4 LightMatrix,vec3 LightPos,bool IsAr)
{
	vec4 FragPosLightSpace = LightMatrix * vec4(vs_position,1.f);
	float shadow = 0.f;
	vec3 LightDir = normalize( vs_position-LightPos);
	float bias = max(0.05 * (1.0 - dot(normalize(Normal),LightDir)),0.005);
	
	vec3 projCoords = FragPosLightSpace.xyz / FragPosLightSpace.w;
	projCoords = projCoords * 0.5 + 0.5;
	float closesetDepth = texture(LightShadow,projCoords.xy).r;
	float currentDepth = projCoords.z;

	vec2 TexeSize = 1.0 / textureSize(LightShadow,0);
	
	for(int x = -1; x <=1;++x)
		{
			for(int y = -1; y <=1;++y)
			{
				float pcfDepth = texture(LightShadow,projCoords.xy + vec2(x,y) * TexeSize).r;
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
 vec4 result = vec4(1.f);
	for(int ii =0; ii < ArLightCount; ii++)
	{ 
		vec3 FinalAmbient = calculateAmbient(material) * AllArInfo[ii].Ambient;
		vec3 FinalDiffuse = material.diffuse * AllArInfo[ii].Diffuse;
		vec3 FinalSpecular = AllArInfo[ii].Specular * calculateSpecular(material,vs_position,vs_normal,
															AllArInfo[ii].LightPos,cameraPos);
		float shadow = ShadowCalculation(AllArInfo[ii].LightShadow,
										vs_normal,AllArInfo[ii].LightMatrix,
										AllArInfo[ii].LightPos,true);
		vec3 LightDir = normalize(AllArInfo[ii].LightPos - vs_position);
		float Theta = dot(LightDir,normalize(-AllArInfo[ii].LightDirection));
		float Epsilon = (AllArInfo[ii].ConeAngle - AllArInfo[ii].UmbraAngle);
		float Intensity = clamp((Theta -AllArInfo[ii].UmbraAngle) / Epsilon, 0.0, 1.0);

		float Dist = length(AllArInfo[ii].LightPos- vs_position);
		float Attenuation = 1.0 / (AllArInfo[ii].Constant + AllArInfo[ii].Linear * Dist + AllArInfo[ii].Quadratic * (Dist * Dist));
		result += Attenuation * (vec4(FinalAmbient,1.f) + Intensity * (1.0 - shadow) * ( vec4(FinalDiffuse,1.f)+ vec4(FinalSpecular,1.f)));
	}
	for(int ii = 0; ii < CnLightCount; ii++)
	{
		vec3 LightDir = normalize(AllCnInfo[ii].LightPos - vs_position);
		float Theta = dot(LightDir , normalize(-1 * AllCnInfo[ii].LightDirection));
		if(Theta > AllCnInfo[ii].ConeAngle)
		{
			vec3 FinalAmbient = AllCnInfo[ii].Ambient * calculateAmbient(material);
			vec3 FinalDiffuse = AllCnInfo[ii].Diffuse * material.diffuse;
			vec3 FinalSpecular = AllCnInfo[ii].Specular * calculateSpecular(material,vs_position,vs_normal,AllCnInfo[ii].LightPos,cameraPos);
			float shadow = ShadowCalculation(AllCnInfo[ii].LightShadow,vs_normal,AllCnInfo[ii].LightMatrix,AllCnInfo[ii].LightPos,false);
			result += vec4(FinalAmbient,1.f) + (1.f - shadow)* (vec4(FinalDiffuse,1.f) + vec4(FinalSpecular,1.f)); 
		}else{
			result += vec4(AllCnInfo[ii].Ambient * calculateAmbient(material),1.f);
		}
	}

	for(int ii = 0; ii < DirLightCount ; ii++)
	{
		vec3 ambientFinal = AllDirInfo[ii].Ambient * calculateAmbient(material);
		vec3 diffuseFinal = AllDirInfo[ii].Diffuse * calculateDiffuse(material, vs_position, vs_normal,AllDirInfo[ii].LightPos);
		vec3 specularFinal = AllDirInfo[ii].Specular * calculateSpecular(material, vs_position, vs_normal, AllDirInfo[ii].LightPos, cameraPos);
		float shadow = ShadowCalculation(AllDirInfo[ii].LightShadow,vs_normal,AllDirInfo[ii].LightMatrix,AllDirInfo[ii].LightPos,false);
		result += vec4(ambientFinal, 1.f) + (1.0 - shadow) * (vec4(diffuseFinal, 1.f) + vec4(specularFinal, 1.f));
	}
	
 
	fs_color = texture(material.diffuseTex, vs_texcoord);
	fs_color = (result) * fs_color;
	fs_color = mix(vec4(SkyColor, 1.0),fs_color, visibility);
	
}
