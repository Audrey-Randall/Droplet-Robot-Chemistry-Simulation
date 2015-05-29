/*
 * Created: 5/28/2015 1:11:07 PM
 *  Author: Audrey
 */ 
#include "droplet_programs/test_code.h"
#include "Atom.h"
#include "stdbool.h"
#include <stdio.h>

#define FOSC 16000000
#define BAUD 115200
#define MYUBRR (((((FOSC * 10) / (16L * BAUD)) + 5) / 10) - 1)

//When using Tera Term, go to Setup -> Serial port and change Baud rate to 115200

//also makes printf work
int usart_putchar_printf(char var, FILE *stream) {
    if (var == '\n') usart_putchar('\r');
    usart_putchar(var);
    return 0;
}

static FILE mystdout = FDEV_SETUP_STREAM(usart_putchar_printf, NULL, _FDEV_SETUP_WRITE); //makes printf work?
//stdout = &mystdout;

//arSize is the size of both arrays (they're the same length). atomArray is to be returned. numArray is the array of atomic numbers that must be converted to an array of Atom structs.
Atom* initAtoms(int arSize, int* numArray)
{
	//bond type, diatomic, valence shell, name
	//Bond type 0 = ionic
	//Diatomic 0 = false
	static Atom atomArray[4];
	
	for(int i = 0; i < arSize; i++)
	{
		int atomicNum= numArray[i];
		switch(atomicNum){
			case 1:
			{
				Atom H = {1, 1, {1, 0, -1, -1, -1, -1, -1, -1}, "H", 1};
				atomArray[i] = H;
				break;
			}
			case 2:
			{
				Atom He = {1, 0, {1, 1, -1, -1, -1, -1, -1, -1}, "He", 2};
				atomArray[i] = He;
				break;
			}
			case 3:
			{
				Atom Li = {0, 0, {1, 0, 0, 0, 0, 0, 0, 0}, "Li", 3};
				atomArray[i] = Li;
				break;
			}
			case 4:
			{
				Atom Be = {0, 0, {1, 1, 0, 0, 0, 0, 0, 0}, "Be", 4};
				atomArray[i] = Be;
				break;
			}
			default:
				printf("No such element");
		}
	

		//printf("Not enough elements exist or I didn't initialize enough. Here's what we've got. \n");
		//return atomArray;
	}
}


/*
 * Any code in this function will be run once, when the robot starts.
 */
void init()
{
	//printf("test \n");
	set_rgb(10, 10, 255); //this is a test line
	//Eventually, this array should be created by user input. It's an array of the atomic numbers of all the atoms we want to make.	
	int numArray[] = {1, 2, 3, 4};
	int arSize = (sizeof(numArray)/sizeof(numArray[0]));
	Atom* atomArray;
	atomArray = initAtoms(arSize, numArray);
	for(int i = 0; i < arSize; i++)
	{
		printf("atomArray[i].name = %s \n", atomArray[i].name);
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

