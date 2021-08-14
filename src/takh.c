#include <stdio.h>

#include "graphic/camera.h"
#include "graphic/gl.h"
#include "graphic/shader.h"
#include "graphic/vertex.h"
#include "resource.h"
#include "window.h"

#include <cglm/cglm.h>

void update(window_t *win, float delta) {
	vec3 in = {
	        glfwGetKey(win->handle, GLFW_KEY_D)
	                + glfwGetKey(win->handle, GLFW_KEY_A) * -1,
	        glfwGetKey(win->handle, GLFW_KEY_SPACE)
	                + glfwGetKey(win->handle, GLFW_KEY_LEFT_SHIFT) * -1,
	        glfwGetKey(win->handle, GLFW_KEY_W)
	                + glfwGetKey(win->handle, GLFW_KEY_S) * -1,
	};
	camera_handle_key(win->cam, in, delta);
}

int main(void) {
	res_load("../assets.pak");
	window_t *win = window_init(1280, 720);

	struct render_state *rs = render_new(win);

	double time_last = 0.f;
	do {
		// Calculate how long the last frame took
		double time_now = glfwGetTime();
		float time_delta = time_now - time_last;
		time_last = time_now;

		// Run game update code
		update(win, time_delta);

		// Render everything
		render(win, rs);
	} while (window_poll(win));

	render_cleanup(rs);
	window_close(win);
}
