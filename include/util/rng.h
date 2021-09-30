#ifndef _UTIL_RNG_H_
#define _UTIL_RNG_H_

#include "common.h"

typedef struct rng {
	u64 seed;
} rng_t;

rng_t *rng_thread(void);
double perlin_smpl2d(double x, double y, double freq, u32 depth);
double perlin_smpl2d_s(rng_t *rng, double x, double y, double freq, u32 depth);

#endif // _UTIL_RNG_H_