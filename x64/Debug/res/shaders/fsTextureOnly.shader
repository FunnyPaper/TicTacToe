#version 460 core

in vec2 fs_texture;

out vec4 output_color;

uniform float control_opacity;
uniform sampler2D Texture0;
uniform sampler2D Texture1;

void main()
{
	output_color = mix(texture(Texture0, fs_texture), texture(Texture1, fs_texture), control_opacity);
}