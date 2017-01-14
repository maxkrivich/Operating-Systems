#include "func.h"
#include "particle.h"

particle_t* pr;
size_t NMAX = 10;

int main(int argc, char ** argv)
{
	puts("GENERATION:");
	init(&pr, NMAX);
	print(pr, NMAX);
	puts("");
	int iter = 1;
	while(NMAX != 0)
	{
		int m = find_min(pr, NMAX), j = 0;
		double dis, min = DBL_MAX;
		for(int i = 0; i < NMAX; i++)
		{
			dis = dist(pr, m, i);
			if(min >= dis) // last point
			{
				min = dis;
				j = i;
			}
		}
		pr[j].mas += pr[m].mas;
		printf("%d ", iter++);
		PRINT(pr[j]);
		rm_pr(pr, &NMAX, j);
	}
	return 0;
}