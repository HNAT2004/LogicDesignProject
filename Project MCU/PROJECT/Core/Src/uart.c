/*
 * uart.c
 *
 *  Created on: Nov 11, 2024
 *      Author: ADMIN
 */

#include "uart.h"
#include "global.h"
#include "stdint.h"
#include "string.h"
#include "main.h"
#include "software_timer.h"
extern UART_HandleTypeDef huart1;

void send_hello(void) {
    char msg[] = "hello";
    HAL_UART_Transmit(&huart1, (uint8_t*)msg, strlen(msg), HAL_MAX_DELAY);
}
