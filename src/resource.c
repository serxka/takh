#include <arpa/inet.h>
#include <errno.h>
#include <fcntl.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <sys/mman.h>
#include <sys/stat.h>

#include "common.h"
#include "resource.h"

#define MAGIC ("cumpack!")
#define MAGIC_SIZE 8
#define COUNT_FIELD 8
#define BLOB_FIELD 10
#define HEADER_SIZE 14

static u8 *asset_mmap = NULL;
static u64 asset_size = 0;
static int mmap_fd = -1;

static struct settings *settings;

static u8 *mmap_assets(const char *path) {
	int fd = open(path, 0);
	if (fd < 0) {
		errorln("failed to open asset file (%d): %s", errno, path);
		return NULL;
	}
	struct stat st;
	fstat(fd, &st);

	u8 *mem = mmap(NULL, st.st_size, PROT_READ, MAP_PRIVATE, fd, 0);
	if (mem == (u8 *)-1)
		fatalln("mmap failed (%d): %s", errno, strerror(errno));

	// Check for the magic number
	if (memcmp(MAGIC, mem + 0, MAGIC_SIZE)) {
		errorln("asset file does not appear to be valid, missing magic number");
		return NULL;
	}

	asset_size = st.st_size;
	mmap_fd = fd;
	return mem;
}

static void munmap_assets(void) {
	if (asset_mmap == NULL)
		return;
	munmap(asset_mmap, asset_size);
}

void res_load(const char *path) {
	asset_mmap = mmap_assets(path);
}

void res_unload(void) {
	munmap_assets();
	asset_mmap = NULL;
	asset_size = 0;
	mmap_fd = -1;
	free(settings);
	settings = NULL;
}

static inline u16 read_u16(u8 *mem) {
	return ntohs(*(u16 *)mem);
}

static inline u32 read_u32(u8 *mem) {
	return ntohl(*(u32 *)mem);
}

u8 *res_request(const char *path, u64 *len_out) {
	if (asset_mmap == NULL)
		return errorln("tried to request resource '%s' when no asset archive was loaded",
		               path),
		       NULL;
	u8 *mem = asset_mmap;

	u16 count = read_u16(mem + COUNT_FIELD);
	u32 start = read_u32(mem + BLOB_FIELD);
	u8 *last = mem + HEADER_SIZE;
	for (size_t i = 0; i < count; ++i) {
		if (strncmp((char *)last, path, 1025)) {
			size_t strlen = strnlen((char *)last, 1025);
			if (strlen == 1025)
				return errorln("asset path was too long"), NULL;
			last += strlen + 1 + 8;
			continue;
		}
		while (*last++)
			;

		u32 len = read_u32(last + 0);
		u32 off = read_u32(last + 4);
		*len_out = len;
		return (u8 *)((u64)mem + (u64)start + (u64)off);
	}
	return NULL;
}

struct settings *res_settings(void) {
	if (!settings) {
		settings = alloc(1, sizeof(struct settings));
		// clang-format off
		*settings = (struct settings){
			.gfx = {
				.yfovd = 90,
				.debug = true,
			}
		};
		// clang-format on
	}
	return settings;
}
