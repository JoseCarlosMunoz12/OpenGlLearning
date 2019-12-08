#version 440 core
layout (location = 0) in vec3 vertex_position;
layout (location = 1) in vec3 vertex_color;
layout (location = 2) in vec2 vertex_texcoord;
layout (location = 3) in vec3 vertex_normal;

uniform mat4 LightSpaceMatrix;
uniform mat4 ModelMatrix;
void main()
{
	gl_Position = LightSpaceMatrix * ModelMatrix * vec4(vertex_position,1.f);
}