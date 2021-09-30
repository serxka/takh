#include <stdio.h>
#include <stdlib.h>

#include <common.h>

#define STDERR (stderr)

static inline int log_header(const char *msg, const char *colour) {
	return fprintf(STDERR, "\x1B[%sm[%s]\033[0m\t", colour, msg);
}

int debugln(const char *fmt, ...) {
	va_list args;
	va_start(args, fmt);
	int written = 0;
	written += log_header("DEBUG", "0;36");
	written += vfprintf(STDERR, fmt, args);
	if (putc('\n', STDERR) != EOF)
		written += 1;
	va_end(args);
	return written;
}

int logln(const char *fmt, ...) {
	va_list args;
	va_start(args, fmt);
	int written = 0;
	written += log_header("INFO", "0;36");
	written += vfprintf(STDERR, fmt, args);
	if (putc('\n', STDERR) != EOF)
		written += 1;
	va_end(args);
	return written;
}

int warnln(const char *fmt, ...) {
	va_list args;
	va_start(args, fmt);
	int written = 0;
	written += log_header("WARN", "0;33");
	written += vfprintf(STDERR, fmt, args);
	if (putc('\n', STDERR) != EOF)
		written += 1;
	va_end(args);
	return written;
}

int errorln(const char *fmt, ...) {
	va_list args;
	va_start(args, fmt);
	int written = 0;
	written += log_header("ERROR", "0;31");
	written += vfprintf(STDERR, fmt, args);
	if (putc('\n', STDERR) != EOF)
		written += 1;
	va_end(args);
	return written;
}

void fatalln(const char *fmt, ...) {
	va_list args;
	va_start(args, fmt);
	log_header("FATAL", "0;35");
	vfprintf(STDERR, fmt, args);
	putc('\n', STDERR);
	va_end(args);

	exit(EXIT_FAILURE);
}
