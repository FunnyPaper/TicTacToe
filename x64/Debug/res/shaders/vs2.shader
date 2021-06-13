#version 460 core

layout(location = 0) in vec3 vs_position;
layout(location = 1) in vec3 vs_color;
layout(location = 2) in vec3 vs_normal;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
uniform mat3 model_normals;

out vec3 fs_color;
out vec3 fs_position;
out vec3 fs_normal;

void main()
{
	gl_Position = projection * view * model * vec4(vs_position.x, vs_position.y, vs_position.z, 1.0f);
	fs_normal = vs_normal * model_normals;
	fs_color = vs_color;
	fs_position = vec3(model * vec4(vs_position, 1.0f));
}