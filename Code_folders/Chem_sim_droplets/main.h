#ifndef USER_TEMPLATE
#define USER_TEMPLATE

#include "droplet_init.h"
#include "Atom.h"
#include "vector.h"

#define RNB_BROADCAST_PERIOD 5000
#define UPDATE_ATOMS_PERIOD 100

typedef struct
{
	Atom atom;
	uint8_t range;
	uint8_t bearing;
	uint8_t heading;
	uint8_t last_msg_t; //time that this atom last sent a message to me
	uint8_t id;
}Near_Atom;

Near_Atom near_atoms[12]; //this number is pretty arbitrary.
Atom NULL_ATOM = {{0,0,0,0,0,0,0,0},{0,0},0,0,0};
Near_Atom NULL_NEAR_ATOM = {{{0,0,0,0,0,0,0,0},{0,0},0,0,0}, 0, 0, 0, 0, 0};
void init();
void loop();
void handle_msg(ir_msg* msg_struct);

/*void periodic_rnb_broadcast();
void add_to_near_atoms();
void update_near_atoms();
Atom getAtomFromAtomicNum(uint8_t atomicNum);
void found_diatomic_routine();
void setAtomColor(Atom ID);
void broadcastChemID(Atom ID);
void sendChemID(Atom ID, uint8_t channels[]);
void add_bond(uint8_t numBonds);
uint8_t valenceFull();
void detectOtherDroplets();
void periodic_rnb_broadcast();
void print_near_atoms()
*/

Atom myID;

#endif
