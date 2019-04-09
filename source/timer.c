#include <time.h>
#include "queue.h"
#include "timer.h"


void timer_start(elevator_t *e) {
    e->time = time(NULL);
}



int timer_check(elevator_t *e) {
    time_t temp = time(NULL);
    time_t diff = temp - e->time;
    if (diff >= DOOR_TIME) {
        return 1;
    }
    return 0;
}
