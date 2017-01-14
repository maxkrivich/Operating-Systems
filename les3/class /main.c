#include <stdio.h>
#include <stdlib.h>
#include <math.h>

int main(int argc, char *argv[])
{
	if(argc  == 4)
	{
		int a = atoi(argv[1]), b = atoi(argv[2]), c = atoi(argv[3]);
		double d = sqrt(b*b - 4*a*c);
		// printf("%g\n", d);
		if(!d)
			printf("x = %g\n", (double) (-b/(2*a)));
		else
		{
			double x1 = (-b-d)/(2*a),
				   x2 = (-b+d)/(2*a);
			printf("x1 = %g\tx2 = %g\n",x1, x2);
		}
	}
	else
	{
		printf("No args\n");
		return 1;
	}
	return 0;
}