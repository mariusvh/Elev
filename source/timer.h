/**
* @file
* @brief Operations to hold the door open for the right amount of time.
*/

#ifndef TIMER_H
#define TIMER_H

// Number of seconds the door should be open.
#define DOOR_TIME 3



/**
 * @brief Sets @c e.time to start time.
 * @param[out] e The elevator.
 */
void timer_start(elevator_t *e);



/**
 * @brief Checks if @c e.time has increased with DOOR_TIME.
 * @param[in] e The elevator.
 * @return 1 if e.time exceed DOOR_TIME, 0 if not.
 */
int timer_check(elevator_t *e);


#endif
