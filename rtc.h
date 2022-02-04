/*
 * RTC.h
 * Created on: 30/01/2022
 * Author: David Santiago Guerrero Martinez
 * FUNCIONES USADAS DEFINIDAS A LO ULTIMO
*/
#ifndef _RTC_H
#define _RTC_H

/*******************************************************************/
/****************Start Bits Fields Type Definition******************/
/*******************************************************************/

typedef union {
	uint32_t WORD;		// Acceso a todo el registro CLKDIV_M1
	struct {
		uint32_t DIVIDER	    :16;        // DIVISOR
		uint32_t 	            :16;        // reserved BIT 7:15
	} BITS;
} __rtc_clkdiv_m1;

typedef union {
	uint32_t WORD;		// Acceso a todo el registro SETUP_0
	struct {
                uint32_t DAY    :5;	  	// DAY OF THE MONTH (1..31)
                uint32_t        :3;	  	// RESERVED
                uint32_t MONTH  :4;	  	// MONTH (1..12)
                uint32_t YEAR   :12;	// DAY OF THE MONTH
                uint32_t        :8;	  	// DAY OF THE MONTH
	} BITS;
} __rtc_setup_0;

typedef union {
	uint32_t WORD;		// Acceso a todo el registro SETUP_1
	struct {
                uint32_t SEC    :6;	  	// SECONDS
                uint32_t        :2;	  	// RESERVED
                uint32_t MIN    :6;	  	// MINUTES
                uint32_t        :2;	  	// RESERVED
                uint32_t HOUR   :5;	    // HOURS
                uint32_t        :3;	  	// RESERVED
                uint32_t DOTW   :3;	  	// DAY OF THE WEEK 1-MONDAY ... 0-SUNDAY
                uint32_t        :8;	  	// RESERVED
	} BITS;
} __rtc_setup_1;

typedef union {
	uint32_t WORD;		// Acceso a todo el registro CTRL
	struct {
		uint32_t RTC_ENABLE         :1;	  	// Enable RTC
                uint32_t RTC_ACTIVE         :1;	  	// RTC enabled (running)
                uint32_t                    :2;	  	// RESERVED
                uint32_t LOAD               :1;	  	// Load RTC
                uint32_t                    :3;	  	// RESERVED
                uint32_t FORCE_NOTLEAPYEAR  :1;	  	// If set, leapyear is forced off. Useful for years divisible by 100 but not by 400
                uint32_t                    :23;    // RESERVED
	} BITS;
} __rtc_ctrl;

typedef union {
	uint32_t WORD;		// Acceso a todo el registro IRQ_SETUP_0
	struct {
		uint32_t DAY            :5;	  	// DAY OF THE MONTH (1..31)
                uint32_t                :3;	  	// RESERVED
                uint32_t MONTH          :4;	  	// MONTH (1..12)
                uint32_t YEAR           :12;	// DAY OF THE MONTH
                uint32_t DAY_ENA        :1;	  	// Enable day matching
                uint32_t MONTH_ENA      :1;	  	// Enable month matching
                uint32_t YEAR_ENA       :1;	  	// Enable year matching
                uint32_t                :1;	  	// reserved
                uint32_t MATCH_ENA      :1;	  	// Global match enable. Don’t change any other value while this one is enabled
                uint32_t MATCH_ACTIVE   :1;     
                uint32_t                :2;
	} BITS; 
} __rtc_irq_setup_0;

typedef union {
	uint32_t WORD;		// Acceso a todo el registro IRQ_SETUP_1
	struct {
		uint32_t SEC        :6;	  	// Seconds
                uint32_t            :2;	  	// RESERVED
                uint32_t MIN        :6;	  	// Minutes
                uint32_t            :2;	    // RESERVED
                uint32_t HOUR       :5;	  	// Hours
                uint32_t            :3;	  	// RESERVED
                uint32_t DOTW       :3;	  	// Day of the week
                uint32_t            :1;	  	// reserved
                uint32_t SEC_ENA    :1;	  	// Enable second matching
                uint32_t MIN_ENA    :1;     // Enable minute matching
                uint32_t HOUR_ENA   :1;     // Enable hour matching
                uint32_t DOTW_ENA   :1;
	} BITS; 
} __rtc_irq_setup_1;

