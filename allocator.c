// code from https://www.youtube.com/watch?v=GIWeQ2I67rk&t=474s

#define _GNU_SOURCE // to avoid error use of undeclared identifier RTLD_NEXT https://techoverflow.net/2019/06/20/how-to-fix-c-error-rtld_next-undeclared/
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <stdint.h>
#include <dlfcn.h>
#include <stdlib.h>
#include <errno.h>

typedef void * (*malloc_like_function) (size_t);
typedef void (*free_like_function) (void *);

static malloc_like_function sysmalloc = NULL;
static free_like_function sysfree = NULL;
static bool init = false;
static FILE *fp = NULL;
static const char *logfilename = "./allocs.log";

void initcheck()
{
	if (!init) {
		sysmalloc = (malloc_like_function)dlsym(RTLD_NEXT, "malloc");
		if (!sysmalloc) {
			printf("sysmalloc not found\n");
			exit(1);
		}
		sysfree = (free_like_function)dlsym(RTLD_NEXT, "free");
		if (!sysfree) {
			printf("sysfree not found\n");
			exit(1);
		}
		//fp = fopen(logfilename, "w+");
		fp = fopen("demo_file.txt", "w");

		if (!fp) {
			fprintf(stderr, "Error %s\n", strerror(errno));
			printf("Cannot open file\n");
			exit(1);
		}

		init = true;
	}
}

void *malloc(size_t size)
{
	initcheck();
	void *result = sysmalloc(size);
	fprintf(fp, "M, %lu, %lu\n", (uintptr_t)result, size);
	return result;
}

void free(void *p)
{
	initcheck();
	fprintf(fp, "F, %lu\n", (uintptr_t)p);
	sysfree(p);
}
