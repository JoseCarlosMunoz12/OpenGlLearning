#version 440

in vec3 vs_position;
in vec3 vs_color;
in vec2 vs_texcoord;
in vec3 vs_normal;
in float visibility;

out vec4 fs_color;

uniform sampler2D Texture0;
uniform sampler2D Texture1;
uniform sampler2D Texture2;
uniform sampler2D Texture3;
uniform sampler2D Texture4;
uniform vec3 SkyColor;

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
	fs_color = backgroundTextureColor + rTextureColor + gTextureColor + bTextureColor;
	//fs_color = mix(vec4(SkyColor, 1.0),fs_color, visibility);
}