#include <stdio.h>
#include <stdlib.h>
#include "hardware.h"
#include "lights.h"
#include "queue.h"
#include "door.h"
#include "definitions.h"


void boot_control() {
	int error = hardware_init();
	if (error != 0) {
		fprintf(stderr, "Unable to initialize hardware\n");
		exit(1);
	}
	while (1) {
		for (int floor = 0; floor < HARDWARE_NUMBER_OF_FLOORS; floor++) {
			if (hardware_read_floor_sensor(floor)) {
				hardware_command_movement(HARDWARE_MOVEMENT_STOP);
				hardware_command_floor_indicator_on(floor);
				current_floor = floor;
				state = IDLE;
				return;
			}
		}
		hardware_command_movement(HARDWARE_MOVEMENT_DOWN);
	}
}

int check_for_floor() {
	if (!(current_floor == -1)) {
		return 1;
	}
	return 0;
}

void stop_button() {
	hardware_command_stop_light(1);
	hardware_command_movement(HARDWARE_MOVEMENT_STOP);
	remove_all_orders();
	clear_all_lights();
	if (check_for_floor()) {
		hardware_command_door_open(1);
	}
}

void update_current_floor(int *cf) {
	for (int floor = 0; floor < HARDWARE_NUMBER_OF_FLOORS; floor++) {
		if (hardware_read_floor_sensor(floor)) {
			*cf = floor;
			break;
		}
		else {
			*cf = -1;
		}
	}
}

void update_previous_floor(int *lvf, float *pf) {
	if (check_for_floor()) {
		for (int floor = 0; floor < HARDWARE_NUMBER_OF_FLOORS; floor++) {
			if (hardware_read_floor_sensor(floor)) {
				*lvf = floor;
				*pf = *lvf;
			}
		}
	}
	else {
		if (state == MOVING_UP) {
			*pf = *lvf + 0.5;
		}

		else if (state == MOVING_DOWN) {
			*pf = *lvf - 0.5;
		}
	}
}

	
void state_handler() {
	switch (state) {
	case IDLE:
		if (check_for_orders()) {
			
			if ((previous_floor - find_order_floor()) < 0) {
				state = MOVING_UP;
			}
			else if ((previous_floor - find_order_floor()) == 0){
			state = OPEN;
				}
			else {
				state = MOVING_DOWN;
			}
		}
		else {
			state = IDLE;
		}
		break;

	case MOVING_UP:
		hardware_command_movement(HARDWARE_MOVEMENT_UP);
		if (check_for_floor()) {
			if ((queue[current_floor][0]) || (queue[current_floor][2]) || !(check_for_order_above(current_floor))) {
				state = OPEN;
					previous_state = MOVING_UP;
			}
			else {
				state = MOVING_UP;
			}
		}
		break;

	case MOVING_DOWN:
		hardware_command_movement(HARDWARE_MOVEMENT_DOWN);
		if (check_for_floor()) {
			if ((queue[current_floor][1]) || (queue[current_floor][2]) || !(check_for_order_below(current_floor))) {
				state = OPEN;
					previous_state = MOVING_DOWN;
			}
			else {
				state = MOVING_DOWN;
			}
		}
		break;

	case OPEN:
		hardware_command_movement(HARDWARE_MOVEMENT_STOP);
		door_open();
		remove_order(current_floor);
		break;

	case EMERGENCY:
		stop_button();
		if (!(hardware_read_stop_signal())) {
			if (check_for_floor()) {
				state = OPEN;
			}
			else {
				state = IDLE;
			}
			previous_state = EMERGENCY;
			hardware_command_stop_light(0);
		}
		break;
	}
}
			


int main(){
	boot_control();
	while (1) {
		hardware_command_floor_indicator_on(last_valid_floor);
		if (hardware_read_stop_signal()) {
			state = EMERGENCY;
		}
		else {
			update_current_floor(&current_floor);
			update_previous_floor(&last_valid_floor, &previous_floor);
			update_queue_list();
		}
		state_handler();
	}
}
