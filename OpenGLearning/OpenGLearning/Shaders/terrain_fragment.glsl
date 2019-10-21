#version 440
#define MAX_LIGHTS  45
struct Material
{
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
	sampler2D diffuseTex;
	sampler2D specularTex;
};
struct LightInfo
{
	vec3 LightPos;
	vec3 LightColor;
	mat4 LightMatrix;
	sampler2D LightShadow; 
};

in int AmountOfLights;
in vec3 vs_position;
in vec3 vs_color;
in vec2 vs_texcoord;
in vec3 vs_normal;
in float visibility;
uniform int LightCount;

out vec4 fs_color;

//Uniforms
uniform Material material;
uniform sampler2D texture0;
uniform sampler2D texture1;

uniform LightInfo AllLightInf[MAX_LIGHTS];
uniform vec3 cameraPos;
uniform vec3 SkyColor;

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
float ShadowCalculation(LightInfo LightToUse,vec3 Normal)
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
	vec3 result = vec3(0.f);
	for(int ii = 0; ii < LightCount ; ii++)
	{
	//Ambient Light

		vec3 ambientFinal = calculateAmbient(material);

	//Diffuse light

		vec3 diffuseFinal = calculateDiffuse(material, vs_position, vs_normal, AllLightInf[ii].LightPos);

	//Specular Light

		vec3 specularFinal = calculateSpecular(material, vs_position, vs_normal, AllLightInf[ii].LightPos, cameraPos);

	//Attenuation
		float shadow = ShadowCalculation(AllLightInf[ii],vs_normal);
	//Final Color
	result += (1.0-shadow) * (diffuseFinal + specularFinal) + ambientFinal;
	}
	
	vec3 color = texture(material.diffuseTex, vs_texcoord).rgb;
	vec3 Lighting = result * color;
	fs_color = vec4(AllLightInf[0].LightColor * Lighting,1.0);
	fs_color = mix(vec4(SkyColor, 1.0),fs_color, visibility);

}