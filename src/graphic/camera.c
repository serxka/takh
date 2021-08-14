#include <math.h>
#include <stdio.h>

#include "graphic/camera.h"
#include "resource.h"

#include <cglm/call.h>
#include <cglm/cglm.h>

camera_t camera_new(vec3 pos) {
	struct settings *settings = res_settings();

	camera_t cam = {0};
	glm_vec3_copy(pos, cam.pos);
	glm_vec3_copy(pos, cam.front);
	cam.fov = glm_rad(settings->gfx.yfovd);

	camera_handle_mouse(&cam, 0.f, 0.f);
	return cam;
}

void camera_handle_mouse(camera_t *c, float dx, float dy) {
	c->yaw += dx * 0.25f;
	c->pitch += dy * 0.25f;
	c->pitch = glm_clamp(c->pitch, -89.0f, 89.0f);

	glm_vec3_copy((vec3){cosf(glm_rad(c->yaw)) * cosf(glm_rad(c->pitch)),
	                     sinf(glm_rad(c->pitch)),
	                     sinf(glm_rad(c->yaw)) * cosf(glm_rad(c->pitch))},
	              c->front);
	glm_vec3_normalize(c->front);
	glm_vec3_crossn(c->front, (vec3){0.f, 1.f, 0.f}, c->right);
	glm_vec3_crossn(c->right, c->front, c->up);
}

void camera_handle_key(camera_t *c, vec3 in, float delta) {
	vec3 tmp;
	float vel = delta * 5.0f;
	// Do our x axis
	if (in[0] == 0.f)
		goto y;
	glm_vec3_scale(c->right, in[0] * vel, tmp);
	glm_vec3_add(c->pos, tmp, c->pos);
y: // Do our y axis
	if (in[1] == 0.f)
		goto z;
	glm_vec3_scale((vec3){0.f, 1.f, 0.f}, in[1] * vel, tmp);
	glm_vec3_add(c->pos, tmp, c->pos);
z: // Do our z axis
	if (in[2] == 0.f)
		return;
	glm_vec3_scale(c->front, in[2] * vel, tmp);
	glm_vec3_add(c->pos, tmp, c->pos);
}

void camera_get_view(camera_t *c, mat4 view) {
	glm_look(c->pos, c->front, c->up, view);
}

void camera_get_proj(camera_t *c, float ratio, mat4 proj) {
	glm_perspective(c->fov, ratio, 0.1f, 10000.f, proj);
}
