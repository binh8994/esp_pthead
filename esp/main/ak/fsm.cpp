#include "ak.h"
#include "message.h"

void fsm_dispatch(fsm_t* me, ak_msg_t* msg) {
	me->state(msg);
}
