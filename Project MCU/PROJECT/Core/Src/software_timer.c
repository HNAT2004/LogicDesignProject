/*
 * software_timer.c
 *
 *  Created on: Sep 23, 2024
 *      Author: ADMIN
 */

#include "main.h"
#include "software_timer.h"

const int MAX_LED = 4;
int index_led = 0;
int led_buffer[4] = {1, 2, 3, 4};

int timer0_flag = 0;
int timer0_counter = 0;
int timer1_flag = 0;
int timer1_counter = 0;

int TIMER_CYCLE = 10;

void setTimer0(int duration){
	timer0_counter = duration / TIMER_CYCLE;
	timer0_flag = 0;
}

void setTimer1(int duration){
	timer1_counter = duration / TIMER_CYCLE;
	timer1_flag = 0;
}

//int isTimerExpired(void){
//	if (timer_flag == 1){
//		timer_flag = 0;
//		return 1;
//	}
//	return 0;
//}

void timer_run(void){
	if(timer0_counter > 0){
		timer0_counter--;
		if(timer0_counter <= 0){
			timer0_flag = 1;
		}
	}
	if(timer1_counter > 0){
		timer1_counter--;
		if(timer1_counter <= 0){
			timer1_flag = 1;
		}
	}
}
void timer_expire(void){
	timer0_counter = 0;
	timer1_counter = 0;
	timer0_flag = 0;
	timer1_flag = 0;
}
