#include "main.h"

uint8_t MY_CHEM_ID; //This will eventually be written to the file by writeFiles.c!
char* global_Atom_str;

//This is a list of Atoms. Should they be static or constant or something?
//valence shell, name, bond type, diatomic, atomic number
//Bond type 0 = ionic, and atoms like the halides that bond with hydrogen and themselves (to form diatomics) are labelled ionic.
//Diatomic 0 = false
Atom H =  {{1, 0, -1, -1, -1, -1, -1, -1}, "H",1, 1, 1};
Atom He = {{1, 1, -1, -1, -1, -1, -1, -1}, "He", 1, 0, 2};
Atom Li = {{1, 0, 0, 0, 0, 0, 0, 0}, "Li", 0, 0, 3};
Atom Be = {{1, 1, 0, 0, 0, 0, 0, 0}, "Be", 0, 0, 4};
Atom C =  {{1, 1, 1, 1, 0, 0, 0, 0}, "C", 1, 0, 6};
Atom N =  {{1, 1, 1, 1, 1, 0, 0, 0}, "N", 1, 1, 7};
Atom O =  {{1, 1, 1, 1, 1, 1, 0, 0}, "O", 1, 1, 8};
Atom F =  {{1, 1, 1, 1, 1, 1, 1, 0}, "F", 0, 1, 9};
Atom Na = {{1, 0, 0, 0, 0, 0, 0, 0}, "Na", 0, 0, 11};
Atom Mg = {{1, 1, 0, 0, 0, 0, 0, 0}, "Mg", 0, 0, 12};
Atom Cl = {{1, 1, 1, 1, 1, 1, 1, 0}, "Cl", 0, 1, 17};
Atom Br = {{1, 1, 1, 1, 1, 1, 1, 0}, "Br", 0, 1, 35};
Atom I =  {{1, 1, 1, 1, 1, 1, 1, 0}, "I", 0, 1, 53};


void print_near_atoms()
{
	printf("PRINTING NEAR_ATOMS: \r\n");
	for(uint8_t i = 0; i < 12; i++)
	{
		if(near_atoms[i].id==0) break;
		printf("Atom: %s  Rng: %hu ID: %04X \r\n", near_atoms[i].atom.name, near_atoms[i].range, near_atoms[i].id);
	}
	
	printf("\r\n");
	printf("\r\n");
	printf("\t Bonded droplets: \r\n");
	for(uint8_t i = 0; i < 6; i++)
	{
		printf("\t %04X \r\n", bonded_atoms[i]);
		//if(bonded_atoms[i] == 0) break;
	}
}

void add_to_near_atoms(Near_Atom near_atom)
{
	uint8_t isSpace = 0;
	for(uint8_t i = 0; i < 12; i++)
	{
		if(near_atoms[i].id == 0) 
		{
			near_atoms[i] = near_atom; //This is probably okay but if the data structure is getting corrupted take a look at what exactly changes when you make this assignment.
			isSpace = 1;
			printf("Adding %s \r\n", near_atom.atom.name);
			break;
		}
	}	
	if (isSpace == 0) printf("No space to add another Droplet \r\n");
}

void update_near_atoms()
{
	for(uint8_t i = 0; i < 12; i++)
	{
		if(near_atoms[i].id != 0 && near_atoms[i].last_msg_t > 10000) //assuming this is in ms
		{
			near_atoms[i] = NULL_NEAR_ATOM;
			printf("Removing %c \r\n", near_atoms[i].atom.name);
		}
		else if(near_atoms[i].id != 0)
		{
			near_atoms->last_msg_t += 100; //assuming update is called every 100 ms
		}
	}
	schedule_task(300, update_near_atoms, NULL);
}

