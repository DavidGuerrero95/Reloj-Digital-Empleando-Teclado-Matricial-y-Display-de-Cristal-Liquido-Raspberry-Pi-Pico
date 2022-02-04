#include <stdio.h>
#include <string.h>
#include "pico/stdlib.h"
#include "pico/util/datetime.h"
#include "hardware/rtc.h"
#include "timer.h"
#include "gpio.h"
#include "general.h"
#include "events.h"
#include "keyboard.h"
#include "lcd.h"
#include "rtc.h"

//******VARIABLES*****
uint8_t estado_stm = 0;

// *****FUNCIONES*****
void putMensaje(uint nDotw,uint nDay,uint nMonth,uint nYear,uint nHour,uint nMin,uint nSeg);
void showOpciones(void);
void move_display_normal(int var);
void captureData(void);
void new_alarm_date(char date[], char message[], unsigned char posCursor, uint data);
void newFecha(void);
void newAlarm(void);
void reset_clock(void);
// variables KeyBoard

// variables RTC
char datetime_buf[256];
char *datetime_str = &datetime_buf[0];

// variables LCD
char msnNormal[] = "Press D to configure";
char msnOpciones1[] = "Press A to Alarm |";
char msnOpciones2[] = "Press B to Clock |";
char msnOpciones3[] = "Press C to Exit";
char msnOpciones4[] = "Press # off alarm";
int num_shift=0;
int contSwDt = 0;
bool banContSwDt = true;
// variables to Display

unsigned int dotw, day, month, year, hour, min, sec;

// Variables Auxiliares
uint contInt = 0;
uint valor = 0;
bool flagData = false, i=false, j=false;
bool eYear = false, eMonth = false;
bool eDay = false, eDotw = false;
bool eHour = false, eMin = false;
bool resetFlag = true;

datetime_t t = {
            .year  = vYEAR,
            .month = vMONTH,        // Month (1..12) 
            .day   = vDAY,        // Day of the month (1..31)
            .dotw  = vDOTW,         // DAY OF THE WEEK 1-MONDAY ... 0-SUNDAY
            .hour  = vHOUR,
            .min   = vMIN,
            .sec   = vSEC
    };
int main() {
    // STDIO: inicialización de USB-UART para entrada/salida
    stdio_init_all();
    keyboard_init();// inicializamos el keyboard 
    lcd_init();
    if (!timerInitialization(TIMER_PERIOD_MS)) {
        printf("Timer initialization error.\n");
        SYSTEM_HALT();
    }
    // Start on Wednesday 2th of Febrary 2022 01:45:00
    
    putMensaje(t.dotw, t.day, t.month, t.year, t.hour, t.min, t.sec);
    rtc_init();
    rtc_set_datetime(&t);

    while (true) {
        if (!PENDING_EVENTS) {
			WAITFORINT();
		}
        eventGpio();
        eventTimer();
        eventRtc();

        if(key == 'D' && estado_stm == 0 && key_pressed){
            estado_stm=1;
            showOpciones();
            num_shift = 0;
        }

        if(key == 'A' && estado_stm == 1){
            WriteCommandToLCD(0x01,1640);
            estado_stm = 2;
            key_pressed = false;
        }

        if(key == 'B' && estado_stm == 1){
            WriteCommandToLCD(0x01,1640); // LCD CLEAR
            estado_stm = 3;
            key_pressed = false;
        }

        if(key == 'C' && key_pressed){
            reset_clock();
        }

        if(key == '#' && key_pressed){
            led_alarm = false;
        }

        if(led_alarm){
            gpio_put(PINLED, 1);
        } else{
            gpio_put(PINLED, 0);
        }

        switch (estado_stm){
        case 0:
            rtc_get_datetime(&t);
            if(lcdDebounce){
                lcdDebounce = false;
                putMensaje(t.dotw, t.day, t.month, t.year, t.hour, t.min, t.sec);
            }else {
                 move_display_normal(7);
            }
            break;
        case 1:
            move_display_normal(22);
            break;
        case 2:
            captureData();
            break;
        case 3:
            captureData();
            break;
        case 4:
            newAlarm();
            estado_stm = 0;
            contSwDt = 0;
            break;
        case 5:
            newFecha();
            estado_stm = 0;
            contSwDt = 0;
            break;
        default:
            break;
        }
    }
}

