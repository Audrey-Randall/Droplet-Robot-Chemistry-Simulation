#ifndef HEADER_FILE
#define HEADER_FILE
#include <string.h>


typedef uint8_t bool;

typedef struct
{
	int8_t valence[8]; //cut down data to 3 bits each? -1 for no slot (H and He), 0 for empty, 1 for free electron, 2 for ionically bonded, 3 for covalently bonded
	char name[2]; //someone said this should be 3 (because strings (char arrays) in C have a \0 at the end?) http://stackoverflow.com/questions/8484952/pruint8_tf-pruint8_ts-junk-characters
	//don't need the null char at end of string, instead pass length of "string"
	bool bondType; //mod instead?
	bool diatomic;	
	uint8_t atomicNum;
}Atom;

#endif
