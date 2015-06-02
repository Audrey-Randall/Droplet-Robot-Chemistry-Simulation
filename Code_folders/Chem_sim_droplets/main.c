#include "droplet_programs/test_code.h"
#include "Atom.h"
#include "stdbool.h"
#include <stdio.h>
#include "vector.h"

int MY_CHEM_ID = 1; //This will eventually be written to the file by writeFiles.c!

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
Atom H = {1, 1, {1, 0, -1, -1, -1, -1, -1, -1}, "H", 1, {1, 1, 1, 0, -1,-1, -1, -1, -1, -1, 'H', ' ', 1}};
Atom He = {1, 0, {1, 1, -1, -1, -1, -1, -1, -1}, "He", 2, {1, 0, 1, 1, -1, -1, -1, -1, -1, -1, 'H', 'e', 2}};
Atom Li = {0, 0, {1, 0, 0, 0, 0, 0, 0, 0}, "Li", 3, {0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'L', 'i', 3,}};
Atom Be = {0, 0, {1, 1, 0, 0, 0, 0, 0, 0}, "Be", 4, {0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 'B', 'e', 4,}};
	


Atom getAtomFromAtomicNum(int atomicNum)
{
	switch(atomicNum){
		case 1:
		{
			//printf("case 1");
			return H;
		}
		case 2:
		{
			//printf("case 2");
			return He;
		}
		case 3:
		{
			//printf("case 3");
			return Li;
		}
		case 4:
		{
			//printf("case 4");
			return Be;
		}
		default:
			printf("No such element");
	}
}

void loadSingleChemID(Atom ID)
{
	//Somehow store the Atom struct on the Droplet
}

void sendChemID()
{
	//send the character array associated with this atom to all nearby droplets?
}

vector recieveAllChemIDs()
{
	//no idea how to set this up, but somehow I want to return a vector of character arrays.
}

void react(vector AllChemIDs)
{
	//recieve the character arrays. Then do stuff!
	
}

/*
 * Any code in this function will be run once, when the robot starts.
 */
void init()
{

	set_rgb(10, 10, 255); //this is a test line
	Atom myID = getAtomFromAtomicNum(MY_CHEM_ID);
	printf("this.name = %s \n", myID.name);
}

/*
 * The code in this function will be called repeatedly, as fast as it can execute.
 */
void loop()
{
	//set_rgb(255, 255, 255);
	//wait for half a second?
}

/*
 * After each pass through loop(), the robot checks for all messages it has 
 * received, and calls this function once for each message.
 */
void handle_msg(ir_msg* msg_struct)
{
	//react();
}

