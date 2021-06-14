#version 330 core

// From Vertex shader
in vec3 fs_color;
in vec2 fs_texture;
in vec3 fs_normal;
in vec3 fs_position;

// Output color
out vec4 output_color;

// Material Struct
struct Material
{
	vec3 ambient_color;
	vec3 diffuse_color;
	vec3 specular_color;
	sampler2D diffuse_texture0;
	sampler2D specular_texture1;
	float shininess;
};

uniform vec3 view_position;
uniform Material material;
uniform float control_opacity;
uniform bool is_texture_attached;

void main()
{
	if (is_texture_attached)
		output_color = texture(material.diffuse_texture0, fs_texture);
	else
		output_color = vec4(fs_color, 1.0f);
}