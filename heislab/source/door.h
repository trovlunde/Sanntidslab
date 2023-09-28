/** @file
  * @brief Door control
  *
  * Modules for control of the door.
  */
#ifndef TIMER_H

#define TIMER_H
#define DOOR_TIMER_BREAK 3

#endif

/**
  * @brief Start the 3 second timer.
  */
void door_timer_start();

/**
  * @brief Check whether timer has expired.
  */
int door_timer_expire();

/**
  * @brief Opens the door for a fixed amount of time and decides next state for the machine. 
  */
void door_open();




