#version 440

in vec3 vs_position;
in vec3 vs_color;
in vec2 vs_texcoord;
in vec3 vs_normal;

out vec4 fs_color;

uniform sampler2D Texture0;
uniform sampler2D Texture1;
uniform sampler2D Texture2;
uniform sampler2D Texture3;


void main()
{
	//Final light
	fs_color = texture(Texture2,vs_texcoord) * texture(Texture0,vs_texcoord) ;
}