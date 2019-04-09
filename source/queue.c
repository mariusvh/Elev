#include "queue.h"
#include "elev.h"
#include <stdio.h>






void queue_reset_matrix(elevator_t *e){
  for (int i = 0; i < N_FLOORS; i++) {
    for (int j = 0; j < N_BUTTONS; j++) {
      e->queue[i][j] = 0;
    }
  }
  e->queue[0][1] = -1;
  e->queue[N_FLOORS-1][0] = -1;
}


void queue_update_matrix(elevator_t *e){
  for (int i = 0; i < N_FLOORS-1; i++) {
      e->queue[i][BUTTON_CALL_UP] = e->queue[i][BUTTON_CALL_UP] || elev_get_button_signal(BUTTON_CALL_UP,i);
  }
  for (int j = 1;j < N_FLOORS ;j++) {
      e->queue[j][BUTTON_CALL_DOWN] = e->queue[j][BUTTON_CALL_DOWN] || elev_get_button_signal(BUTTON_CALL_DOWN,j);
  }
  for (int k = 0; k < N_FLOORS; k++) {
      e->queue[k][BUTTON_COMMAND] = e->queue[k][BUTTON_COMMAND] || elev_get_button_signal(BUTTON_COMMAND,k);
  }
  e->queue[0][1] = -1;
  e->queue[N_FLOORS-1][0] = -1;
}


void queue_print_matrix(elevator_t *e){
    for (int i = 0; i < N_FLOORS; i++) {
        for (size_t j = 0; j < N_BUTTONS; j++) {
            printf("%d\t",e->queue[i][j]);
        }
        printf("\n");
    }
}


int queue_any_orders(elevator_t *e){
  for (int i = 0; i < N_FLOORS; i++) {
    for (int j = 0; j < N_BUTTONS; j++) {
      if (e->queue[i][j] == 1) {
        return 1;
      }
    }
  }
  return 0;
}


int queue_orders_above(elevator_t *e){
  for (int i = e->floor + 1; i < N_FLOORS; i++) {
    for (int j = 0; j < N_BUTTONS; j++) {
      if (e->queue[i][j] == 1) {
        return 1;
      }
    }
  }
  return 0;
}


int queue_orders_under(elevator_t *e){
  for (int i = 0; i < e->floor; i++) {
    for (int j = 0; j < N_BUTTONS; j++) {
      if (e->queue[i][j] == 1) {
        return 1;
      }
    }
  }
  return 0;
}


void queue_delete_executed_orders(elevator_t *e){
  for (int i = 0; i < N_BUTTONS; i++) {
    e->queue[e->floor][i] = 0;
  }
  e->queue[0][1] = -1;
  e->queue[N_FLOORS-1][0] = -1;
}



