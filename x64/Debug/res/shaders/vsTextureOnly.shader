#version 460 core

layout(location = 0) in vec3 vs_position;
layout(location = 1) in vec2 vs_texture;

uniform vec2 changePos;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

out vec2 fs_texture;

void main()
{
	gl_Position = projection * view * model * vec4(vs_position.x + changePos.x, vs_position.y + changePos.y, vs_position.z, 1.0f);
	fs_texture = vs_texture;
}