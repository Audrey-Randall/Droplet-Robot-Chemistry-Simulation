#ifndef HEADER_FILE
#define HEADER_FILE
#include "stdbool.h"
#include <string.h>

typedef struct
{
	bool bondType;
	bool diatomic;
	int valence[8];
	char name[2];
	int atomicNum;
}Atom;

#endif