void newAlarm(void){
    datetime_t alarma = {
            .year  = year,
            .month = month,        // Month (1..12) 
            .day   = day,        // Day of the month (1..31)
            .dotw  = dotw,         // DAY OF THE WEEK 1-MONDAY ... 0-SUNDAY
            .hour  = hour,
            .min   = min,
            .sec   = sec
    };
    rtc_alarm_init(&alarma);
}

void newFecha(void){
    datetime_t t = {
            .year  = year,
            .month = month,        // Month (1..12) 
            .day   = day,        // Day of the month (1..31)
            .dotw  = dotw,         // DAY OF THE WEEK 1-MONDAY ... 0-SUNDAY
            .hour  = hour,
            .min   = min,
            .sec   = sec
    };
    rtc_set_datetime(&t);
    putMensaje(t.dotw, t.day, t.month, t.year, t.hour, t.min, t.sec);
}

void captureData(void) {
    switch (contSwDt) {
    case 0: 
        if(i){
            new_alarm_date("Year:", "VALOR MENOR ALARMA", 0x05, 4);
        } else if(j){
            new_alarm_date("Year:", "VALOR ERRONEO", 0x05, 4);
        }else {
            new_alarm_date("Year:", "0..4095, PRESS D", 0x05, 4);
        }
        if(key == 'D' && key_pressed){
            if(valor <= 4095){
                if(valor < t.year && estado_stm == 2){
                    i = true;
                    j = false;
                } else {
                    if(valor == t.year){eYear=true;}
                    year = valor;
                    contSwDt++;
                    i = false;
                    j = false;
                }  
            }else {
                j = true;
                i = false;
            }
            banContSwDt = true;
            contInt = 0;
            valor = 0;
            WriteCommandToLCD(0x01,1640); // LCD CLEAR
            key_pressed = false;
        }
        break;
    case 1:
        if(i){
            new_alarm_date("Month:", "VALOR MENOR ALARMA", 0x06, 2);
        } else if(j){
            new_alarm_date("Month:", "VALOR ERRONEO", 0x06, 2);
        }else {
            new_alarm_date("Month:", "1..12, PRESS D", 0x06, 2);
        }
        if(key == 'D' && key_pressed){
            if(valor <= 12 && valor > 0){
                if(valor < t.month && estado_stm == 2 && eYear){
                    i = true;
                    j = false;
                } else {
                    if(valor == t.month && eYear ){eMonth=true;}
                    month = valor;
                    contSwDt++;
                    i = false;
                    j = false;
                }  
            }else {
                j = true;
                i = false;
            }
            banContSwDt = true;
            contInt = 0;
            valor = 0;
            WriteCommandToLCD(0x01,1640); // LCD CLEAR
            key_pressed = false;
        }
        break;
    case 2:
        if(i){
            new_alarm_date("Day:", "VALOR MENOR ALARMA", 0x04, 2);
        } else if(j){
            new_alarm_date("Day:", "VALOR ERRONEO", 0x04, 2);
        }else {
            new_alarm_date("Day:", "1..31, PRESS D", 0x04, 2);
        }
        if(key == 'D' && key_pressed){
            if(valor <= 31 && valor > 0){
                if(valor < t.day && estado_stm == 2 && eMonth && eYear){
                    i = true;
                    j = false;
                } else {
                    if(valor == t.day && eMonth && eYear){eDay=true;}
                    day = valor;
                    contSwDt++;
                    i = false;
                    j = false;
                }  
            }else {
                j = true;
                i = false;
            }
            banContSwDt = true;
            contInt = 0;
            valor = 0;
            WriteCommandToLCD(0x01,1640); // LCD CLEAR
            key_pressed = false;
        }
        break;
    case 3:
        if(j){
            new_alarm_date("Day of Week:", "VALOR ERRONEO", 0x0c, 1);
        }else {
            new_alarm_date("Day of Week:", "0...6.Sunday=0 D", 0x0c, 1);
        }
        if(key == 'D' && key_pressed){
            if(valor <= 6){
                dotw = valor;
                contSwDt++;
                i = false;
                j = false;
            }else {
                j = true;
                i = false;
            }
            banContSwDt = true;
            contInt = 0;
            valor = 0;
            WriteCommandToLCD(0x01,1640); // LCD CLEAR
            key_pressed = false;
        }
        break;
    case 4:
        if(i){
            new_alarm_date("Hour:", "VALOR MENOR ALARMA", 0x05, 2);
        } else if(j){
            new_alarm_date("Hour:", "VALOR ERRONEO", 0x05, 2);
        }else {
            new_alarm_date("Hour:", "0..23, PRESS D", 0x05, 2);
        }
        if(key == 'D' && key_pressed){
            if(valor <= 23){
                if(valor < t.hour && estado_stm == 2 && eDay && eMonth && eYear){
                    i = true;
                    j = false;
                } else {
                    if(valor==t.hour && eDay && eMonth && eYear ){eHour=true;}
                    hour = valor;
                    contSwDt++;
                    i = false;
                    j = false;
                }  
            }else {
                j = true;
                i = false;
            }
            banContSwDt = true;
            contInt = 0;
            valor = 0;
            WriteCommandToLCD(0x01,1640); // LCD CLEAR
            key_pressed = false;
        }
        break;
    case 5:
        if(i){
            new_alarm_date("Min:", "VALOR MENOR ALARMA", 0x04, 2);
        } else if(j){
            new_alarm_date("Min:", "VALOR ERRONEO", 0x04, 2);
        }else {
            new_alarm_date("Min:", "0..59, PRESS D", 0x04, 2);
        }
        if(key == 'D' && key_pressed){
            if(valor <= 59){
                if(valor < t.min && estado_stm == 2 && eHour && eDay && eMonth && eYear){
                    i = true;
                    j = false;
                } else {
                    if(valor==t.min && eHour && eDay && eMonth && eYear){eMin=true;}
                    min = valor;
                    contSwDt++;
                    i = false;
                    j = false;
                }  
            }else {
                j = true;
                i = false;
            }
            banContSwDt = true;
            contInt = 0;
            valor = 0;
            WriteCommandToLCD(0x01,1640); // LCD CLEAR
            key_pressed = false;
        }
        break;
    case 6:
        if(i){
            new_alarm_date("Sec:", "VALOR MENOR ALARMA", 0x04, 2);
        } else if(j){
            new_alarm_date("Sec:", "VALOR ERRONEO", 0x04, 2);
        }else {
            new_alarm_date("Sec:", "0..59, PRESS D", 0x04, 2);
        }
        if(key == 'D' && key_pressed){
            if(valor <= 59){
                if(valor <= t.sec && estado_stm == 2 && eHour && eDay && eMonth && eYear && eMin){
                    i = true;
                    j = false;
                } else {
                    sec = valor;
                    contSwDt = 0;
                    i = false;
                    j = false;
                }  
            }else {
                j = true;
                i = false;
            }
            banContSwDt = true;
            contInt = 0;
            valor = 0;
            WriteCommandToLCD(0x01,1640); // LCD CLEAR
            key_pressed = false;
            num_shift = 0;
            if(estado_stm == 2) {
                estado_stm = 4;
            } else {
                estado_stm = 5;
            }
        }
        break;
    default:
        year = 2021;
        month = 12;
        day = 1;
        dotw = 0;
        hour = 0;
        min = 1;
        sec = 1;
        break;
    }
}

