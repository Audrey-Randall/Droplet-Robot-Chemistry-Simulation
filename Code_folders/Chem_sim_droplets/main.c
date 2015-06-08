#include "main.h"

uint8_t MY_CHEM_ID; //This will eventually be written to the file by writeFiles.c!
char* global_Atom_str;

//This is a list of Atoms. Should they be static or constant or something?
//bond type, diatomic, valence shell, name, atomic number
//Bond type 0 = ionic
//Diatomic 0 = false
Atom H =  {{1, 0, -1, -1, -1, -1, -1, -1}, "H",1, 1, 1};
Atom He = {{1, 1, -1, -1, -1, -1, -1, -1}, "He", 1, 0, 2};
Atom Li = {{1, 0, 0, 0, 0, 0, 0, 0}, "Li", 0, 0, 3};
Atom Be = {{1, 1, 0, 0, 0, 0, 0, 0}, "Be", 0, 0, 4};

void print_near_atoms()
{
	printf("PRINTING NEAR_ATOMS:");
	for(uint8_t i = 0; i < 12; i++)
	{
		printf("Atom: %c  Rng: %u ID: %u \r\n", near_atoms[i].atom.name, near_atoms[i].range, near_atoms[i].id);
	}
	
	printf("\r\n");
}

void add_to_near_atoms(Near_Atom near_atom)
{
	uint8_t isSpace = 0;
	for(uint8_t i = 0; i < 12; i++)
	{
		if(near_atoms[i].id == 0) 
		{
			near_atoms[i] = near_atom;
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
	schedule_task(UPDATE_ATOMS_PERIOD, update_near_atoms, NULL);
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
			set_rgb(100, 0, 255); //purple
			return Li;
		}
		case 4:
		{
			//printf("case 4");
			set_rgb(0, 255, 0); //green
			return Be;
		}
		default:
			printf("No such element");
	}
}

void found_diatomic_routine()
{
	printf("IN FOUND_DIATOMIC_ROUTINE \r\n");
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
			set_rgb(100, 0, 255); //purple
			break;
		}
		case 4:
		{
			//printf("case 4");
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
	ir_send(ALL_DIRS, (char*)(&ID), sizeof(Atom));
	//printf("sending chem ID \r\n");
}

void sendChemID(Atom ID, uint8_t channels[])
{
	global_Atom_str = (char*)(&ID);
	ir_send(channels[0], (char*)(&ID), sizeof(Atom));
	ir_send(channels[1], (char*)(&ID), sizeof(Atom));
}

void add_bond(uint8_t numBonds)
{
	for(uint8_t i = 0; i < 8; i++)
	{
		if(myID.valence[i] == 1) 
		{
			myID.valence[i] = 2;
			numBonds--;
		}
		if (numBonds == 0) break;
	}
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
		//printf("range: %f\r\n", received_range);
		
		for(uint8_t i = 0; i < 12; i++)
		{
			if(near_atoms[i].id == received_id)
			{
				near_atoms[i].bearing = received_bearing;
				near_atoms[i].heading = received_heading;
				near_atoms[i].range = received_range;
			}
		}
		print_near_atoms();
		if(received_range <= 3) broadcastChemID(myID);
		//insert code to make droplets move toward each other here. For now, we'll move them by hand.
		/*//sendChemID to droplet # recieved_id. Send from two sensors nearest receiving_id droplet.
		//This is a very crude way of telling if a droplet nearby is the one sending the message. It needs to be replaced.
		if(received_bearing >= 0 && received_bearing < 60) 
		{
			USART_t channels[2] = {USARTC0, USARTC1};
			if(dir_mask == 1 || dir_mask == 2 || dir_mask == 3) sendChemID(myID, channels);
		}
		else if(received_bearing >= 60 && received_bearing < 120) 
		{
			USART_t channels[2] = {USARTC1, USARTD0};
			if(dir_mask == 2 || dir_mask == 4 || dir_mask == 6) sendChemID(myID, channels);
		}
		else if(received_bearing >= 120 && received_bearing < 180) 
		{
			USART_t channels[2] = {USARTD0, USARTE0};
			if(dir_mask == 4 || dir_mask == 8 || dir_mask == 12) sendChemID(myID, channels);
		}
		else if(received_bearing >= 180 && received_bearing < 240)
		{
			USART_t channels[2] = {USARTE0, USARTE1};
			if(dir_mask == 8 || dir_mask == 16 || dir_mask == 24) sendChemID(myID, channels);
		}
		else if(received_bearing >= 240 && received_bearing < 300) 
		{
			USART_t channels[2] = {USARTE1, USARTF0};
			if(dir_mask == 16 || dir_mask == 32 || dir_mask == 48) sendChemID(myID, channels);
		}
		else 
		{
			USART_t channels[2] = {USARTF0, USARTC0};
			if(dir_mask == 32 || dir_mask == 1 || dir_mask == 33) sendChemID(myID, channels);
		}
		*/
		rnb_updated=0;
	}
}

