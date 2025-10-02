#include "permutit.h"

extern char *nfostr;
extern char *optarg;
extern int optdlm;

extern int cltail(int, char *const[], const char *, char ***, char ***);
extern void combnext(size_t *, size_t, size_t);
extern void permnext(size_t *, size_t, size_t);

int main(int argc, char *argv[])
{
	size_t finele, i, j, n, nCr, nPr, nlines, r, x;
	size_t seq[MAXN];
	void (*permutit)(size_t *, size_t, size_t);
	char **args, **opts, **u, **v;
	char buf[BUFLEN];
	int c, spchr;

	if (argc < 2) {
		fprintf(stderr, "For information, enter:\n");
		fprintf(stderr, "%s -?\n", argv[0]);
		return 0;
	}
	spchr = ' ';
	n = nlines = 0;
	c = cltail(argc, argv, "+:CDIR:Scdir:s", &opts, &args);
	if (c == ENOMEM) {
		cleanup(&opts, &args, 0);
		perror(MEMmsg);
		exit(errno);
	}
	if (c == '?') {
		lptnfo(argv[0]);
		cleanup(NULL, NULL, 0);
		return '?';
	}
	if (c != 0) {
		cleanup(&opts, &args, 0);
		fprintf(stderr, "For more information, enter:\n");
		fprintf(stderr, "%s %c?\n", argv[0], optdlm);
		return EXIT_FAILURE;
	}
	for (u = opts; *u != NULL; ++u)
		if (**u == 'I' || **u == 'i') break;
	if (*u != NULL) {
		spchr = '\n';
		args = realloc(args, sizeof (char *));
		if (args == NULL) {
			cleanup(&opts, &args, nlines);
			perror(MEMmsg);
			exit(errno);
		}
		*args = NULL;
		while (fgets(buf, BUFLEN, stdin) != NULL) {
			if (*buf == '\n') break;
			args = realloc(args, (++nlines + 1) * sizeof (char *));
			if (args == NULL)
				cleanup(&opts, &args, nlines);
			j = strlen(buf);
			if (j == BUFLEN - 1 || buf[j - 1] == '\n')
				buf[--j] = '\0';
			*(args + n) = calloc(j + 1, sizeof (*buf));
			if (*(args + n) == NULL) {
				perror(MEMmsg);
				cleanup(&opts, &args, nlines);
				exit(errno);
			}
			strcpy(*(args + n), buf);
			*(args + ++n) = NULL;
		}
	}
	else {
		v = args;
		while (*v != NULL) {
			++n;
			++v;
		}
	}
	if (n > MAXN) {
		fprintf(stderr, "Exceeded maximum of %lu ", MAXN);
		fprintf(stderr, "positional arguments: %lu given.\n", n);
		cleanup(&opts, &args, nlines);
		exit(EXIT_FAILURE);
	}
	for (u = opts; *u != NULL; ++u)
		if (**u == 'R' || **u == 'r') break;
	(*u != NULL) ? (r = (size_t) atoi(*u + 1)) : (r = n);
	if (r > n) {
		fputs("Out-of-range r value.\n", stderr);
		cleanup(&opts, &args, nlines);
		return EXIT_FAILURE;
	}
	else if (n == 0 || r == 0) {
		cleanup(&opts, &args, nlines);
		return 0;
	}
	for (u = opts; *u != NULL; ++u)
		if (**u == 'S' || **u == 's') break;
	if (*u != NULL) bbsort(args);
	nPr = factrl(n) / factrl(n - r);
	nCr = nPr / factrl(r);
	for (u = opts; *u != NULL; ++u)
		if (**u == 'C' || **u == 'c') break;
	if (*u == NULL) {
		for (j = n - 1, i = 0; i < r; --j, ++i)
			*(seq + i) = j;
		permutit = permnext;
		x = nPr;
	}
	else {
		for (j = n - r, i = 0; i < r; ++j, ++i)
			*(seq + i) = j;
		permutit = combnext;
		x = nCr;
	}
	for (u = opts; *u != NULL; ++u)
		if (**u == 'D' || **u == 'd')
			break;
	if (*u != NULL)
		(spchr == ' ') ? (spchr = '\n') : (spchr = ' ');
	finele = r - 1;
	i = 0;
	while (i < x) {
		permutit(seq, n, r);
		for (j = 0; j < r; ++j) {
			fputs(*(args + *(seq + j)), stdout);
			if (j < finele)
				putchar(spchr);
			else {
				putchar('\n');
				if (spchr == '\n') putchar('\n');
			}
		}
		++i;
	}
	cleanup(&opts, &args, nlines);
	return EXIT_SUCCESS;
}

static size_t factrl(size_t n)
{
	size_t y;

	for (y = 1; n > 1; --n)
		y *= n;
	return y;
}

static void bbsort(char *vec[])
{
	char **u, **v;
	char *tptr;
	int sdiff, sorted;

	if (vec[0] == NULL || vec[1] == NULL)
		return;
	do {
		sorted = 1;
		v = vec;
		for (u = v, ++v; *v != NULL; ++u, ++v) {
			sdiff = strcmp(*u, *v);
			if (sdiff > 0) {
				tptr = *u;
				*u = *v;
				*v = tptr;
				sorted = 0;
			}
		}
	} while (sorted == 0);
	return;
}

static void lptnfo(char *s) {
	fprintf(stderr, "%s ", s);
	fprintf(stderr, "<%cc> <%cd> <%cr[x]> <%cs> <%c%c> ",
	 optdlm, optdlm, optdlm, optdlm, optdlm, optdlm);
	fprintf(stderr, "[arg1]..<arg%lu>\n", MAXN);
	fputc('\n', stderr);
	fprintf(stderr, "Options:\n");
	fprintf(stderr, " c  : Print only combinations\n");
	fprintf(stderr, " d  : Change token separator\n");
	fprintf(stderr, " r  : Specify an r value (where, 0 <= r <= n)\n");
	fprintf(stderr, " s  : Sort arguments in ascending ASCII order\n");
	fprintf(stderr, " %c  : Ends processing of options\n", optdlm);
	fputc('\n', stderr);
	fprintf(stderr, "Options, if any, precede positional arguments.\n");
	return;
}

static void cleanup(char ***opts, char ***args, size_t nlines)
{
	char **chpp;

	if (opts != NULL && *opts != NULL) {
		chpp = *opts;
		while (*chpp != NULL) {
			free(*chpp);
			++chpp;
		}
		free(*opts);
	}
	if (args != NULL && *args != NULL) {
		chpp = *args;
		if (nlines != 0)
			while (*chpp != NULL) {
				free(*chpp);
				++chpp;
			}
		free(*args);
	}
	return;
}
