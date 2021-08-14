#ifndef _GRAPHIC_CAMERA_H_
#define _GRAPHIC_CAMERA_H_

#include <cglm/cglm.h>

typedef struct camera {
	vec3 pos;
	float fov; // vertical fov in radians
	vec3 front, right, up;
	float yaw, pitch;
} camera_t;

camera_t camera_new(vec3 pos);
void camera_handle_mouse(camera_t *c, float dx, float dy);
void camera_handle_key(camera_t *c, vec3 in, float delta);
void camera_get_view(camera_t *c, mat4 view);
void camera_get_proj(camera_t *c, float ratio, mat4 view);

#endif // _GRAPHIC_CAMERA_H_
