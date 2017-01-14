#include "option.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

char * v;
char * a;

int deletevar(const char * name)
{
	int res = unsetenv(name);
	if(!res)
		fprintf(stdout, "DELETED: %s\n", name);
	else
		fprintf(stderr, "VAR not found %s\n", name);
	return res;
}

int clearenvr(void)
{
	return clearenv();
}
void help(void)
{
	fprintf(stdout, "HELP\n");
}

void infovar(const char * name)
{
	char * var = getenv(name);
	if(var)
		fprintf(stdout, "GET: %s=%s\n", name, var);
	else
		fprintf(stderr, "VAR \"%s\" not found\n", name);
}

int __setvar(char * name, char * value)
{
	fprintf(stdout, "SET: %s=%s\n", name, value);
	return setenv(name, value, 1);
}

int setvar(char * arg)
{
	if (arg)
	{
		char * name = strtok(arg, "=");
		char * value = strtok(NULL, "=");
		return __setvar(name, value);
	}
	return -1;
}

void seta(char * pa)
{
	a = pa;
}

void setv(char * pv)
{
	v = pv;
}

void avopt()
{
	if(a && v)
		__setvar(a, v);
	else if(a && !v)
		__setvar(a, "");
	else if (v)
		fprintf(stderr, "-a not init\n");
}