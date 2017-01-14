#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(void)
{
	char * str = (char *) malloc(sizeof(char) * 256);
	scanf("%s", str);
	for(char * pt = strtok(str, ";"); pt ; pt = strtok(NULL, ";"))
		system(pt); 
	return 0;
}