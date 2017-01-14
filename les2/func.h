#ifndef _FUNC_H_
	#define _FUNC_H

	#include <stdio.h> 
	#include <stdlib.h>
	#include <ctype.h>
	#include <math.h>
	#include <time.h>
	#include <float.h>
	#include <stdbool.h>
	#include <string.h>
	#include "particle.h"

	#define SQ(a) (a)*(a)
	#define MAX(a, b) (a) > (b) ? (b) : (a)
	#define PRINT(p) printf("x=%d\ty=%d\tmas=%f\n", p.x, p.y, p.mas)
	// #define SIZE(x)  (sizeof (x) / sizeof((x)[0]))

	void init(particle_t **, size_t);
	void print(particle_t *, size_t);
	void rm_pr(particle_t *, size_t *, size_t);
	double dist(particle_t *, int, int);
	int find_min(particle_t *, size_t);
	void clear(particle_t**);

#endif