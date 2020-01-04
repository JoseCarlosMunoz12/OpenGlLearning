#version 440

layout (location = 0) in vec3 vertex_position;
layout (location = 1) in vec3 vertex_color;
layout (location = 2) in vec2 vertex_texcoord;
layout (location = 3) in vec3 vertex_normal;
layout (location = 4) in ivec3 vertex_matid;
layout (location = 5) in vec3 vertex_weights;

out vec3 vs_position;
out vec3 vs_color;
out vec2 vs_texcoord;
out vec3 vs_normal;
out float visibility;

const int MAX_BONES = 100;

uniform mat4 ModelMatrix;
uniform mat4 ViewMatrix;
uniform mat4 ProjectionMatrix;
uniform mat4 Bones[MAX_BONES];

const float density = 0.01f;
const float gradient = 1.5f;

mat4 VerifyVal(int MatId,float Weight)
{
	mat4 Temps = mat4(1.0);
	if ( MatId == -1)
	{
	return mat4(0.0);
	}
	Temps = Bones[MatId] * Weight;
	return Temps;
}
void main()
{
	mat4 BoneTransform = VerifyVal(vertex_matid.x,vertex_weights.x);
	BoneTransform += VerifyVal(vertex_matid.y,vertex_weights.y);
	BoneTransform += VerifyVal(vertex_matid.z,vertex_weights.z);

	vec4 GLPos = BoneTransform * vec4(vertex_position,1.f);
	vec4 positionRelativeToCam = ViewMatrix * ModelMatrix * GLPos;
	vs_position = vec4(ModelMatrix * GLPos).xyz;
	vs_texcoord = vertex_texcoord;

	vec4 NormalL = BoneTransform * vec4(vertex_normal,1.f);
	vs_normal = vec4(ModelMatrix * NormalL).xyz;

	gl_Position = ProjectionMatrix * ViewMatrix * ModelMatrix * GLPos;
	float distance = length(positionRelativeToCam.xyz);

	visibility = exp(-pow((distance * density),gradient));
	visibility = clamp(visibility,0.0,1.0);
	vs_color = vertex_color;
}