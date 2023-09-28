#include <stdlib.h>
#include "hardware.h"
#include "door.h"
#include "time.h"
#include "definitions.h"
#include "queue.h"

static int door_timer;


void door_timer_start() {
	door_timer = time(NULL);
}

int door_timer_expired() {
	if ((time(NULL) - door_timer) < DOOR_TIMER_BREAK) {
		return 1;
	}
	return 0;
}

void door_open() {
	door_timer_start();
	hardware_command_door_open(1);
	
	if (door_timer_expired() && !hardware_read_obstruction_signal()) {
		hardware_command_door_open(0);
		if (previous_state == MOVING_UP && check_for_compatible_order_above(current_floor)) {
			state = previous_state;
		}
		else if (previous_state == MOVING_DOWN && check_for_compatible_order_below(current_floor)) {
			state = previous_state;
		}
		else {
			state = IDLE;
		}
	}
}

