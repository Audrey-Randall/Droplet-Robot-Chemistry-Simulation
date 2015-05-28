/*
 * Created: 5/28/2015 1:11:07 PM
 *  Author: Audrey
 */ 
#include "droplet_programs/test_code.h"
#include "Atom.h"

//control+k then c will comment everything highlighted. control+k then u uncomments

/*
 * Any code in this function will be run once, when the robot starts.
 */
void init()
{
	uint8_t test = 255;
	printf("Hello.");
	//set_rgb(255,255,255);
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
