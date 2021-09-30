project( 'takh', 'c',
	version : '0.1',
	default_options : ['warning_level=3', 'c_std=c11'])
cc = meson.get_compiler('c')

src = ['src/takh.c', 'src/window.c', 'src/resource.c', 'src/chunk.c', 'src/voxel.c',
       'src/util/log.c', 'src/util/rng.c',
       'src/graphic/gl.c', 'src/graphic/camera.c', 'src/graphic/vertex.c', 'src/graphic/shader.c', 'src/graphic/texture.c']

libm = cc.find_library('m', required : false)
cglm = dependency('cglm', fallback : 'cglm')
glfw = dependency('glfw3')

exe = executable( 'takh',
	sources : src,
	include_directories : include_directories('include'),
	dependencies : [cglm, glfw, libm])