typedef union {
	uint32_t WORD;		// Acceso a todo el registro RTC_1
	struct {
		uint32_t DAY    :5;	  	// Day of the month (1..31)
                uint32_t        :3;	  	// RESERVED
                uint32_t MONTH  :4;	  	// Month (1..12)
                uint32_t YEAR   :12;	// Year
                uint32_t        :8;	  	// reserved
	} BITS; 
} __rtc_1;

typedef union {
	uint32_t WORD;		// Acceso a todo el registro RTC_0
	struct {
		uint32_t SEC    :6;	  	// Seconds
                uint32_t        :2;	  	// RESERVED
                uint32_t MIN    :6;	  	// Minutes
                uint32_t        :2;	  	// RESERVED
                uint32_t HOUR   :5;	    // Hours
                uint32_t        :3;	  	// reserved
                uint32_t DOTW   :3;	  	// Day of the week
                uint32_t        :5;	  	// reserved
	} BITS; 
} __rtc_0;

typedef union {
	uint32_t WORD;		// Acceso a todo el registro INTR
	struct {
                uint32_t RTC    :1;	  	// 
                uint32_t        :31;	    // RESERVED
	} BITS; 
} __rtc_intr;

typedef union {
	uint32_t WORD;		// Acceso a todo el registro INTE
	struct {
                uint32_t RTC    :1;	  	// 
                uint32_t        :31;	    // RESERVED
	} BITS; 
} __rtc_inte;

typedef union {
	uint32_t WORD;		// Acceso a todo el registro INTF
	struct {
                uint32_t RTC    :1;	  	// 
                uint32_t        :31;	    // RESERVED
	} BITS; 
} __rtc_intf;

typedef union {
	uint32_t WORD;		// Acceso a todo el registro INTS
	struct {
                uint32_t RTC    :1;	  	// 
                uint32_t        :31;	    // RESERVED
	} BITS; 
} __rtc_ints;


/*******************************************************************/
/****************End Bits Fields Type Definition********************/
/*******************************************************************/

/*******************************************************************/
/****************Start Fields Structure Definition******************/
/*******************************************************************/

typedef struct{
        volatile __rtc_clkdiv_m1    rCLKDIV;    // 0x4005c000 Divider minus 1 for the 1 second counter. Safe to change the value when RTC is not enabled.
        volatile __rtc_setup_0      rSETUP0;    // 0x4005c004 RTC setup register 0.
        volatile __rtc_setup_1      rSETUP1;    // 0x4005c008 RTC setup register 1.
        volatile __rtc_ctrl         rCTRL;      // 0x4005c00c RTC Control and status.
        volatile __rtc_irq_setup_0  rIRQSETUP0; // 0x4005c010 Interrupt setup register 0.
        volatile __rtc_irq_setup_1  rIRQSETUP1; // 0x4005c014 Interrupt setup register 1.
        volatile __rtc_1            rRTC1;      // 0x4005c018 RTC register 1.
        volatile __rtc_0            rRTC0;      // 0x4005c01c RTC register 0, Read this before RTC 1!.
        volatile __rtc_intr         rINTR;      // 0x4005c020 Raw Interrupts
        volatile __rtc_inte         rINTE;      // 0x4005c024 Interrupt Enable
        volatile __rtc_intf         rINTF;      // 0x4005c028 Interrupt Force
        volatile __rtc_ints         rINTS;      // 0x4005c02c Interrupt status after masking & forcing
} __rtc_t;

/*******************************************************************/
/****************End Fields Structure Definition********************/
/*******************************************************************/

/*******************************************************************/
/****************Start MACROs per Register**************************/
/*******************************************************************/

#define sRTC (*((volatile __rtc_t *)(0x4005c000)))			// RTC general structure

// Para CLKDIV_M1
#define rCLKDIV_M1 sRTC.rCLKDIV.WORD
#define bCLKDIV_M1_DIVIDER sRTC.rCLKDIV.BITS.DIVIDER

// Para SETUP_0
#define rSETUP_0 sRTC.rSETUP0.WORD
#define bSETUP0_YEAR sRTC.rSETUP0.BITS.YEAR
#define bSETUP0_MONTH sRTC.rSETUP0.BITS.MONTH
#define bSETUP0_DAY sRTC.rSETUP0.BITS.DAY

