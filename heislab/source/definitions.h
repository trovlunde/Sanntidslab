typedef enum {
	IDLE, MOVING_UP, MOVING_DOWN, OPEN, EMERGENCY
}valid_states;

extern int queue[][3];

extern int current_floor;

extern int last_valid_floor;

extern float previous_floor;

extern valid_states state;

extern valid_states previous_state;





