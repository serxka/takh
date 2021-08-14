#ifndef _WINDOW_H_
#define _WINDOW_H_

#include <stdbool.h>

#include "common.h"
#include "graphic/camera.h"
#include "graphic/shader.h"
#include "graphic/vertex.h"

#include <GLFW/glfw3.h>

typedef struct window {
	GLFWwindow *handle;
	camera_t *cam;
	u32 width;
	u32 height;
} window_t;

window_t *window_init(u32 w, u32 h);
void window_resize(window_t *win, u32 w, u32 h);
bool window_poll(window_t *win);
void window_swap(window_t *win);
float window_ratio(window_t *win);
void window_close(window_t *win);

struct render_state {
	shader_t chunk_shader;
	verts_t *chunks;
	vecm chunks_v;
};

struct render_state *render_new(window_t *win);
void render(window_t *win, struct render_state *rs);
void render_cleanup(struct render_state *rs);

#endif // _WINDOW_H_
