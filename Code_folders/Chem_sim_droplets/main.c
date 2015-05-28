/*
 * Created: 5/28/2015 1:11:07 PM
 *  Author: Audrey
 */ 
#include "droplet_programs/test_code.h"
#include "Atom.h"
#include "stdbool.h"


//control+k then c will comment everything highlighted. control+k then u uncomments

Atom* initAtoms(int numAtoms, Atom* atomArray)
{
	//bond type, diatomic, valence shell, name
	//Bond type 0 = ionic
	//Diatomic 0 = false
	
	int index = 0;
	Atom H = {1, 1, {1, 0, -1, -1, -1, -1, -1, -1}, "H", 1};
	atomArray[index] = H;
	if(index < numAtoms) index = index + 1;
	else return atomArray;
	Atom He = {1, 0, {1, 1, -1, -1, -1, -1, -1, -1}, "He", 2};
	atomArray[index] = He;
	if(index < numAtoms) index = index + 1;
	else return atomArray;
	Atom Li = {0, 0, {1, 0, 0, 0, 0, 0, 0, 0}, "Li", 3};
	atomArray[index] = Li;
	if(index < numAtoms) index = index + 1;
	else return atomArray;
	Atom Be = {0, 0, {1, 1, 0, 0, 0, 0, 0, 0}, "Be", 4};
	atomArray[index] = Be;
	if(index < numAtoms) index = index + 1;
	else return atomArray;

	printf("Not enough elements exist or I didn't initialize enough. Here's what we've got.");
	return atomArray;
}


/*
 * Any code in this function will be run once, when the robot starts.
 */
void init()
{
	uint8_t test = 255;
	int numAtoms = 3; //this should eventually be user input
	Atom atomArray[numAtoms]; //does this belong here? Should it be global?
	initAtoms(3, atomArray);
	printf("atomArray[0].name = ", atomArray[0].name);
}

/*
 * The code in this function will be called repeatedly, as fast as it can execute.
 */
void loop()
{
	
}

/*
 * After each pass through loop(), the robot checks for all messages it has 
 * received, and calls this function once for each message.
 */
void handle_msg(ir_msg* msg_struct)
{
	
}

