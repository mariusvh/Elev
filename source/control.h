/**
* @file
* @brief Operations to control the elevators attributes.
*/

#ifndef CONTROL_H
#define CONTROL_H


/**
 * @brief Tracks which floor the elevator e is currently on.
 * @param[in,out] e The elevator that is beeing tracked.
 */
void control_update_floor(elevator_t *e);



/**
 * @brief Changes motor direction of the elevator when it reaches the top/bottom floor.
 */
void control_secure_range();


/**
 * @brief Changes motor direction of the elevator, updates the elevators new direction, e.dir, and its previous direction e.prevDir.
 * @param[in, out] e The elevator.
 * @param[in] dirn The elevators new motor direction.
 */
void control_update_direction(elevator_t *e, elev_motor_direction_t dirn);


/**
  Preparing elevator to start.Initializes elevator attributes.
  @param e The elevator.
*/
void control_elev_defined_start(elevator_t *e) ;


/**
 * @brief Checks if e.queue contains orders at the different buttons.
 * Sets button-lamps on or off.
 * @param[in, out] e The elevator.
 */
void control_update_button_lamps(elevator_t *e);


/**
 * @brief Checks if the elevator is between two floors.
 * Chooses next direction depending on previous direction
 * or indicated current floor.
 * @param[in, out] e The elevator.
 * @return @c DIRN_UP, @c DIRN_DOWN or @c DIRN_STOP.
 */
elev_motor_direction_t control_floor_inbetween(elevator_t *e);


/**
 * @brief Chooses the next direction of the elevator,
 * depending on previous direction and next orders.
 * @param[in] e The elevator.
 * @return @c DIRN_UP, @c DIRN_DOWN or @c DIRN_STOP.
 */
elev_motor_direction_t control_choose_direction(elevator_t *e, int value);


/**
 * @brief Checks if the elevator has orders in e.queue to execute at current floor, e.floor.
 * @param[in] e The elevator.
 * @return 1 if @c e.queue has orders to stop at @c e.floor, 0 otherwise.
 */
int control_stop_at_floor(elevator_t *e);




#endif
