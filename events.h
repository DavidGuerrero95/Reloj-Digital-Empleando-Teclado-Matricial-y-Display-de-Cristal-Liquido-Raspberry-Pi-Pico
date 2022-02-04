#ifndef MYEVENTS_H_
#define MYEVENTS_H_

#include "pico/stdlib.h"

typedef union {
	uint8_t reg;
	struct {
		uint8_t flag0 : 1;
		uint8_t flag1 : 1;
		uint8_t flag2 : 1;
		uint8_t flag3 : 1;
		uint8_t flag4 : 1;
		uint8_t flag5 : 1;
		uint8_t flag6 : 1;
		uint8_t flag7 : 1;
	}flags;
}_events_str;

extern volatile _events_str _events;

#define PENDING_EVENTS	_events.reg
#define EV_TIMER		_events.flags.flag0
#define EV_GPIO			_events.flags.flag1
#define EV_RTC			_events.flags.flag2

extern volatile bool deadline_shift;
extern volatile char key;
extern uint8_t time_shift;
extern bool key_pressed;
extern bool led_alarm;
extern uint16_t led_debounce;

#define DEBOUNCECOUNT	3			// Timer generates INTs each 10ms (3x10ms = 30ms for debouncing)
#define LEDDEBOUNCE		6000
#define time_shift_ 	50
#define PINLED			18

void eventTimer(void);
void eventGpio(void);
void eventRtc(void);
#endif