// Para SETUP_1
#define rSETUP_1 sRTC.rSETUP1.WORD
#define bSETUP1_DOTW sRTC.rSETUP1.BITS.DOTW
#define bSETUP1_HOUR sRTC.rSETUP1.BITS.HOUR
#define bSETUP1_MIN sRTC.rSETUP1.BITS.MIN
#define bSETUP1_SEC sRTC.rSETUP1.BITS.SEC

// Para CTRL
#define rCTRL_ sRTC.rCTRL.WORD
#define bCTRL_FORCE_NOTLEAPYEAR sRTC.rCTRL.BITS.FORCE_NOTLEAPYEAR
#define bCTRL_LOAD sRTC.rCTRL.BITS.LOAD
#define bCTRL_RTC_ACTIVE sRTC.rCTRL.BITS.RTC_ACTIVE
#define bCTRL_RTC_ENABLE sRTC.rCTRL.BITS.RTC_ENABLE

// Para IRQ_SETUP_0
#define rIRQSETUP_0 sRTC.rIRQSETUP0.WORD
#define bIRQSETUP0_MATCH_ACTIVE sRTC.rIRQSETUP0.BITS.MATCH_ACTIVE
#define bIRQSETUP0_MATCH_ENA sRTC.rIRQSETUP0.BITS.MATCH_ENA
#define bIRQSETUP0_YEAR_ENA sRTC.rIRQSETUP0.BITS.YEAR_ENA
#define bIRQSETUP0_MONTH_ENA sRTC.rIRQSETUP0.BITS.MONTH_ENA
#define bIRQSETUP0_DAY_ENA sRTC.rIRQSETUP0.BITS.DAY_ENA
#define bIRQSETUP0_YEAR sRTC.rIRQSETUP0.BITS.YEAR
#define bIRQSETUP0_MONTH sRTC.rIRQSETUP0.BITS.MONTH
#define bIRQSETUP0_DAY sRTC.rIRQSETUP0.BITS.DAY

// Para IRQ_SETUP_1
#define rIRQSETUP_1 sRTC.rIRQSETUP1.WORD
#define bIRQSETUP1_DOTW_ENA sRTC.rIRQSETUP1.BITS.DOTW_ENA
#define bIRQSETUP1_HOUR_ENA sRTC.rIRQSETUP1.BITS.HOUR_ENA
#define bIRQSETUP1_MIN_ENA sRTC.rIRQSETUP1.BITS.MIN_ENA
#define bIRQSETUP1_SEC_ENA sRTC.rIRQSETUP1.BITS.SEC_ENA
#define bIRQSETUP1_DOTW sRTC.rIRQSETUP1.BITS.DOTW
#define bIRQSETUP1_HOUR sRTC.rIRQSETUP1.BITS.HOUR
#define bIRQSETUP1_MIN sRTC.rIRQSETUP1.BITS.MIN
#define bIRQSETUP1_SEC sRTC.rIRQSETUP1.BITS.SEC

// Para RTC_1
#define rRTC_1 sRTC.rRTC1.WORD
#define bRTC1_YEAR sRTC.rRTC1.BITS.YEAR
#define bRTC1_MONTH sRTC.rRTC1.BITS.MONTH
#define bRTC1_DAY sRTC.rRTC1.BITS.DAY

// Para RTC_1
#define rRTC_0 sRTC.rRTC0.WORD
#define bRTC0_DOTW sRTC.rRTC0.BITS.DOTW
#define bRTC0_HOUR sRTC.rRTC0.BITS.HOUR
#define bRTC0_MIN sRTC.rRTC0.BITS.MIN
#define bRTC0_SEC sRTC.rRTC0.BITS.SEC

// Para INTR
#define rINTR sRTC.rINTR.WORD
#define bINTR sRTC.rINTR.BITS.RTC

// Para INTE
#define rINTE sRTC.rINTE.WORD
#define bINTE sRTC.rINTE.BITS.RTC

// Para INTF
#define rINTF sRTC.rINTF.WORD
#define bINTF sRTC.rINTF.BITS.RTC

// Para INTS
#define rINTS sRTC.rINTS.WORD
#define bINTS sRTC.rINTS.BITS.RTC


extern volatile uint8_t alarm_req;
void rtc_alarm_init(datetime_t *t);

#define vYEAR    2022
#define vMONTH   1
#define vDAY     30
#define vDOTW    0
#define vHOUR    7
#define vMIN     20
#define vSEC     50

#endif /* RTC_H_ */