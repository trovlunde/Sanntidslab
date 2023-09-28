
#include "definitions.h"
#include "hardware.h"

int queue[HARDWARE_NUMBER_OF_FLOORS][3]; 

int current_floor;

int last_valid_floor;

float previous_floor;

valid_states state;

valid_states previous_state;
