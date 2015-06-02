#include "droplet_programs/test_code.h"
#include "Atom.h"
#include "vector.h"

uint8_t MY_CHEM_ID = 1; //This will eventually be written to the file by writeFiles.c!

//This is a list of Atoms. Should they be static or constant or something?
//bond type, diatomic, valence shell, name, atomic number
//Bond type 0 = ionic
//Diatomic 0 = false
Atom H =  {{1, 0, -1, -1, -1, -1, -1, -1}, "H",1, 1, 1};
Atom He = {{1, 1, -1, -1, -1, -1, -1, -1}, "He", 1, 0, 2};
Atom Li = {{1, 0, 0, 0, 0, 0, 0, 0}, "Li", 0, 0, 3};
Atom Be = {{1, 1, 0, 0, 0, 0, 0, 0}, "Be", 0, 0, 4};

Atom getAtomFromAtomicNum(uint8_t atomicNum)
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

void sendChemID(Atom myID)
{
	//send the character array associated with this atom to all nearby droplets? is this the right function?
	//For now, it needs to go to every droplet on the board. Later, possibly change that.
	/*
	for(uint8_t i = 0; i < (max number of directions); i++)
	{
		ir_send( (some function of i), myID.chemID, 13);
	}
	*/
	ir_send(ALL_DIRS, (char*)(&myID), sizeof(Atom));
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
void loop(Atom myID)
{
	//set_rgb(255, 255, 255);
	//wait for half a second?
	sendChemID(myID);
	delay_ms(1000);
}

/*
 * After each pass through loop(), the robot checks for all messages it has 
 * received, and calls this function once for each message.
 */
void handle_msg(ir_msg* msg_struct)
{
	Atom* near_atom;
	if(msg_struct->length==sizeof(Atom))
	{
		near_atom = (Atom*)(msg_struct->msg); //do some kind of check to make sure this is actually an atom.
	}

	printf("in handle_msg, near_atom.name = %s \n", near_atom->name);
}

