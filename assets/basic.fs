#version 450 core
out vec4 o_colour;

void main()
{
	vec3 albedo = vec3(1.0, 0.0, 0.0);
	o_colour = vec4(albedo, 1.0);
}
