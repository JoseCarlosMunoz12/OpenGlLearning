 #version 440
#define MAX_LIGHTS 10

 struct Material
 {
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
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

struct DirLightInfo
{
	vec3 Ambient;
	vec3 Diffuse;
	vec3 Specular;
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

uniform int DirLightCount;
uniform int CnLightCount;
uniform int ArLightCount;

out vec4 fs_color;

uniform sampler2D Texture0;
uniform sampler2D Texture1;
uniform sampler2D Texture2;
uniform sampler2D Texture3;
uniform sampler2D Texture4;
uniform Material material;

uniform DirLightInfo AllDirInfo[MAX_LIGHTS];
uniform CnLightInfo AllCnInfo[MAX_LIGHTS];
uniform AreaLightInfo AllArInfo[MAX_LIGHTS];
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
float ShadowCalculation(sampler2D LightShadow,vec3 Normal,mat4 LightMatrix,vec3 LightPos,bool IsAr)
{
	vec4 FragPosLightSpace = LightMatrix * vec4(vs_position,1.f);
	float shadow = 0.f;
	vec3 LightDir = normalize( vs_position-LightPos );
	float bias = max(0.05 * (1.0 - dot(Normal,LightDir)),0.005);
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

	for(int ii =0; ii < ArLightCount; ii++)
	{ 
		vec3 FinalAmbient = CalculateAmbient(material) * AllArInfo[ii].Ambient;
		vec3 FinalDiffuse = material.diffuse * AllArInfo[ii].Diffuse;
		vec3 FinalSpecular = AllArInfo[ii].Specular * CalculateSpec(material,vs_position,vs_normal,
															AllArInfo[ii].LightPos,cameraPos);
		float shadow = ShadowCalculation(AllArInfo[ii].LightShadow,
										vs_normal,AllArInfo[ii].LightMatrix,
										AllArInfo[ii].LightPos,true);
		vec3 LightDir = normalize(AllArInfo[ii].LightPos - vs_position);
		float Theta = dot(LightDir,normalize(-1 * AllArInfo[ii].LightDirection));
		float Epsilon = (AllArInfo[ii].ConeAngle - AllArInfo[ii].UmbraAngle);
		float Intensity = clamp((Theta -AllArInfo[ii].ConeAngle) / Epsilon, 0.0, 1.0);

		float Dist = length(AllArInfo[ii].LightPos- vs_position);
		float Attenuation = 1.0 / (AllArInfo[ii].Constant + AllArInfo[ii].Linear * Dist + AllArInfo[ii].Quadratic * (Dist * Dist));
		result += Attenuation * (FinalAmbient + Intensity * (1.0 - shadow) * ( FinalDiffuse+ FinalSpecular));
	}
	
	for(int ii = 0; ii < CnLightCount; ii++)
	{
		vec3 LightDir = normalize(AllCnInfo[ii].LightPos - vs_position);
		float Theta = dot(LightDir , normalize(-1 * AllCnInfo[ii].LightDirection));
		if(Theta > AllCnInfo[ii].ConeAngle)
		{
			vec3 FinalAmbient = AllCnInfo[ii].Ambient * CalculateAmbient(material);
			vec3 FinalDiffuse = AllCnInfo[ii].Diffuse * material.diffuse;
			vec3 FinalSpecular = AllCnInfo[ii].Specular * CalculateSpec(material,vs_position,vs_normal,AllCnInfo[ii].LightPos,cameraPos);
			float shadow = ShadowCalculation(AllCnInfo[ii].LightShadow,vs_normal,AllCnInfo[ii].LightMatrix,AllCnInfo[ii].LightPos,false);
			result += FinalAmbient + (1.f - shadow) * (FinalDiffuse + FinalSpecular);

		}else{
			vec3 FinalAmbient = AllCnInfo[ii].Ambient * CalculateAmbient(material);
			result += FinalAmbient;
		}
	}

	for(int ii = 0; ii < DirLightCount ; ii++)
	{
		vec3 FinalAmbiant = AllDirInfo[ii].Ambient * CalculateAmbient(material);
		vec3 FinalDiffuse = AllDirInfo[ii].Diffuse * material.diffuse;
		vec3 FinalSpecular = AllDirInfo[ii].Specular * CalculateSpec(material,vs_position,vs_normal,AllDirInfo[ii].LightPos,cameraPos);

		float shadow = ShadowCalculation(AllDirInfo[ii].LightShadow,vs_normal,AllDirInfo[ii].LightMatrix,AllDirInfo[ii].LightPos,false);
		result += FinalAmbiant + (1.0 - shadow) * (FinalDiffuse + FinalSpecular);
	} 

	vec3 Lighting = (result) * color;
	fs_color = vec4(Lighting,1.0);
	fs_color = mix(vec4(SkyColor, 1.0),fs_color, visibility);
}