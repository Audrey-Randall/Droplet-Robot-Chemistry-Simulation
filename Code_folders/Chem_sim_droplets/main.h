#ifndef USER_TEMPLATE
#define USER_TEMPLATE

#include "droplet_init.h"
#include "Atom.h"
#include "vector.h"

#define RNB_BROADCAST_PERIOD 5000

void init();
void loop();
void handle_msg(ir_msg* msg_struct);

void periodic_rnb_broadcast();

Atom myID;

#endif
