#version 450 core
layout (location = 0) in vec3 v_pos;
layout (location = 0) uniform mat4 u_view;
layout (location = 1) uniform mat4 u_proj;

void main()
{
	gl_Position = u_proj * u_view * vec4(v_pos, 1.0);
}
