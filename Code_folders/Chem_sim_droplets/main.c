#include "main.h"

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

char* global_Atom_str;

void sendChemID(Atom ID)
{
	//send the character array associated with this atom to all nearby droplets
	//For now, it needs to go to every droplet on the board. Later, possibly change that.
	global_Atom_str = (char*)(&ID);
	ir_send(ALL_DIRS, (char*)(&ID), sizeof(Atom));
	printf("sending chem ID \r\n");
}



/*
 * Any code in this function will be run once, when the robot starts.
 */
void init()
{

	set_rgb(10, 255, 255); //this is a test line
	myID = getAtomFromAtomicNum(MY_CHEM_ID);
	printf("this.name = %c%c \r\n", myID.name[0], myID.name[1]);
	periodic_rnb_broadcast();
}

/*
 * The code in this function will be called repeatedly, as fast as it can execute.
 */
void loop()
{
	//set_rgb(255, 255, 255);
	//why does the LED flicker once per loop?
	sendChemID(myID);
	delay_ms(1000);
	uint8_t recieving_id;
	
	uint8_t dir_mask = check_collisions();
	printf("Collisions: %02hX \r\n", dir_mask);
	if(rnb_updated)
	{
		recieving_id = last_good_rnb.id_number;
		printf("recieving_id = %u. \r\n");
		//last_good_rnb.range
		//last_good_rnb.bearing
		//last_good_rnb.heading
		
		rnb_updated=0;
	}
	//printf("LOOP \r\n");
}

//This is still hitting the buffer-full-delay-indefinitely error. It needs some sort of synchronization with the other Droplets so they don't send at the same time.
void periodic_rnb_broadcast()
{
	printf("periodic_rnb_broadcast \r\n");
	broadcast_rnb_data();
	schedule_task(RNB_BROADCAST_PERIOD, periodic_rnb_broadcast, NULL);
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

	printf("in handle_msg, near_atom.name = %c%c \r\n", near_atom->name[0], near_atom->name[1]);
	if(near_atom->diatomic == 1 && near_atom->atomicNum == myID.atomicNum)
	{
		printf("found diatomic partner \r\n");
		//walk(NORTH, 50);
		delay_ms(1000);
		stop();
	}
	printf("END OF HANDLE_MSG \r\n");
}

