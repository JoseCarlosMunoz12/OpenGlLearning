#version 440 core
out vec4 FragColor;

in vec2 TexCoords;
uniform vec3 SkyColor;
uniform sampler2D Texture0;
void main()
{
	float DepthValue = texture(Texture0,TexCoords).r;
	FragColor = vec4(vec3(DepthValue),1.0);//orthographic
}