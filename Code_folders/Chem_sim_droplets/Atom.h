#ifndef HEADER_FILE
#define HEADER_FILE
#include "stdbool.h"
#include <string.h>

typedef struct
{
	bool bondType;
	bool diatomic;
	int valence[8];
	char name[2]; //someone said this should be 3 (because strings (char arrays) in C have a \0 at the end?) http://stackoverflow.com/questions/8484952/printf-prints-junk-characters
	int atomicNum;
	char chemID[13];
}Atom;

#endif
