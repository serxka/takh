#ifndef _COMMON_H_
#define _COMMON_H_

#include <assert.h>
#include <errno.h>
#include <stdarg.h>
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#define ARR_LEN(x) (sizeof(x) / sizeof((x)[0]))
#define UNUSED(x) (void)(x)

#define PACKED __attribute__((packed))

int debugln(const char *fmt, ...);
int logln(const char *fmt, ...);
int warnln(const char *fmt, ...);
int errorln(const char *fmt, ...);
void fatalln(const char *fmt, ...);

typedef uint8_t u8;
typedef uint16_t u16;
typedef uint32_t u32;
typedef uint64_t u64;
typedef int8_t s8;
typedef int16_t s16;
typedef int32_t s32;
typedef int64_t s64;

static inline void *alloc(size_t nmemb, size_t size) {
	void *mem = calloc(nmemb, size);
	if (mem == NULL)
		fatalln("malloc: %s", strerror(errno));
	return mem;
}

typedef struct vecm {
	size_t len;
	size_t cap;
	u16 size;
} vecm;

static inline vecm vecm_new(void **d, u16 size) {
	*d = NULL;
	return (vecm){.len = 0, .cap = 0, .size = size};
}

static size_t vecm_left(vecm *v) {
	return v->cap - v->len;
}

static void vecm_grow(vecm *v, void **d, size_t amt) {
	size_t new_cap = v->cap + (amt == 0 ? v->cap : amt);
	if (v->cap == 0)
		new_cap = (amt != 0) ? amt : 4;
	void *new = realloc(*d, new_cap * v->size);
	if (new == NULL)
		fatalln("realloc: %s", strerror(errno));
	v->cap = new_cap;
	*d = new;
}

static inline void vecm_push(vecm *v, void **d, const void *i) {
	if (v->len == v->cap)
		vecm_grow(v, d, 0);
	u8 *last = (u8 *)*d + v->len * v->size;
	memcpy(last, i, v->size);
	++v->len;
}

static inline void vecm_append(vecm *v, void **d, const void *s, size_t len) {
	if (v->len == v->cap)
		vecm_grow(v, d, len - vecm_left(v));
	u8 *last = (u8 *)*d + v->len * v->size;
	memcpy(last, s, len * v->size);
	v->len += len;
}

#endif // _COMMON_H_