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
in float visibility;
in vec4 FragPosLightSpace;


out vec4 fs_color;

//Uniforms
uniform Material material;
uniform vec3 lightPos0;
uniform vec3 lightColor;
uniform vec3 cameraPos;
uniform vec3 SkyColor;
uniform sampler2D ShadowTex;

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
float ShadowCalculation(vec4 fragPosLightSpace,vec3 Normal,vec3 LightDirection,bool TypeOfShadow = true)
{
	float shadow = 0.f;
	float bias = max(0.05 * (1.0 - dot(Normal,LightDirection)),0.005);
	vec3 projCoords = fragPosLightSpace.xyz / fragPosLightSpace.w;
	projCoords = projCoords * 0.5 + 0.5;
	float closesetDepth = texture(ShadowTex,projCoords.xy).r;
	float currentDepth = projCoords.z;
	if(TypeOfShadow)
	{
		vec2 TexeSize = 1.0 / textureSize(ShadowTex,0);
		for(int x = -1; x <=1;++x)
		{
			for(int y = -1; y <=1;++y)
			{
				float pcfDepth = texture(ShadowTex,projCoords.xy + vec2(x,y) * TexeSize).r;
				shadow += currentDepth - bias > pcfDepth ? 1.0 : 0.0;
			}
		}
		shadow /=9.0;
	if (projCoords.z > 1.0)
		shadow = 0.0;
		return shadow;
	}
	shadow = (currentDepth - bias) > closesetDepth ? 1.0 : 0.0;

	return shadow;
}

void main()
{
	//fs_color = vec4(vs_color, 1.f);
	//fs_color = texture(texture0, vs_texcoord) * texture(texture1, vs_texcoord) * vec4(vs_color, 1.f);
	
	//Ambient light
	vec3 ambientFinal = calculateAmbient(material);

	//Diffuse light
	vec3 diffuseFinal = calculateDiffuse(material, vs_position, vs_normal, lightPos0);

	//Specular light
	vec3 specularFinal = calculateSpecular(material, vs_position, vs_normal, lightPos0, cameraPos);

	//Attenuation
	float shadow = ShadowCalculation(FragPosLightSpace,vs_normal,lightPos0);
	//Final light
	fs_color = texture(material.diffuseTex, vs_texcoord);
	fs_color = (vec4(ambientFinal, 1.f) + (1.0 - shadow) * (vec4(diffuseFinal, 1.f) + vec4(specularFinal, 1.f))) * fs_color;
	fs_color = mix(vec4(SkyColor, 1.0),fs_color, visibility);
	fs_color = vec4(lightColor * fs_color.xyz,1.f);
}
