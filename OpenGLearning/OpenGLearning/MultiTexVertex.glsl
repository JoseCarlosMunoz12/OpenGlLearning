#version 440

layout (location = 0) in vec3 vertex_position;
layout (location = 1) in vec3 vertex_color;
layout (location = 2) in vec2 vertex_texcoord;
layout (location = 3) in vec3 vertex_normal;

out vec3 vs_position;
out vec3 vs_color;
out vec2 vs_texcoord;
out vec3 vs_normal;
out float visibility;


uniform mat4 ModelMatrix;
uniform mat4 ViewMatrix;
uniform mat4 ProjectionMatrix;

uniform sampler2D Texture5;

const float density = 0.01;
const float gradient = 1.5;

void main()
{
	vec4 p = vec4(vertex_position,1.0f) +  vec4((vec3(0,1,0) * ((texture(Texture5,vertex_texcoord).r)) * 5.f) , 1.0f);
	//vs_position = vec4(ModelMatrix * vec4(vertex_position,1.f)).xyz;
	vs_position = (ModelMatrix * p).xyz;
	vec4 positionRelativeToCam = ViewMatrix * ModelMatrix * p;
	vs_color = vertex_color;
	vs_texcoord = vec2(vertex_texcoord.x, vertex_texcoord.y);
	vs_normal = mat3(ModelMatrix) * vertex_normal;

	gl_Position = ProjectionMatrix * ViewMatrix * ModelMatrix * p;

	float distance = length(positionRelativeToCam.xyz);

	visibility = exp(-pow((distance * density),gradient));
	visibility = clamp(visibility, 0.0,1.0);
}