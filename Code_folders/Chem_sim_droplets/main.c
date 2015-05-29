/*
 * Created: 5/28/2015 1:11:07 PM
 *  Author: Audrey
 */ 
#include "droplet_programs/test_code.h"
#include "Atom.h"
#include "stdbool.h"
#include <stdio.h>
#include "vector.h"

#define FOSC 16000000
#define BAUD 115200
#define MYUBRR (((((FOSC * 10) / (16L * BAUD)) + 5) / 10) - 1)

//also makes printf work
int usart_putchar_printf(char var, FILE *stream) {
	if (var == '\n') usart_putchar('\r');
	usart_putchar(var);
	return 0;
}

static FILE mystdout = FDEV_SETUP_STREAM(usart_putchar_printf, NULL, _FDEV_SETUP_WRITE); //makes printf work?
//stdout = &mystdout;
//When using Tera Term, go to Setup -> Serial port and change Baud rate to 115200

//This is a list of Atoms. Should they be static or constant or something?
//bond type, diatomic, valence shell, name, atomic number
//Bond type 0 = ionic
//Diatomic 0 = false
Atom* H = {1, 1, {1, 0, -1, -1, -1, -1, -1, -1}, {'H',' '}, 1};
Atom* He = {1, 0, {1, 1, -1, -1, -1, -1, -1, -1}, {'H', 'e'}, 2};
Atom* Li = {0, 0, {1, 0, 0, 0, 0, 0, 0, 0}, {'L','i'}, 3};
Atom* Be = {0, 0, {1, 1, 0, 0, 0, 0, 0, 0}, {'B','e'}, 4};
	


//arSize is the size of both arrays (they're the same length). atomArray is to be returned. numArray is the array of atomic numbers that must be converted to an array of Atom structs.
vector initAtoms(int vectorSize, vector atomicNums)
{
	vector atomVector;
	vector_init(&atomVector);
	for(int i = 0; i < vectorSize; i++)
	{
		int atomicNum = vector_get(&atomicNums, i);
		switch(atomicNum){
			case 1:
			{
				//Atom* H = {1, 1, {1, 0, -1, -1, -1, -1, -1, -1}, {'H',' '}, 1};
				vector_add(&atomVector, H);
				break;
			}
			case 2:
			{
				//Atom* He = {1, 0, {1, 1, -1, -1, -1, -1, -1, -1}, {'H', 'e'}, 2};
				vector_add(&atomVector, He);
				break;
			}
			case 3:
			{
				//Atom* Li = {0, 0, {1, 0, 0, 0, 0, 0, 0, 0}, {'L','i'}, 3};
				vector_add(&atomVector, Li);
				break;
			}
			case 4:
			{
				//Atom* Be = {0, 0, {1, 1, 0, 0, 0, 0, 0, 0}, {'B','e'}, 4};
				vector_add(&atomVector, Be);
				break;
			}
			default:
				printf("No such element");
		}
	

		//printf("Not enough elements exist or I didn't initialize enough. Here's what we've got. \n");
		return atomVector;
	}
}


/*
 * Any code in this function will be run once, when the robot starts.
 */
void init()
{

	set_rgb(10, 10, 255); //this is a test line
	//Eventually, this vector should be created by user input. It has the atomic numbers of all the atoms we want to make.	
	vector atomicNums;
	int one = 1;
	int two = 2;
	int three = 3;
	int four = 4;
	vector_init(&atomicNums);
	vector_add(&atomicNums, &one);
	vector_add(&atomicNums, &two);
	vector_add(&atomicNums, &three);
	vector_add(&atomicNums, &four);
	int vectorSize = vector_count(&atomicNums);
	
	//Convert vector of atomic numbers to vector of Atom structs
	vector atomVector;
	atomVector = initAtoms(vectorSize, atomicNums);
	for(int i = 0; i < vectorSize; i++)
	{
		//vector_get returns *void. To typecast to *Atom, use (Atom*)vector_get(stuff)
		printf("atomArray[i].name = %d \n", ((Atom*)vector_get(&atomVector, i))->atomicNum);
	}
	
}

/*
 * The code in this function will be called repeatedly, as fast as it can execute.
 */
void loop()
{
	//set_rgb(255, 255, 255);
}

/*
 * After each pass through loop(), the robot checks for all messages it has 
 * received, and calls this function once for each message.
 */
void handle_msg(ir_msg* msg_struct)
{
	
}

