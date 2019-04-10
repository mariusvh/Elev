#include "queue.h"
#include "control.h"
#include "elev.h"
#include <stdio.h>





void control_update_floor(elevator_t *e) {
    int tempFloor = elev_get_floor_sensor_signal();

    if (tempFloor == -1) {
        return;
    }
    e->floor = tempFloor;
    elev_set_floor_indicator(e->floor);
}


void control_secure_range(){
    if (elev_get_floor_sensor_signal() == N_FLOORS - 1) {
        elev_set_motor_direction(DIRN_DOWN);
    } else if (elev_get_floor_sensor_signal() == 0) {
        elev_set_motor_direction(DIRN_UP);
    }
}


void control_update_direction(elevator_t *e, elev_motor_direction_t dirn) {
    e->prevDir = e->dir;
    e->dir = dirn;
    elev_set_motor_direction(dirn);
}


void control_elev_defined_start(elevator_t *e) {
    e->floor = elev_get_floor_sensor_signal();
    if (e->floor != -1) {
        elev_set_floor_indicator(e->floor);
        e->dir = DIRN_STOP;
        e->prevDir = DIRN_STOP;
        e->state = IDLE;
        e->time = 0;
        queue_reset_matrix(e);
    }
    else {
        elev_set_motor_direction(DIRN_DOWN);
        while (elev_get_floor_sensor_signal() == -1) {
        }
        elev_set_motor_direction(DIRN_STOP);
        e->floor = elev_get_floor_sensor_signal();
        elev_set_floor_indicator(e->floor);
        e->dir = DIRN_STOP;
        e->prevDir = DIRN_STOP;
        e->state = IDLE;
        e->time = 0;
        queue_reset_matrix(e);
    }
}


void control_update_button_lamps(elevator_t *e) {
    for (int i = 0; i < N_FLOORS-1; i++) {
        if (e->queue[i][BUTTON_CALL_UP] == 1) {
            elev_set_button_lamp(BUTTON_CALL_UP,i,1);
        }
        else {
            elev_set_button_lamp(BUTTON_CALL_UP,i,0);
        }
    }
    for (int j = 1;j < N_FLOORS ;j++) {
        if (e->queue[j][BUTTON_CALL_DOWN] == 1) {
            elev_set_button_lamp(BUTTON_CALL_DOWN,j,1);
        }
        else {
            elev_set_button_lamp(BUTTON_CALL_DOWN,j,0);
        }
    }
    for (int k = 0; k < N_FLOORS; k++) {
        if (e->queue[k][BUTTON_COMMAND] == 1) {
            elev_set_button_lamp(BUTTON_COMMAND,k,1);
        }
        else {
            elev_set_button_lamp(BUTTON_COMMAND,k,0);
        }
    }

}


elev_motor_direction_t control_floor_inbetween(elevator_t *e) {
    if (e->floor == N_FLOORS-1) {
        if (e->queue[e->floor][BUTTON_CALL_DOWN]||  e->queue[e->floor][BUTTON_COMMAND]) {
            return DIRN_UP;
        }
    }
    if (e->floor == 0) {
        if (e->queue[e->floor][BUTTON_CALL_UP] || e->queue[e->floor][BUTTON_COMMAND]) {
            return DIRN_DOWN;
        }

    }
    if (e->prevDir == DIRN_UP && (e->floor != 0 && e->floor != N_FLOORS-1)) {
        if (e->queue[e->floor][BUTTON_CALL_DOWN]|| e->queue[e->floor][BUTTON_CALL_UP] || e->queue[e->floor][BUTTON_COMMAND]) {
            return DIRN_DOWN;
        }
    }
    if (e->prevDir == DIRN_DOWN && (e->floor != 0 && e->floor != N_FLOORS-1)) {
        if (e->queue[e->floor][BUTTON_CALL_DOWN]|| e->queue[e->floor][BUTTON_CALL_UP] || e->queue[e->floor][BUTTON_COMMAND]) {
            return DIRN_UP;
        }
    }
    return control_choose_direction(e,0);
}


elev_motor_direction_t control_choose_direction(elevator_t *e, int value){
    if (value == -1) {
        return control_floor_inbetween(e);
      }
  switch (e->prevDir) {
    case DIRN_UP:
      if (queue_orders_above(e)) {
          return DIRN_UP;
     }
      if (queue_orders_under(e)) {
          return DIRN_DOWN;
      }
      return DIRN_STOP;

    case DIRN_DOWN:
      if (queue_orders_under(e)) {
          return DIRN_DOWN;
      }
      if (queue_orders_above(e)) {
          return DIRN_UP;
      }
      return DIRN_STOP;

    case DIRN_STOP:
      if (queue_orders_under(e)) {
          return DIRN_DOWN;
      }
      if (queue_orders_above(e)) {
          return DIRN_UP;
      }
      return DIRN_STOP;

    default:
      return DIRN_STOP;
  }

}


int control_stop_at_floor(elevator_t *e) {
  switch (e->dir) {
    case DIRN_UP:
      if ((e->queue[e->floor][BUTTON_CALL_UP] || e->queue[e->floor][BUTTON_COMMAND] || !queue_orders_above(e)) && elev_get_floor_sensor_signal() != -1) {
        return 1;
      }
      return 0;

    case DIRN_DOWN:
        if ((e->queue[e->floor][BUTTON_CALL_DOWN] || e->queue[e->floor][BUTTON_COMMAND] || !queue_orders_under(e)) && elev_get_floor_sensor_signal() != -1) {
            return 1;
        }
        return 0;

    case DIRN_STOP:
        if (e->floor == 0 && elev_get_floor_sensor_signal() != -1) {
            if (e->queue[e->floor][BUTTON_CALL_UP] || e->queue[e->floor][BUTTON_COMMAND]) {
                return 1;
            }
            return 0;
        }
        if (e->floor == N_FLOORS-1 && elev_get_floor_sensor_signal() != -1) {
            if (e->queue[e->floor][BUTTON_CALL_DOWN] || e->queue[e->floor][BUTTON_COMMAND]) {
                return 1;
            }
            return 0;
        }
        if (e->floor != -1 && elev_get_floor_sensor_signal() != -1) {
            if (e->queue[e->floor][BUTTON_CALL_DOWN]|| e->queue[e->floor][BUTTON_CALL_UP] || e->queue[e->floor][BUTTON_COMMAND]) {
                return 1;
            }
            return 0;
        }
    }
    return 0;
}
