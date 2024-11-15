/*
 * uart_com.c
 *
 *  Created on: Nov 13, 2024
 *      Author: Genki
 */
#include "main.h"
#include "stdio.h"
#include "string.h"
#include "uart_com.h"
#include "software_timer.h"
#include "fsm.h"
#include "global.h"
#include "i2c-lcd.h"
#define MAX_BUFFER_SIZE 10
uint8_t command_flag = 0;
uint32_t ADC_value = 0;
uint8_t temp = 0;
uint8_t buffer[MAX_BUFFER_SIZE];
uint8_t index_buffer = 0;
uint8_t buffer_flag = 0;

extern UART_HandleTypeDef huart2;
void HAL_UART_RxCpltCallback(UART_HandleTypeDef * huart){
	if(huart -> Instance == USART2){
		buffer[index_buffer++] = temp;
		if(index_buffer == 30) index_buffer = 0;
		buffer_flag = 1;

		HAL_UART_Receive_IT(&huart2,&temp,1);
	}
}
void clear_buffer() {
	for (int i = 0; i < 10; i++) {
		buffer[i] = 0;
	}
	index_buffer = 0;
}
void command_parser_fsm(){
	if(strncmp((char*)buffer, "1", 1) == 0) {
		HAL_GPIO_TogglePin(LED_GPIO_Port, LED_Pin);
		setTimer0(2000);
		lcd_init();
		status = DECLINE;
		clear_buffer();
	}
	else if (buffer[index_buffer - 1] == '#'){
		HAL_GPIO_TogglePin(LED_GPIO_Port, LED_Pin);
		for (size_t i = 0; i < 4 - 1; i++) {
			printlcd[i] = (char)buffer[i];
		  }
		printlcd[3] = '\0'; // Ký tự kết thúc chuỗi
		setTimer0(2000);
		lcd_init();
		status = ACCEPT;
		clear_buffer();
	}
}
void uart_communication_fsm(){
	static char str[50];
	if(timer1_flag == 1){
		sprintf(str, "\r\n!ADC=%lu#", ADC_value);
		HAL_GPIO_TogglePin(LED_GPIO_Port, LED_Pin);
		setTimer1(300);
	}
}
