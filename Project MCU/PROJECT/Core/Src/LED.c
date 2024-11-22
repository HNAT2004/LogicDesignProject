/*
 * LED.c
 *
 *  Created on: Nov 7, 2024
 *      Author: ADMIN
 */
#include "LED.h"
#include "main.h"
void Init_Led(void){
	  HAL_GPIO_WritePin(LED1_GPIO_Port, LED1_Pin, RESET);
	  HAL_GPIO_WritePin(LED2_GPIO_Port, LED2_Pin, RESET);
}
void LED_RED(void){
	  HAL_GPIO_WritePin(LED1_GPIO_Port, LED1_Pin, SET);
	  HAL_GPIO_WritePin(LED2_GPIO_Port, LED2_Pin, SET);
}
void LED_YELLOW(void){
	  HAL_GPIO_WritePin(LED1_GPIO_Port, LED1_Pin, RESET);
	  HAL_GPIO_WritePin(LED2_GPIO_Port, LED2_Pin, SET);
}
void LED_GREEN(void){
	  HAL_GPIO_WritePin(LED1_GPIO_Port, LED1_Pin, SET);
	  HAL_GPIO_WritePin(LED2_GPIO_Port, LED2_Pin, RESET);
}
