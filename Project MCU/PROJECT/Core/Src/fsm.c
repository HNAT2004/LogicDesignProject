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
#include "get_time.h"

void fsm_run(void){
	switch(status){
	case WAITING:
		LED_BLUE();
		lcd_goto_XY(0, 0);
		lcd_send_string(time_str);
		lcd_goto_XY(1, 0);
		lcd_send_string(date_str);
		break;
	case ACCEPT:
		LED_GREEN();
		lcd_goto_XY(1, 5);
		lcd_send_string("Welcome");
		lcd_goto_XY(2, 0);
		lcd_send_string(printlcd);
		if(timer0_flag == 1){
			lcd_init();
			status = WAITING;
		}
		break;
	case DECLINE:
		LED_RED();
		lcd_goto_XY(1, 5);
		lcd_send_string("ERROR");
		if(timer0_flag == 1){
			lcd_init();
			status = WAITING;
		}
		break;
	}

}