//Returns a 3-char array containing [deltaH of formation, deltaG of formation, and S naught].
//In other words, [enthalpy, gibbs free energy, entropy]
//Note that this is for individual atoms, not diatomics. Case 1 returns for H, not H2
//Units are kJ/mol, except for S, which is in kJ/mol*K
float* getThermoInfo(uint8_t atomicNum, uint8_t phase, uint8_t diatomic)
{
	float thermo[3];
	
	if(diatomic == 0) //get the monotomic numbers
	{
		if(phase == 1) //element is a gas
		{
			switch(atomicNum){
				case 1:
				{
					thermo = {218.0, 203.30, 0.11460};
					return thermo;
				}
				case 2:
				{
					//This is oversimplified, aka, wrong. Pending more research on Gibbs free energy and noble gases, it'll have to do.
					thermo = {0, -1.000, 0.1262};
					return thermo;
				}
				case 3:
				{
					thermo = {161.0, 128.0, 0.13867};
					return thermo;
				}
				case 4:
				{
					thermo = {-1.000, -1.000, -1.000};
					return thermo;
				}
				case 6:
				{
					thermo = {715.0, 669.6, 0.1580};
					return thermo;
				}
				case 7:
				{
					thermo = {473.0, 456.0, 0.1532};
					return thermo;
				}
				case 8:
				{
					thermo = {249.2, 231.7, 0.16095};
					return thermo;
				}
				case 9:
				{
					thermo = {78.9, 61.8, 0.15864};
					return thermo;
				}
				case 17:
				{
					thermo = {121.0, 105.0, 0.1651};
					return thermo;
				}
				case 35:
				{
					thermo = {111.9, 82.40, 0.17490};
					return thermo;	
				}
				case 53:
				{
					thermo = {106.8, 70.21, 0.18067};
					return thermo;
				}
				default:
					printf("No such element");
					return {-1.0, -1.0, -1.0};
			}
		}
	
		else if (phase == 2) //element is aqueous, which means it probably has a formal charge != 0. The most common formal charge has been assumed.
		{
			switch(atomicNum){
				case 1: //H+
				{
					thermo = {0, 0, 0};
					return thermo;
				}
				case 3: //Li+
				{
					thermo = {-278.46, -293.8, 0.014};
					return thermo;
				}
				case 9: //F-
				{
					thermo = {-329.1, -276.5, -0.0096};
					return thermo;
				}
				case 17: //Cl-
				{
					thermo = {-167.46, -131.17, 0.05510};
					return thermo;
				}
				case 35: //Br-
				{
					thermo = {-120.9,  -102.82, 0.08071};
					return thermo;	
				}
				case 53: //I-
				{
					thermo = {-55.94, -51.67, 0.1094};
					return thermo;
				}
				default:
					printf("No such element in that state");
					return {-1.0, -1.0, -1.0};
		}
		else if(phase == 3) //element is solid
		{
			switch(atomicNum){
				case 3: 
				{
					thermo = {0, 0, 0.02910};
					return thermo;
				}
		}
	}
	else //get the diatomic numbers
	{
		switch(atomicNum){
			case 1: //H2
			{
				thermo = {0, 0, 0.1306};
				return thermo;
			}
			case 7: //N2
			{
				thermo = {0, 0, 0.1915};
				return thermo;
			}
			case 8: //O2
			{
				thermo = {0, 0, 0.2050};
				return thermo;
			}
			case 9: //F2
			{
				thermo = {0, 0, 0.2027};
				return thermo;
			}
			case 17: //Cl2
			{
				thermo = {0, 0, 0.2230};
				return thermo;
			}
			case 35: //Br2
			{
				if(phase == 2) thermo = {0, 0, 0.15223};
				else thermo = {30.91, 3.13, 0.24538};
				return thermo;	
			}
			case 53: //I2
			{
				if(phase == 3) thermo = {0, 0, 0.11614};
				else thermo = {62.442, 19.38, 0.26058};
				return thermo;
			}
			default:
				printf("No such element in that state");
				return {-1.0, -1.0, -1.0};
	}
}

Atom getAtomFromAtomicNum(uint8_t atomicNum)
{
	switch(atomicNum){
		case 1:
		{
			//printf("case 1");
			set_rgb(255, 200, 0); //yellow
			return H;
		}
		case 2:
		{
			//printf("case 2");
			set_rgb(255, 50, 0); //orange
			return He;
		}
		case 3:
		{
			//printf("case 3");
			set_rgb(100, 100, 255); //ice white
			return Li;
		}
		case 4:
		{
			//printf("case 4");
			set_rgb(100, 255, 100); //ice green
			return Be;
		}
		case 6:
		{
			set_rgb(100, 0, 255); //indigo
			return C;
		}
		case 7:
		{
			set_rgb(200, 10, 10); //red-pink
			return N;
		}
		case 8:
		{
			set_rgb(0, 0, 255); //blue
			return O;
		}
		//All halogens are green
		case 9:
		{
			set_rgb(0, 255, 0); //green
			return F;
		}
		case 17:
		{
			set_rgb(0, 255, 0); //green
			return Cl;
		}
		case 35:
		{
			set_rgb(0, 255, 0); //green
			return Br;
		}
		case 53:
		{
			set_rgb(0, 255, 0); //green
			return I;
		}
		default:
			printf("No such element");
	}
}

void found_bond_routine()
{
	//printf("IN FOUND_DIATOMIC_ROUTINE \r\n");
	set_rgb(255, 0, 0);
	delay_ms(300);
	setAtomColor(myID);
	delay_ms(300);
	set_rgb(255, 0, 0);
	delay_ms(300);
	setAtomColor(myID);
}

void setAtomColor(Atom ID)
{
	uint8_t atomicNum = ID.atomicNum;
	switch(atomicNum){
		case 1:
		{
			//printf("case 1");
			set_rgb(255, 200, 0); //yellow
			break;
		}
		case 2:
		{
			//printf("case 2");
			set_rgb(255, 50, 0); //orange
			break;
		}
		case 3:
		{
			//printf("case 3");
			set_rgb(100, 100, 255); //purple
			break;
		}
		case 4:
		{
			//printf("case 4");
			set_rgb(100, 255, 100); //green
			break;
		}
		case 6:
		{
			set_rgb(100, 0, 255); //indigo
			break;
		}
		case 7:
		{
			set_rgb(200, 10, 10); //red-pink
			break;
		}
		case 8:
		{
			set_rgb(0, 0, 255); //blue
			break;
		}
		//All halogens are green
		case 9:
		{
			set_rgb(0, 255, 0); //green 
			break;
		}
		case 17:
		{
			set_rgb(0, 255, 0); //green
			break;
		}
		case 35:
		{
			set_rgb(0, 255, 0); //green
			break;
		}
		case 53:
		{
			set_rgb(0, 255, 0); //green
			break;
		}
		default:
		printf("No such element");
	}
}

void broadcastChemID(Atom ID)
{
	//send the character array associated with this atom to all nearby droplets
	//For now, it needs to go to every droplet on the board. Later, possibly change that.
	//global_Atom_str = (char*)(&ID);
	printf("broadcastChemID called");
	uint8_t r=get_red_led(), g=get_green_led(), b=get_blue_led();
	set_rgb(255,255,255);	
	ir_send(ALL_DIRS, (char*)(&ID), sizeof(Atom));
	delay_ms(100);
	set_rgb(r,g,b);
	//printf("sending chem ID \r\n");
}

void sendChemID(Atom ID, uint8_t channels[])
{
	global_Atom_str = (char*)(&ID);
	ir_send(channels[0], (char*)(&ID), sizeof(Atom));
	ir_send(channels[1], (char*)(&ID), sizeof(Atom));
}

uint8_t valenceFull()
{
	for(uint8_t i = 0; i < 8; i++)
	{
		if(myID.valence[i] == 0) return 0; //false
	}
	return 1; //true
}

void detectOtherDroplets()
{
	uint16_t received_id;
	float received_range;
	float received_bearing;
	float received_heading;
	uint8_t dir_mask = check_collisions();
	//printf("Collisions: %02hX \r\n", dir_mask);
	
	if(rnb_updated)
	{
		received_id = last_good_rnb.id_number;
		received_range = last_good_rnb.range;
		received_bearing = last_good_rnb.bearing;
		received_heading = last_good_rnb.heading;
		//convert to degrees from radians
		received_bearing = rad_to_deg(received_bearing);
		received_heading = rad_to_deg(received_heading);
		//scaling the range to mm.
		received_range = received_range*10;
		printf("range: %f\r\n", received_range);
		
		for(uint8_t i = 0; i < 12; i++)
		{
			if(near_atoms[i].id == received_id)
			{
				near_atoms[i].bearing = (int16_t)received_bearing;
				near_atoms[i].heading = (int16_t)received_heading;
				near_atoms[i].range = (uint8_t)received_range;
			}
		}
		print_near_atoms();
		broadcastChemID(myID);
		rnb_updated=0;
	}
}

//Checks to see what kind of bonds the newly discovered near_atom can make with me
uint8_t checkPossibleBonds(Atom near_atom, uint16_t senderID)
{
	unsigned char diatomic[9];
	uint8_t diatomicPossible = 1;
	
	uint8_t empty_spaces = 0;
	for(uint8_t i = 0; i < 8; i++)
	{
		if(near_atom.valence[i] == 0) empty_spaces = empty_spaces + 1;
		if(near_atom.valence[i] == 2)
		{
			printf("This atom is already bonded to something, it can't form a diatomic bond");
			diatomicPossible = 0;
		}
	}
	
	//Diatomic bond?
	if(near_atom.diatomic == 1 && near_atom.atomicNum == myID.atomicNum && VALENCE_FULL == 0 && diatomicPossible == 1)
	{
		printf("\tEntered diatomic if statement.\r\n");
		diatomic[0] = 'd';
		uint8_t empty_slot_counter = empty_spaces;
		uint8_t free_electron_counter = empty_spaces;
		for(uint8_t i = 1; i < 9; i++)
		{
			printf("near_atom.valence[i] = %hd \r\n ", near_atom.valence[i-1]);
			if(near_atom.valence[i-1] == 0 && empty_slot_counter > 0)
			{
				diatomic[i] = 2;
				empty_slot_counter = empty_slot_counter - 1;
			}
			else if(near_atom.valence[i-1] == 1 && free_electron_counter > 0)
			{
				diatomic[i] = 2;
				free_electron_counter = free_electron_counter - 1;
			}
			else diatomic[i] = near_atom.valence[i-1];
		}
		printf("At the end of the diatomic statement in checkPossibleBonds, valence is: ");
		//printValence(diatomic);
		for(uint8_t i=0; i<9; i++) printf(" %hd ", (int8_t)diatomic[i]); 
		printf("\r\n");
		found_bond_routine();
		VALENCE_FULL = 1;
		for(uint8_t i = 0; i < 6; i++)
		{
			if(bonded_atoms[i] == 0 || bonded_atoms[i] == senderID)
			{
				bonded_atoms[i] = senderID;
				break;
			}
		}
		ir_targeted_send(ALL_DIRS, diatomic, 9, senderID);
		return 1;
	}

}

void printValence(int8_t valence[])
{
	for(uint8_t i = 0; i < 8; i++)
	{
		printf(" %hd ", valence[i]);
	}
	printf("\r\n");
}

/*
 * Any code in this function will be run once, when the robot starts.
 */
void init()
{
	switch(get_droplet_id()){
		case 0xAFD8: MY_CHEM_ID = 3; break;
		case 0x3062: MY_CHEM_ID = 8; break;
		case 0xFA6F: MY_CHEM_ID = 6; break;
		case 0x6C6F: MY_CHEM_ID = 7; break;
		case 0xD86C: MY_CHEM_ID = 8; break;
		case 0xB36F: MY_CHEM_ID = 8; break;
		case 0x6B6F: MY_CHEM_ID = 17; break;
		case 0xBC6E: MY_CHEM_ID = 17; break;
		case 0x46A1: MY_CHEM_ID = 4; break;
		default:     MY_CHEM_ID = 1; break;
	}
	
	VALENCE_FULL = 0;
	for(uint8_t i = 0; i < 12; i++)
	{
		near_atoms[i] = NULL_NEAR_ATOM;
	}
	//set_rgb(10, 255, 255); //this is a test line
	myID = getAtomFromAtomicNum(MY_CHEM_ID);
	update_near_atoms();
}

/*
 * The code in this function will be called repeatedly, as fast as it can execute.
 */
void loop()
{
	delay_ms(500);
	detectOtherDroplets();
	uint32_t time_floor = ((get_time()/500)*500);
	if(time_floor%4000==0){
		printf("sent bonded_atoms\r\n");
		broadcast_rnb_data();
		ir_send(ALL_DIRS, bonded_atoms, 12); //Should this be here or inside the 5 second loop? Also, do I have the last parameter right? 12 bytes?
	}
	
}

////this recursively schedules itself to happen every RNB_BROADCAST_PERIOD, which is defined in main.h. It broadcasts my range and bearing (rnb) data.
//void periodic_rnb_broadcast()
//{
	////printf("periodic_rnb_broadcast \r\n");
	//broadcast_rnb_data();
	//schedule_task(RNB_BROADCAST_PERIOD, periodic_rnb_broadcast, NULL);
//}

/*
 * After each pass through loop(), the robot checks for all messages it has 
 * received, and calls this function once for each message.
 */
void handle_msg(ir_msg* msg_struct)
{
	uint8_t r=get_red_led(), g=get_green_led(), b=get_blue_led();
	set_rgb(0,0,0);
	delay_ms(50);
	Atom* near_atom;
	
	//Message is an Atom struct
	if(msg_struct->length==sizeof(Atom))
	{
		printf("Received atom struct \r\n");
		near_atom = (Atom*)(msg_struct->msg); //do some kind of check to make sure this is actually an atom.
		//printf("\tin handle_msg, near_atom.name = %c%c \r\n", near_atom->name[0], near_atom->name[1]);
		//printf("Printing near_atom.valence before modification: ");
		//printValence(near_atom->valence);
		
		//If this droplet isn't in our list, add it. If it is, update its last_msg_t to 0.
		uint8_t found = 0;
		for(uint8_t i = 0; i < 12; i++)
		{
			//printf("near_atoms[i].id = %u and msg_struct.sender_id is %u \r\n", near_atoms[i].id, msg_struct->sender_ID);
			if(near_atoms[i].id ==msg_struct->sender_ID)
			{
				near_atoms[i].last_msg_t = 0;
				found = 1;
				break;
			}
		}
		if (found == 0) //add new droplet to near_atoms
		{
			Near_Atom close_atom = {*near_atom, msg_struct->sender_ID, 0, 0, 0, 0, 0};
			//printf("\t\tMsg_struct.sender_id is: %u and close_atom.id is: %u \r\n", msg_struct->sender_ID, close_atom.id);
			uint8_t isBonded = checkPossibleBonds(*near_atom, msg_struct->sender_ID);
			if(isBonded) close_atom.bonded = 1;
			add_to_near_atoms(close_atom);
		}
	}
	
	//Message is that a diatomic bond was formed
	else if(msg_struct->msg[0] == 'd')
	{
		printf("Got 'bond found' message.\r\n");
		found_bond_routine();
		VALENCE_FULL = 1;
		for(uint8_t i = 0; i < 8; i++)
		{
			myID.valence[i] = msg_struct->msg[i+1];
		}
		printf("After receiving the 'bond found' message, valence is: ");
		printValence(myID.valence);
		for(uint8_t i = 0; i < 6; i++)
		{
			if(bonded_atoms[i] == 0 || bonded_atoms[i] == msg_struct->sender_ID)
			{
				bonded_atoms[i] = msg_struct->sender_ID;
				break;
			}
		}
	}
	
	//Message is another Droplet's bonded_atoms array
	else if(msg_struct->length == sizeof(bonded_atoms))
	{
		//printf("Received bonded_atoms \r\n");
		//How does the code know this is an array of uint16_ts? Can I iterate through it like usual?
		//Check to see if I'm bonded to the droplet who just sent me his bonded_atoms array
		uint16_t senderIDFound = 0;
		uint8_t i;
		for(i = 0; i < 6; i++)
		{
			if(bonded_atoms[i] == msg_struct->sender_ID)
			{
				senderIDFound = 1;
				break;
			}
		}
		//If so, check to see if he's bonded to me. If he isn't, break my bond.
		if(senderIDFound == 1)
		{
			uint8_t myIdFound = 0;
			for(uint8_t j = 0; j < 6; j++)
			{
				if(msg_struct->msg[j] == get_droplet_id())
				{
					myIdFound = 1;
					break;
				}
			}
			if(myIdFound == 0)
			{
				printf("I think I'm bonded to a droplet who doesn't think he's bonded to me. I'm breaking that bond. \r\n");
				//Remove other droplet from bonded_atoms and remove the bonded flag from near_atoms
				bonded_atoms[i] = 0;
				for(uint8_t k = 0; k < 12; k++)
				{
					if(near_atoms[k].id == msg_struct->sender_ID) 
					{
						near_atoms[k].bonded = 0;
						break;
					}
				}
			}
		}
		
	}
	set_rgb(r,g,b);
}



