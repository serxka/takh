#version 450 core
in vec3 f_norm;
in vec3 f_uv;

layout (binding = 0) uniform sampler2DArray u_tex;

out vec4 o_colour;

const vec3 light_pos = vec3(-1.5, -1.0, 2.0);

void main()
{
	vec3 light_col = vec3(1.0, 1.0, 1.0);
	vec3 albedo = texture(u_tex, f_uv).rgb;
	
	vec3 light_dir = normalize(light_pos);
	vec3 diffuse = max(dot(f_norm, light_dir), 0.0) * light_col;
	
	vec3 result = (light_col * 0.4 + diffuse) * albedo;
	o_colour = vec4(result, 1.0);
}
