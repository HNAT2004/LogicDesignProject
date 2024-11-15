/*
 * software_timer.h
 *
 *  Created on: Sep 23, 2024
 *      Author: ADMIN
 */

#ifndef INC_SOFTWARE_TIMER_H_
#define INC_SOFTWARE_TIMER_H_

extern int timer0_flag;
extern int timer1_flag;
extern int hour;
extern int minute;
extern int second;

void setTimer0(int x);
void setTimer1(int y);

int isTimerExpired(void);
void timer_run(void);
void display7SEG(int number);
void update7SEG(int index);
void updateClockBuffer(void);
void timer_expire(void);
#endif /* INC_SOFTWARE_TIMER_H_ */
