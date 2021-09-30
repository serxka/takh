#version 450 core
layout (location = 0) in vec3 v_pos;
layout (location = 1) in uint v_norm;
layout (location = 2) in vec2 v_uv;
layout (location = 3) in uint v_texi;

layout (location = 0) uniform mat4 u_view;
layout (location = 1) uniform mat4 u_proj;
layout (location = 2) uniform vec3 u_modl;

out vec3 f_norm;
out vec3 f_uv;

const vec3 normals[6] = vec3[6](
	vec3( 0.0, 0.0,-1.0),
	vec3( 0.0, 0.0, 1.0),
	vec3(-1.0, 0.0, 0.0),
	vec3( 1.0, 0.0, 0.0),
	vec3( 0.0, 1.0, 0.0),
	vec3( 0.0,-1.0, 0.0)
);

void main()
{
	f_norm = normals[v_norm];
	f_uv = vec3(v_uv, float(v_texi));
	gl_Position = u_proj * u_view * vec4((v_pos + u_modl) * 0.5, 1.0);
}