void showOpciones(void){
    WriteCommandToLCD(0x01,1640); // LCD CLEAR
    WriteManyCharactersPosition(msnOpciones1,0x01);
    WriteManyCharactersPosition(msnOpciones2,0x41);
    WriteManyCharactersPosition(msnOpciones3,0x14);
    WriteManyCharactersPosition(msnOpciones4,0x54);
}

void putMensaje(uint nDotw,uint nDay,uint nMonth,uint nYear,uint nHour,uint nMin,uint nSeg){
    bool flag = false;
    WriteManyCharactersPosition(msnNormal,0x41);
    switch (nDotw){
    case 0:
        WriteManyCharactersPosition("SUN",0x00);
        break;
    case 1:
        WriteManyCharactersPosition("MON",0x00);
        break;
    case 2:
        WriteManyCharactersPosition("TUE",0x00);
        break;
    case 3:
        WriteManyCharactersPosition("WED",0x00);
        break;
    case 4:
        WriteManyCharactersPosition("THU",0x00);
        break;
    case 5:
        WriteManyCharactersPosition("FRI",0x00);
        break;
    default:
        WriteManyCharactersPosition("SAT",0x00);
        break;
    }
    WriteDataToLCD(' ');
    if(nDay < 10){
        WriteDataToLCD(' ');
        char str[1];
        sprintf(str, "%d", nDay);
        WriteManyCharacters(str);
    } else {
        char str[2];
        sprintf(str, "%d", nDay);
        WriteManyCharacters(str);
    }
    WriteDataToLCD('/');
    if(nMonth < 10){
        WriteDataToLCD('0');
        char str[1];
        sprintf(str, "%d", nMonth);
        WriteManyCharacters(str);
    } else {
        char str[2];
        sprintf(str, "%d", nMonth);
        WriteManyCharacters(str);
    }
    WriteDataToLCD('/');
    char str[4];
    sprintf(str, "%d", nYear);
    WriteManyCharacters(str);
    WriteDataToLCD(' ');
    if(nHour < 12){
        flag = true;
    }else{
        nHour -= 12;
    }
    if(nHour < 10){
        WriteDataToLCD(' ');
        char str[1];
        sprintf(str, "%d", nHour);
        WriteManyCharacters(str);
    } else {
        char str[2];
        sprintf(str, "%d", nHour);
        WriteManyCharacters(str);
    }
    WriteDataToLCD(':');
    if(nMin< 10){
        WriteDataToLCD('0');
        char str[1];
        sprintf(str, "%d", nMin);
        WriteManyCharacters(str);
    } else {
        char str[2];
        sprintf(str, "%d", nMin);
        WriteManyCharacters(str);
    }
    if(flag){
        WriteManyCharacters("AM");
    } else {
        WriteManyCharacters("PM");
    }
}

