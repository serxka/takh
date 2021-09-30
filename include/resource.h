#ifndef _RESOURCE_H_
#define _RESOURCE_H_

#include <stdbool.h>

#include "common.h"

struct settings {
	struct {
		float yfovd;
		bool debug;
		int msaa_samples;
	} gfx;
};

void res_load(const char *path);
u8 *res_request(const char *path, u64 *len);
void res_unload(void);
struct settings *res_settings(void);

#endif // _RESOURCE_H_
