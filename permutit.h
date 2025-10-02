#include <ctype.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#if defined(MSDOS) || defined(_MSDOS) || defined(__DOS__) || defined(__MSDOS__)
#define MAXN ((size_t) 8)
#else
#define MAXN ((size_t) 12)
#endif

#define BUFLEN 128

char *MEMmsg = "Heap error";

static size_t factrl(size_t);
static void bbsort(char **);
static void lptnfo(char *);
static void cleanup(char ***, char ***, size_t);
