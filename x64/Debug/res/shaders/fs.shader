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
	//sampler2D diffuse_texture2;
	//sampler2D specular_texture3;
	float shininess;
};
// Light Struct
struct Light
{
	vec3 position;
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
};

vec3 AmbientLight(Light l, Material m);
vec3 DiffuseLight(Light l, Material m);
vec3 SpecularLight(Light l, Material m);

uniform vec3 view_position;
uniform Material material;
uniform Light light;
uniform float control_opacity;
uniform bool is_texture_attached;

void main()
{
	// Diffuse lightning
	//vec3 diffuse = DiffuseLight(light, material);

	// Ambient lightning
	//vec3 ambient = AmbientLight(light, material);

	// Specular lightning
	//vec3 specular = SpecularLight(light, material);

	// Rezultat 
	//vec3 result = (ambient + diffuse + specular);

	// output_color = vec4(result, 1.0f);
	if (is_texture_attached)
		output_color = texture(material.diffuse_texture0, fs_texture);
	else
		//output_color = vec4(light.ambient * material.ambient_color * fs_color, 1.0f);
		output_color = vec4(fs_color, 1.0f);
}

vec3 AmbientLight(Light l, Material m)
{
	if (is_texture_attached)
	{
		//return l.ambient * vec3(mix(texture(m.diffuse_texture0, fs_texture), texture(m.diffuse_texture2, fs_texture), control_opacity));
		return l.ambient * vec3(texture(m.diffuse_texture0, fs_texture));
	}
	else
	{
		return l.ambient * m.ambient_color * fs_color;
	}
}
vec3 DiffuseLight(Light l, Material m)
{
	vec3 this_normal = normalize(fs_normal);
	//vec3 this_light_direction = normalize(-l.position);
	vec3 this_light_direction = normalize(l.position - fs_position);
	float this_diffuse_factor = max(dot(this_normal, this_light_direction), 0.0);
	if (is_texture_attached)
	{
		//return this_diffuse_factor * l.diffuse * vec3(mix(texture(m.diffuse_texture0, fs_texture), texture(m.diffuse_texture2, fs_texture), control_opacity));
		return this_diffuse_factor * l.diffuse * vec3(texture(m.diffuse_texture0, fs_texture));
	}
	else
	{
		return this_diffuse_factor * m.diffuse_color * l.diffuse * fs_color;
	}
}
vec3 SpecularLight(Light l, Material m)
{
	vec3 this_normal = normalize(fs_normal);
	//vec3 this_light_direction = normalize(-l.position);
	vec3 this_light_direction = normalize(l.position - fs_position);
	vec3 this_view_direction = normalize(view_position - fs_position);
	vec3 this_reflect_direction = reflect(-this_light_direction, this_normal);
	float this_specular_factor = pow(max(dot(this_view_direction, this_reflect_direction), 0.0), m.shininess);
	if (is_texture_attached)
	{
		//return this_specular_factor * l.specular * vec3(mix(texture(m.specular_texture1, fs_texture), texture(m.specular_texture3, fs_texture), control_opacity));
		return this_specular_factor * l.specular * vec3(texture(m.specular_texture1, fs_texture));
	}
	else
	{
		return m.specular_color * this_specular_factor * l.specular * fs_color;
	}
}