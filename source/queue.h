#include "elev.h"
#include <time.h>
/**
 * @file
 * @brief Operations to take care of the elevator orders. Defines the elevator, its states and its queue matrix.
 */

#ifndef QUEUE_H
#define QUEUE_H



/**
 * @brief The elevators different states.
 */
typedef enum states {
  IDLE, /**< When the elevator is in idle state.*/
  MOVE,  /**< When the elevator is moving.*/
  STOP,  /**< When the elevator is in emergency stop state.*/
  DOOR_OPEN  /**< When the elevator opens the door.*/
} state_t;



/**
 * @brief A structure to represent the elevator.
 */
typedef struct elevator{
  elev_motor_direction_t dir;  /**< The motor direction of the elevator.*/
  elev_motor_direction_t prevDir;  /**< The previous motor direction of the elevator.*/
  int floor;  /**< The floor the elevator is at. (Shows previous floor if elevator is moving.)*/
  state_t state;  /**< The state the elevator is located in. */
  int queue[N_FLOORS][N_BUTTONS];  /**< The queue, a matrix that contains the orders for the elevator.*/
  time_t time;  /**< Time variable used for timing the elevator door.*/
}elevator_t;



/**
 * @brief Resets all orders in e.queue. Sets unavailable orders to -1.
 * @param[in, out] e The elevator.
 */
void queue_reset_matrix(elevator_t *e);



/**
 * @brief Updates e.queue with new orders from elevator-panel.
 * @param[in, out] e The elevator.
 */
void queue_update_matrix(elevator_t *e);


/**
 * @brief Prints the matrix @c e.queue.
 * @param[in] e The elevator.
 */
void queue_print_matrix(elevator_t *e);


/**
 * @brief Checks if e.queue contains any orders.
 * @param[in] e The elevator.
 * @return 1 if @c e.queue contains any orders, and 0 if not.
 */
int queue_any_orders(elevator_t *e);



/**
 * @brief Checks if e.queue contains any orders above its current floor, @c e.floor.
 * @param[in] e The elevator.
 * @return 1 if @c e.queue contains any orders above @c e.floor , 0 otherwise.
 */
int queue_orders_above(elevator_t *e);



/**
 * @brief Checks if e.queue contains any orders under its current floor, e.floor.
 * @param[in] e The elevator.
 * @return 1 if @c e.queue contains any orders under @c e.floor, 0 otherwise.
 */
int queue_orders_under(elevator_t *e);



/**
 * @brief Deletes executed orders in e.queue at current floor, e.floor.
 * @param[in, out] e The elevator.
 */
void queue_delete_executed_orders(elevator_t *e);



#endif
