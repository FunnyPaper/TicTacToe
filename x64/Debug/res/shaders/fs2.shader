#version 460 core

in vec3 fs_color;
in vec3 fs_normal;
in vec3 fs_position;

out vec4 output_color;

struct Material
{
	vec3 ambient_color;
	vec3 diffuse_color;
	vec3 specular_color;
	float shininess;
};
struct Light
{
	vec3 position;
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
};

uniform vec3 view_position;
uniform Material material;
uniform Light light;

void main()
{
	//diffuse lightning
	vec3 norm = normalize(fs_normal);
	vec3 lightDir = normalize(light.position - fs_position);
	float diff = max(dot(norm, lightDir), 0.0);
	vec3 diffuse = (diff * material.diffuse_color) * light.diffuse;

	//ambient lightning
	vec3 ambient = light.ambient * material.ambient_color;

	//specular lightning
	vec3 viewDir = normalize(view_position - fs_position);
	vec3 reflectDir = reflect(-lightDir, norm);
	float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
	vec3 specular = (material.specular_color * spec) * light.specular;

	vec3 result = (ambient + diffuse + specular) * fs_color;
	output_color = vec4(result, 1.0f);
}