void move_display_normal(int var){
    if(deadline_shift){
        if(num_shift<var){
            num_shift++;
            lcd_shift(1,0);
        }else if(num_shift>=var && num_shift<(var*2)){
            num_shift++;
            lcd_shift(1,1);
            }else {
                num_shift = 0;
            }
            deadline_shift=false;
            time_shift=time_shift_;
    }
}

void new_alarm_date(char date[], char message[], unsigned char posCursor, uint data){
    if(banContSwDt){
        WriteManyCharacters(date);
        WriteManyCharactersPosition(message,0x40);
        banContSwDt = false;
        lcd_set_dd_ram_address(posCursor);
    }
    if(key_pressed && contInt < data && key != 'A' && key != 'B' && key != 'C' && key != 'D' && key != '*' && key != '#' ){
        key_pressed = false;
        WriteDataToLCD(key);
        if(contInt != 0){valor *= 10;}
        valor += key - '0' ;
        contInt++;
    }
}

void reset_clock(void){
    WriteCommandToLCD(0x01,1640);
    estado_stm = 0;
    key_pressed = false;
    estado_stm = 0;
    contSwDt = 0;
    banContSwDt = true;
    contInt = 0;
    valor = 0;
    i = false;
    j = false;
    num_shift=0;
    uint contInt = 0;
    valor = 0;
    flagData = false; i=false; j=false;
    eYear = false; eMonth = false;
    eDay = false; eDotw = false;
    eHour = false; eMin = false;
}