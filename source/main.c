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

    printf("Press 'CTRL^C' to stop elevator and exit program.\n");

    //Create Elevator e
    elevator_t el;

    //Preparing elevator el for start.
    control_elev_defined_start(&el);

    while (1) {
        // Updates el.floor and button lamps in real time.
        control_update_floor(&el);
        control_update_button_lamps(&el);


        switch (el.state) {
          case IDLE:
            // Updates incoming orders in el.queue in real time.
            queue_update_matrix(&el);

            // If stop-button is pressed, enter stop-state.
            if (elev_get_stop_signal()) {
                elev_set_stop_lamp(1);
                if (elev_get_floor_sensor_signal() != -1) {
                    elev_set_door_open_lamp(1);
                }
                el.state = STOP;
            }
            // If el.queue contains any orders and if elevator should not stop, enter move-state.
            // Updates direction for next order.
            if (queue_any_orders(&el) && !control_stop_at_floor(&el)) {
                control_update_direction(&el,control_choose_direction(&el,elev_get_floor_sensor_signal()));
                el.state = MOVE;
            }
            // If el.queue has any orders at current cloor, enter open-door-state.
            if (control_stop_at_floor(&el)) {
                elev_set_door_open_lamp(1);
                el.state = DOOR_OPEN;
            }
            break;

          case MOVE:
            control_secure_range();
            queue_update_matrix(&el);

            // If stop-button is pressed, enter stop-state.
            if (elev_get_stop_signal()) {
                control_update_direction(&el, DIRN_STOP);
                elev_set_stop_lamp(1);
                el.state = STOP;
            }

            // If el.queue has any orders at current cloor, enter idle-state.
            if (control_stop_at_floor(&el)) {
                control_update_direction(&el, DIRN_STOP);
                el.state = IDLE;
            }
            break;

          case DOOR_OPEN:
            queue_update_matrix(&el);

            // If stop-button is pressed, enter stop-state.
            if (elev_get_stop_signal()) {
                elev_set_stop_lamp(1);
                el.state = STOP;
            }

            if (el.time == 0) {
                timer_start(&el);
                queue_delete_executed_orders(&el);
            }

            // If DOOR_TIME seconds has passed since el.time, door closes,
            // and elevator returns to idle-state.
            if (timer_check(&el)) {
                elev_set_door_open_lamp(0);
                el.time = 0;
                el.state = IDLE;
            }

            if (!timer_check(&el) && control_stop_at_floor(&el)) {
                el.time = 0;
            }

            break;

          case STOP:

          // If stop-button is released, returns to door-open state if elevator is at a floor,
          // returns to idle-state otherwise. Deletes all orders in el.queue.
          if (!elev_get_stop_signal()) {
              elev_set_stop_lamp(0);
              if (elev_get_floor_sensor_signal() != -1 ) {
                  el.time = 0;
                  elev_set_door_open_lamp(1);
                  el.state = DOOR_OPEN;
              }
              else{
                  el.state = IDLE;
              }
          }
            queue_reset_matrix(&el);
            break;
      }
    }

    return 0;
}
