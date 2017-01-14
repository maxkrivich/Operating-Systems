#include "func.h"
// #include "particle.h"

void init(particle_t ** pr, size_t size)
{
	if((*pr = (particle_t*) malloc(size * sizeof(particle_t))) != NULL)
	{
		srand(time(NULL));
		for(int i = 0; i < size; i++)
		{
			(*pr)[i].x = rand();
			(*pr)[i].y = rand();
			(*pr)[i].mas = (double) rand() / rand();
		}
	}
}

void print(particle_t * arr, size_t size)
{
	for(int i = 0; i < size; i++)
	{
		printf("%d ", i+1);
		PRINT(arr[i]);
	}
}

void rm_pr(particle_t * array, size_t * size, size_t ind)
{
	for(int i = ind + 1; i < *size; i++)
		array[i-1] = array[i];

	(*size)--;

	array = (particle_t*) realloc(array, (*size) * sizeof(particle_t));
}

double dist(particle_t * pr, int i, int j)
{
	return sqrt(SQ(pr[j].x - pr[i].x) + SQ(pr[j].y - pr[i].y));
}

int find_min(particle_t * pr, size_t size)
{
	int mini = -1; // error code "-1"
	double min = DBL_MAX;
	for(int i = 0; i < size; i++)
		if(min > pr[i].mas)
		{
			mini = i;
			min = pr[i].mas;
		}
	return mini;
}

void clear(particle_t** pr)
{
	free(*pr);
}	