/*
 * Any code in this function will be run once, when the robot starts.
 */
void init()
{
	switch(get_droplet_id()){
		case 0xAFD8: MY_CHEM_ID = 3; break;
		case 0x3062: MY_CHEM_ID = 4; break;
		default:     MY_CHEM_ID = 1; break;
	}
	
	for(uint8_t i = 0; i < 12; i++)
	{
		near_atoms[i] = NULL_NEAR_ATOM;
	}
	//set_rgb(10, 255, 255); //this is a test line
	myID = getAtomFromAtomicNum(MY_CHEM_ID);
	update_near_atoms();
	periodic_rnb_broadcast();
}

/*
 * The code in this function will be called repeatedly, as fast as it can execute.
 */
void loop()
{
	delay_ms(500);
	detectOtherDroplets();
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
	//List of messages
	Atom* near_atom;
	char diatomic[9]; //message to send if diatomic found
	char changeValence[9]; //not initialized here because there's a LOT of things valence could do, we'll set it equal to functions later. Consists of a 'c' plus the new valence shell.
	
	for(uint8_t i = 0; i < 12; i++)
	{
		if(near_atoms[i].id == msg_struct->sender_ID)
		{
			near_atoms[i].last_msg_t = 0;
		}
	}
	
	//Message is an Atom struct
	if(msg_struct->length==sizeof(Atom))
	{
		near_atom = (Atom*)(msg_struct->msg); //do some kind of check to make sure this is actually an atom.
		printf("\tin handle_msg, near_atom.name = %c%c \r\n", near_atom->name[0], near_atom->name[1]);
		
		//If this droplet isn't in our list, add it. If it is, update its last_msg_t to 0.
		uint8_t found = 0;
		for(uint8_t i = 0; i < 12; i++)
		{
			if(near_atoms[i].id ==msg_struct->sender_ID)
			{
				near_atoms[i].last_msg_t = 0;
				found = 1;
				break;
			}
		}
		if (found == 0) //add new droplet to near_atoms
		{
			//This will (hopefully) cause the rnb data to be collected for this droplet the next time detectOtherDroplets is called, since it makes 
			// the droplet that sent me this struct broadcast its rnb data.
			//collect_rnb_data(msg_struct->sender_ID, 255); //what does power have to be?
			Near_Atom close_atom = {*near_atom, 0, 0, 0, 0, msg_struct->sender_ID};
			printf("Close_atom.atom.name is: %s and near_atom.name is: %s \r\n", close_atom.atom.name, near_atom->name);
			add_to_near_atoms(close_atom);
		}
		
		//Is the newly discovered atom diatomic? This should be its own function, checkPossibleBonds(Atom near_atom); and should include other bond checks
		if(near_atom->diatomic == 1 && near_atom->atomicNum == myID.atomicNum)
		{
			printf("\tDIATOMIC CRITERIA MET");
			found_diatomic_routine();
			ir_targeted_send(ALL_DIRS, diatomic, 2, msg_struct->sender_ID);
		}
	}
	
	//Message is that my diatomic partner was found
	else if((msg_struct->msg[0] == 'd') && (msg_struct->msg[1] ==  'i'))
	{
		printf("\tChar array 'diatomic' received\r\n");
		found_diatomic_routine();
	}
	
	//Message is to change my valence shell
}

