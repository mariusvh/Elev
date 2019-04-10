#include "elev.h"
#include "queue.h"
#include "control.h"
#include "timer.h"
#include <stdio.h>


int main() {
    // Initialize hardware
    if (!elev_init()) {
        printf("Unable to initialize elevator hardware!\n");
        return 1;
    }

    printf("Press 'OBSTRUCTION' button to stop elevator and exit program.\n");

    //Create Elevator e
    elevator_t el;

    //Preparing elevator el for start.
    control_elev_defined_start(&el);

    while (1) {

        control_update_floor(&el);
        control_update_button_lamps(&el);

        // Stop elevator and exit program if OBSTRUCTION is pressed
        if (elev_get_obstruction_signal()) {
            break;
        }


        switch (el.state) {
          case IDLE:
            queue_update_matrix(&el);

            if (elev_get_stop_signal()) {
                elev_set_stop_lamp(1);
                if (elev_get_floor_sensor_signal() != -1) {
                    elev_set_door_open_lamp(1);
                }
                el.state = STOP;
                printf("State: %d\n", el.state);
            }
            if (queue_any_orders(&el) && !control_stop_at_floor(&el)) {
                control_update_direction(&el,control_choose_direction(&el,elev_get_floor_sensor_signal()));
                el.state = MOVE;
                printf("State: %d\n", el.state);
            }
            if (control_stop_at_floor(&el)) {
                elev_set_door_open_lamp(1);
                el.state = DOOR_OPEN;
                queue_print_matrix(&el);
                printf("State: %d\n", el.state);
            }
            break;

          case MOVE:
            control_secure_range();
            queue_update_matrix(&el);

            if (elev_get_stop_signal()) {
                control_update_direction(&el, DIRN_STOP);
                elev_set_stop_lamp(1);
                el.state = STOP;
                printf("State: %d\n", el.state);
            }
            if (control_stop_at_floor(&el)) {
                control_update_direction(&el, DIRN_STOP);
                el.state = IDLE;
                printf("State: %d\n", el.state);
            }
            break;

          case DOOR_OPEN:
            queue_update_matrix(&el);

            if (elev_get_stop_signal()) {
                elev_set_stop_lamp(1);
                el.state = STOP;
                printf("State: %d\n", el.state);
            }

            if (el.time == 0) {
                timer_start(&el);
            }
            if (timer_check(&el)) {
                elev_set_door_open_lamp(0);
                el.time = 0;
                queue_delete_executed_orders(&el);
                queue_print_matrix(&el);
                el.state = IDLE;
                printf("State: %d\n", el.state);
            }

            break;

          case STOP:

          if (!elev_get_stop_signal()) {
              elev_set_stop_lamp(0);
              if (elev_get_floor_sensor_signal() != -1 ) {
                  el.time = 0;
                  elev_set_door_open_lamp(1);
                  el.state = DOOR_OPEN;
                  printf("State: %d\n", el.state);
              }
              else{
                  el.state = IDLE;
                  printf("State: %d\n", el.state);
              }
          }
            queue_reset_matrix(&el);
            break;

      }
    }

    return 0;
}
