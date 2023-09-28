#include <stdlib.h>
#include "hardware.h"
#include "lights.h"
#include "definitions.h"

void clear_all_lights() {
	for (int i = 0; i < HARDWARE_NUMBER_OF_FLOORS; i++){
		hardware_command_order_light(i, HARDWARE_ORDER_UP, 0);
	}
	for (int i = 0; i < HARDWARE_NUMBER_OF_FLOORS; i++){
		hardware_command_order_light(i, HARDWARE_ORDER_DOWN, 0);
	}
	for (int i = 0; i < HARDWARE_NUMBER_OF_FLOORS; i++){
		hardware_command_order_light(i, HARDWARE_ORDER_INSIDE, 0);
	}
}

