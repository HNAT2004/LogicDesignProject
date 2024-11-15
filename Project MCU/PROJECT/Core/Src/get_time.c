/*
 * get_time.c
 *
 *  Created on: Nov 7, 2024
 *      Author: ADMIN
 */
#include "main.h"
#include "stdio.h"
#include "stm32f1xx_hal.h"  // Thay thế bằng thư viện HAL phù hợp với STM32 của bạn

RTC_TimeTypeDef sTime;
RTC_DateTypeDef sDate;
extern RTC_HandleTypeDef hrtc;
// Khai báo các biến chuỗi cho ngày và giờ
char time_str[9];  // Chuỗi cho thời gian (HH:MM:SS)
char date_str[11]; // Chuỗi cho ngày (DD-MM-YYYY)

void Update_Date_String(void)
{
    // Lấy ngày hiện tại
    HAL_RTC_GetDate(&hrtc, &sDate, RTC_FORMAT_BIN);

    // Format ngày vào chuỗi date_str
    sprintf(date_str, "%02d-%02d-20%02d", sDate.Date, sDate.Month, sDate.Year);
}

void Update_Time_String(void)
{
    // Lấy giờ hiện tại
    HAL_RTC_GetTime(&hrtc, &sTime, RTC_FORMAT_BIN);

    // Format giờ vào chuỗi time_str
    sprintf(time_str, "%02d:%02d:%02d", sTime.Hours, sTime.Minutes, sTime.Seconds);
}


