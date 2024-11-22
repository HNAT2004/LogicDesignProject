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
uint8_t command_flag = 0;
uint8_t temp = 0;
uint8_t buffer_flag = 0;
extern UART_HandleTypeDef huart2;
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart) {
    if (huart->Instance == USART2) {
        buffer_flag = 1;  // Đánh dấu đã nhận được dữ liệu
        HAL_UART_Receive_IT(&huart2, &temp, 1); // Tiếp tục nhận dữ liệu
    }
}

void command_parser_fsm() {
    if (temp == '1') {  // Nhận được ký tự '1'
        HAL_GPIO_WritePin(LED_GPIO_Port, LED_Pin, GPIO_PIN_SET); // Đèn LED bật
        setTimer0(2000); // Ví dụ: Bật LED trong 2 giây
        lcd_init();
        status = ACCEPT;
        temp = 0; // Xóa giá trị để tránh xử lý lại
    } else if (temp == '2') {  // Nhận được ký tự '0'
        HAL_GPIO_WritePin(LED_GPIO_Port, LED_Pin, GPIO_PIN_RESET); // Đèn LED tắt
        setTimer0(2000); // Ví dụ: Tắt LED trong 2 giây
        lcd_init();
        status = DECLINE;
        temp = 0; // Xóa giá trị
    }
}

