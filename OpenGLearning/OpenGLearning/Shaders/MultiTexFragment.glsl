 #version 440
#define MAX_LIGHTS 45

 struct Material
 {
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
 };

struct LightInfo
{
	vec3 LightPos;
	vec3 LightColor;
	mat4 LightMatrix;
	sampler2D LightShadow; 
};


in vec3 vs_position;
in vec3 vs_color;
in vec2 vs_texcoord;
in vec3 vs_normal;
in float visibility;
uniform int LightCount;

out vec4 fs_color;

uniform sampler2D Texture0;
uniform sampler2D Texture1;
uniform sampler2D Texture2;
uniform sampler2D Texture3;
uniform sampler2D Texture4;
uniform Material material;

uniform LightInfo AllLightInf[MAX_LIGHTS];
uniform vec3 cameraPos;
uniform vec3 SkyColor;

//Functions
vec3 CalculateAmbient(Material material)
{
	return material.ambient;
}
vec3 CalculateDiffuse(Material material, vec3 vs_position,vec3 vs_normal,vec3 LightPos)
{
	vec3 norm = normalize(vs_normal);
	vec3 PosToLightDirVec = normalize(LightPos - vs_position);
	float diffuse = max(dot(norm, PosToLightDirVec),0.0);
	return material.diffuse * diffuse;
}
vec3 CalculateSpec(Material material, vec3 vs_position,vec3 vs_normal,vec3 LightPos,vec3 CameraPos)
{
	vec3 lightToPosDirVec = normalize(vs_position - LightPos);
	vec3 reflectDirVec = normalize(reflect(lightToPosDirVec, normalize(vs_normal)));
	vec3 posToViewDirVec = normalize(CameraPos - vs_position);
	float specularConstant = pow(max(dot(posToViewDirVec, reflectDirVec), 0), 35);
	vec3 specularFinal = material.specular * specularConstant;

	return specularFinal;
}
// shadow function
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
	vec4 blendMapColor = texture(Texture4,vs_texcoord);
	float backTextureAmount = 1 - (blendMapColor.r + blendMapColor.g + blendMapColor.b);
	vec2 tiledCoords = vs_texcoord * 100.0;
	vec4 backgroundTextureColor = texture(Texture0,tiledCoords) * backTextureAmount;
	vec4 rTextureColor = texture(Texture1,tiledCoords) * blendMapColor.r;
	vec4 gTextureColor = texture(Texture2,tiledCoords) * blendMapColor.g;
	vec4 bTextureColor = texture(Texture3,tiledCoords) * blendMapColor.b;
	
	//Final light
	vec3 result = vec3(0.f);
	fs_color = backgroundTextureColor + rTextureColor + gTextureColor + bTextureColor;
	vec3 color = fs_color.rgb;

	for(int ii = 0; ii < LightCount ; ii++)
	{
		vec3 FinalAmbiant = CalculateAmbient(material);
		vec3 FinalDiffuse = material.diffuse;
		vec3 FinalSpecular = CalculateSpec(material,vs_position,vs_normal,AllLightInf[ii].LightPos,cameraPos);

		float shadow = ShadowCalculation(AllLightInf[ii],vs_normal);
		result += FinalAmbiant + (1.0 - shadow) * (FinalDiffuse + FinalSpecular);
	} 

	vec3 Lighting = (result) * color;
	fs_color = vec4(Lighting,1.0);
	fs_color = mix(vec4(SkyColor, 1.0),fs_color, visibility);
}