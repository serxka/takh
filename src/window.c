#include <stdbool.h>

#include "chunk.h"
#include "common.h"
#include "graphic/camera.h"
#include "graphic/gl.h"
#include "graphic/shader.h"
#include "graphic/vertex.h"
#include "resource.h"
#include "voxel.h"
#include "window.h"

#include <GLFW/glfw3.h>

static void check_glfw_fatal(const char *fn) {
	const char *desc;
	int err = glfwGetError(&desc);
	if (err == GLFW_NO_ERROR)
		return;
	fatalln("%s: failed (0x%X): %s", fn, err, desc);
}

static void glfw_error_cb(int err_code, const char *desc) {
	errorln("glfw encountered an error (0x%X): %s", err_code, desc);
}

static void gl_debug_cb(GLenum source, GLenum type, GLuint id, GLenum severity,
                        GLsizei len, const GLchar *msg, const void *user) {
	UNUSED(source);
	UNUSED(type);
	UNUSED(id);
	UNUSED(severity);
	UNUSED(len);
	UNUSED(user);
	warnln("GL_KHR_debug: %s", msg);
}

static void glfw_resize_cb(GLFWwindow *handle, int w, int h) {
	window_t *win = glfwGetWindowUserPointer(handle);
	window_resize(win, w, h);
}

static void glfw_mouse_cb(GLFWwindow *handle, double x, double y) {
	static double px, py;
	window_t *win = glfwGetWindowUserPointer(handle);
	y = -y;
	if (glfwGetInputMode(win->handle, GLFW_CURSOR) != GLFW_CURSOR_NORMAL)
		camera_handle_mouse(win->cam, x - px, y - py);
	px = x;
	py = y;
}

static void glfw_key_cb(GLFWwindow *handle, int key, int scan, int act,
                        int mods) {
	UNUSED(scan);
	UNUSED(mods);

	window_t *win = glfwGetWindowUserPointer(handle);
	if (key == GLFW_KEY_ESCAPE && act == GLFW_PRESS) {
		if (glfwGetInputMode(win->handle, GLFW_CURSOR)
		    == GLFW_CURSOR_NORMAL) {
			glfwSetInputMode(win->handle, GLFW_RAW_MOUSE_MOTION,
			                 GLFW_TRUE);
			glfwSetInputMode(win->handle, GLFW_CURSOR,
			                 GLFW_CURSOR_DISABLED);
		} else {
			glfwSetInputMode(win->handle, GLFW_RAW_MOUSE_MOTION,
			                 GLFW_FALSE);
			glfwSetInputMode(win->handle, GLFW_CURSOR,
			                 GLFW_CURSOR_NORMAL);
		}
	}
}

static void check_gl_features(void);
static void setup_opengl_state(void);

window_t *window_init(u32 w, u32 h) {
	window_t *win = alloc(1, sizeof(window_t));
	struct settings *settings = res_settings();

	if (!glfwInit())
		check_glfw_fatal("glfwInit");

	// Tell GLFW what kind of OpenGL context we are looking for
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 5);
	if (settings->gfx.debug)
		glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GLFW_TRUE);

	// Create our window
	win->handle = glfwCreateWindow(w, h, "voxel", NULL, NULL);
	if (!win->handle)
		check_glfw_fatal("glfwInit");
	glfwSetWindowUserPointer(win->handle, win);

	// Set our GLFWwindow GL context as the current one
	glfwMakeContextCurrent(win->handle);
	check_glfw_fatal("glfwMakeContextCurrent");
	glfwSwapInterval(1);


	// Set our GLFW callbacks
	glfwSetErrorCallback(glfw_error_cb);
	glfwSetWindowSizeCallback(win->handle, glfw_resize_cb);
	glfwSetCursorPosCallback(win->handle, glfw_mouse_cb);
	glfwSetKeyCallback(win->handle, glfw_key_cb);

	// Check that OpenGL has the features we need
	gl_loadproc_minimal();
	check_gl_features();
	setup_opengl_state();

	// Initial window resize to set OpenGL viewport
	window_resize(win, w, h);
	return win;
}

void window_resize(window_t *win, u32 w, u32 h) {
	win->width = w;
	win->height = h;
	gl_Viewport(0, 0, win->width, win->height);
}

bool window_poll(window_t *win) {
	glfwPollEvents();
	return !glfwWindowShouldClose(win->handle);
}

void window_swap(window_t *win) {
	glfwSwapBuffers(win->handle);
}

float window_ratio(window_t *win) {
	return (float)win->width / (float)win->height;
}

void window_close(window_t *win) {
	glfwTerminate();
	free(win);
}

static void check_gl_features(void) {
	GLint f;
	gl_GetIntegerv(GL_CONTEXT_FLAGS, &f);

	if (f & ~GL_CONTEXT_FLAG_DEBUG_BIT)
		errorln("OpenGL context does not contain the required %s extension",
		        "GL_KHR_debug");
}

static void setup_opengl_state(void) {
	struct settings *settings = res_settings();

	// Load all of our OpenGL functions
	gl_loadproc();

	// Setup basic OpenGL state
	if (settings->gfx.debug)
		gl_DebugMessageCallback(gl_debug_cb, NULL);
	gl_Enable(GL_DEPTH_TEST);
	gl_Enable(GL_CULL_FACE);
	gl_FrontFace(GL_CW);
	gl_CullFace(GL_BACK);
}

struct render_state *render_new(window_t *win) {
	struct render_state *rs = alloc(1, sizeof(struct render_state));

	// Create and set our camera
	camera_t *cam = alloc(1, sizeof(camera_t));
	*cam = camera_new((vec3){0.f, 0.f, 0.f});
	win->cam = cam;

	// Setup our frame buffer clear colour
	gl_ClearColor(0.85f, 0.61f, 0.66f, 1.f);
	// gl_ClearColor(1.f, 1.f, 1.f, 1.f);

	// Setup our chunk render states
	rs->chunk_shader = shader_compile("chunk.vs", "chunk.fs");
	rs->chunks_v = vecm_new((void **)&rs->chunks, sizeof(verts_t));

	chunk_t chunk = chunk_new();
	chunk_setv(&chunk, 0, 0, 0, &(voxel_t){true});
	chunk_setv(&chunk, 1, 1, 1, &(voxel_t){true});
	verts_t mesh = chunk_mesh(&chunk);

	vecm_push(&rs->chunks_v, (void **)&rs->chunks, &mesh);

	return rs;
}

void render(window_t *win, struct render_state *rs) {
	camera_t *cam = win->cam;
	shader_t *shdr; // Generic working shader shorthand
	mat4 view, proj;

	// Clear our buffers
	gl_Clear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// Bind and set our chunk shader uniforms
	shdr = &rs->chunk_shader;
	shader_bind(shdr);
	camera_get_view(cam, view);
	camera_get_proj(cam, window_ratio(win), proj);
	set_uni_mat4(shdr, 0, view);
	set_uni_mat4(shdr, 1, proj);
	set_uni_vec3(shdr, 2, (vec3){0.f, 0.f, 0.f});

	for (u64 i = 0; i < rs->chunks_v.len; ++i) {
		verts_draw(rs->chunks + i);
	}

	// Swap our framebuffer
	window_swap(win);
}

void render_cleanup(struct render_state *rs) {
	shader_delete(&rs->chunk_shader);
	for (u64 i = 0; i < rs->chunks_v.len; ++i)
		verts_delete(rs->chunks + i);
	free(rs->chunks);
}
