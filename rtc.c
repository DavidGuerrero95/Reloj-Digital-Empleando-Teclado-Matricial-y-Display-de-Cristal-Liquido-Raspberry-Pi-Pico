#include <stdio.h>
#include "pico/stdlib.h"
#include "events.h"
#include "rtc.h"

/* Manejador de interrupción o callback */
void alarm_callback() {
    EV_RTC = 1;
}

/* Rutina para preparar el timer */
void rtc_alarm_init(datetime_t *t) {
    rtc_set_alarm(t, &alarm_callback);
    //add_repeating_timer_ms(-delay_ms, timer_callback, NULL, &timer);
}