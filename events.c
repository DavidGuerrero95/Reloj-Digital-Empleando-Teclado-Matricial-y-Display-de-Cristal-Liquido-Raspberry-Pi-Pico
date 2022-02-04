#include <stdio.h>
#include "events.h"
#include "general.h"
#include "gpio.h"
#include "lcd.h"
#include "keyboard.h"

volatile _events_str _events;
volatile uint8_t debounceCounter = 0;
volatile bool    pendingCounting = false;
volatile uint8_t counter = 0;
bool init_debounce = true;
bool key_pressed = false;
bool led_alarm = false;
uint16_t led_debounce = 0;

uint8_t time_debounce = KEYBOARD_DEBOUNCE;
uint16_t show_lcd_debounce = LCD_DEBOUNCE;

uint8_t time_shift = time_shift_;
volatile bool deadline_shift = false;
volatile char key = 0;

void eventTimer(void){
	if(EV_TIMER){
		EV_TIMER = 0;
		if(--time_shift ==0){
            deadline_shift=true;
        }
		if((--time_debounce == 0) & init_debounce){
            init_debounce=false;
            key = capture_key();
			key_pressed = true;
        }
		if(--show_lcd_debounce == 0){
			lcdDebounce = true;
			show_lcd_debounce = LCD_DEBOUNCE;
		}

		if((--led_debounce == 0) & led_alarm){
			led_alarm = false;
		}
	}
}

void eventGpio(void){
	if(EV_GPIO){
		EV_GPIO = 0;
		time_debounce = KEYBOARD_DEBOUNCE;
        init_debounce = true;
	}
}

void eventRtc(void){
	if(EV_RTC){
		EV_RTC = 0;
		led_alarm = true;
		led_debounce = LEDDEBOUNCE;
	}
}