/*
 * fsm.c
 *
 *  Created on: Nov 7, 2024
 *      Author: ADMIN
 */
#include "fsm.h"
#include "global.h"
#include "LED.h"
#include "i2c-lcd.h"
#include "software_timer.h"

void fsm_run(void){
	switch(status){
	case WAITING:
		LED_YELLOW();
		lcd_goto_XY(1, 5);
		lcd_send_string("WAITING");
		break;
	case ACCEPT:
		LED_GREEN();
		lcd_goto_XY(1, 5);
		lcd_send_string("WELCOME");
		if(timer0_flag == 1){
			lcd_init();
			status = WAITING;
		}
		break;
	case DECLINE:
		LED_RED();
		lcd_goto_XY(1, 5);
		lcd_send_string("INVALID");
		if(timer0_flag == 1){
			lcd_init();
			status = WAITING;
		}
		break;
	}

}

