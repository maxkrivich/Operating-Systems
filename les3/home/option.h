#ifndef _OPTION_H_

	#define _OPTION_H_ 

	int deletevar(const char*);
	int clearenvr(void);
	void help(void);
	void infovar(const char*);
	int setvar(char*);
	int __setvar(char*, char*);
	void seta(char*);
	void setv(char*);
	void avopt(void);

	extern char * a;
	extern char * v;

#endif