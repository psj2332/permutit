#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#if defined(MSDOS) || defined(_MSDOS) || defined(__DOS__) || defined(__MSDOS__)
#define WINDOS 1
#elif defined(_WIN64) || defined(_WIN32) || defined(__WIN32__)
#define WINDOS 1
#elif defined(_POSIX_VERSION) || defined(__linux__) || defined(__unix__)
#define POSIXY 1
#else
#error
#endif

int cltail(int, char *const[], const char *, char ***, char ***);
static int dospar(int, char *const[], const char *, int *);
static int optdos(int, char *const[], const char *, char ***);
static int optget(int, char *const[], const char *);
static int clihlp(char *const[], const char *, const char *);
static int isdup(char **);
static void setdlm(int, char *const[], const char *);

char *DLMmsg = "Delimiter missing option:";
char *OMAmsg = "Option missing argument:";
char *OPTmsg = "Option invalid or missing argument:";
char *MALmsg = "calloc()";

char *optarg;
int opterr = 0;
int optind = 1;
int optopt;

#ifdef WINDOS
const char *nfostr = "?h";
#else
const char *nfostr = "?h";
#endif

int chkdup = 1;
int chkhlp = 1;
int errmsg = 1;
#ifdef POSIXY
int optdlm = '-';
#else
int optdlm = '\0';
#endif

#define ERR(_s, _c) if (opterr != 0 && *optstring != ':')\
 fprintf(stderr, "%s %c\n", _s, _c);
#define _ERRCH(_s, _c) if (opterr == 0 && errmsg != 0)\
 fprintf(stderr, "%s %c\n", _s, _c);
#define _ERRST(_p, _q) if (opterr == 0 && errmsg != 0)\
 fprintf(stderr, "%s %s\n", _p, _q);
