/**
  ******************************************************************************
  * File Name          : main.c
  * Description        : Main program body
  ******************************************************************************
  ** This notice applies to any and all portions of this file
  * that are not between comment pairs USER CODE BEGIN and
  * USER CODE END. Other portions of this file, whether 
  * inserted by the user or by software development tools
  * are owned by their respective copyright owners.
  *
  * COPYRIGHT(c) 2017 STMicroelectronics
  *
  * Redistribution and use in source and binary forms, with or without modification,
  * are permitted provided that the following conditions are met:
  *   1. Redistributions of source code must retain the above copyright notice,
  *      this list of conditions and the following disclaimer.
  *   2. Redistributions in binary form must reproduce the above copyright notice,
  *      this list of conditions and the following disclaimer in the documentation
  *      and/or other materials provided with the distribution.
  *   3. Neither the name of STMicroelectronics nor the names of its contributors
  *      may be used to endorse or promote products derived from this software
  *      without specific prior written permission.
  *
  * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
  * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
  * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
  * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
  * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
  * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
  * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
  * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
  * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
  * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
  *
  ******************************************************************************
  */
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "stm32f1xx_hal.h"
#include "rtc.h"
#include "usart.h"
#include "gpio.h"

/* USER CODE BEGIN Includes */
#include "variables.h"
#include <string.h>
#include <stm32f1xx_hal_rtc.h>
#include <stdlib.h>
/* USER CODE END Includes */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */
/* Private variables ---------------------------------------------------------*/
volatile uint8_t sw_closed;
volatile uint8_t action;
volatile uint8_t alarm_chosen;
static uint8_t cnt = 0;
uint16_t recv_msg_len;

RTC_TimeTypeDef RTC_Time;
RTC_DateTypeDef RTC_Day;
RTC_AlarmTypeDef RTC_Alarm;
struct time* next_alarm;
static uint8_t receive_buf[UART_RECV_MSG_SIZE+1];
uint8_t uart_1_buf[UART_RECV_BUF_SIZE];

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);

/* USER CODE BEGIN PFP */
/* Private function prototypes -----------------------------------------------*/

void configAlarm(void);
void choseAlarm(struct day_schedule*);
void fault(void);
void NumToChar(uint8_t, char[2]);
uint8_t SetDate(uint8_t*);
uint8_t SetOpenTime(uint8_t*);
uint8_t SetCloseTime(uint8_t*);

/* USER CODE END PFP */

/* USER CODE BEGIN 0 */

/**********************************************
 * STRUCTURE THAT HOLDS USER DEFINED SCHEDULE**
 *********************************************/

struct week_schedule user_schedule;

/* USER CODE END 0 */

int main(void)
{

  /* USER CODE BEGIN 1 */

  /* USER CODE END 1 */

  /* MCU Configuration----------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */
    uart_1_buf[0] = 0;
  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_RTC_Init();
  MX_USART1_UART_Init();
  MX_USART2_UART_Init();

  /* USER CODE BEGIN 2 */

  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */

    // turn off on-board LED
    HAL_GPIO_WritePin(LED_GPIO_Port, LED_Pin, GPIO_PIN_SET);

    HAL_GPIO_WritePin(O_STATUS_LED_GPIO_Port, O_STATUS_LED_Pin, GPIO_PIN_RESET);
    /** enable Bluetooth module **/
    HAL_GPIO_WritePin(BT_EN_GPIO_Port, BT_EN_Pin, GPIO_PIN_SET);

    /** check the Bluetooth module is running **/
    while(HAL_GPIO_ReadPin(BLE_STATE_GPIO_Port, BLE_STATE_Pin) != GPIO_PIN_SET)
        fault();

    /** Fill the structure "user_schedule" with values from file variable.c **/
    InitSchedule(&user_schedule);
    /** set up variable to 0 to force choosing the alarm **/
    alarm_chosen = 0;
    /** current time **/
    HAL_RTC_GetTime(&hrtc, &RTC_Time, RTC_FORMAT_BCD);
    HAL_UART_Transmit(&huart1, (uint8_t*)"Current time ", (uint16_t)strlen("Current time "), 150);

    char cur_hour[2] = {0};
    NumToChar(RTC_Time.Hours, cur_hour);
    HAL_UART_Transmit(&huart1, (uint8_t*) cur_hour, 2, 50);
    //  //
    HAL_UART_Transmit(&huart1, (uint8_t*) ":", 1, 5);
    //  //
    char cur_min[2] = {0};
    NumToChar(RTC_Time.Minutes, cur_min);
    HAL_UART_Transmit(&huart1, (uint8_t*) cur_min, 2, 50);

    HAL_UART_Transmit(&huart1, (uint8_t*)"\n", 1, 5);

    HAL_Delay(1000);

    /** Delay before closing the Lock**/
    HAL_UART_Transmit(&huart1, (uint8_t*)"Lock after 10 sec.", (uint16_t) strlen("Lock after 5 sec."), 50);
    HAL_Delay(ON_START_WAIT_TO_CLOSE);
    /***************************************
     *         Locking the mechanism
     * (to make open-time alarm go first)
     * *************************************/
    LockMechanism();

    /****** INITIALIZATION OF TIME, DATE AND ALARMS SET FROM USART CONNECTION ******
     * Procedure of getting time, date and user schedule from USART_1 (from Bluetooth module):
     *
     * 1) enter current time in 12H format; ->check ->response "Thank you" or "Wrong value!"
     * 2) enter current day of the week; -> check ->response "Thank you" or "Wrong value!"
     * 3) enter first open-time in 12H format; -> check
     *          ->response "Enter close-time in 12H format" -> check ->response "Five sets left" or "Wrong value"
     * **/



    /* HAL_RTC_SetTime(*value get from usart(bluetooth)*)*/
    /* HAL_RTC_SetDate(*value get from usart(bluetooth)*)*/

    /* USER CODE END 3 */

#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wmissing-noreturn"
while (1)
{

    HAL_UART_Receive_IT(&huart1, uart_1_buf, UART_RECV_BUF_SIZE);
    /** Alarm configured and waiting for action**/

    if(alarm_chosen){

    }
    else
    //**Alarm gone and the new one need to be chosen
    {
        //**Call function to chose the next alarm from user defined schedule
        configAlarm();
        if(next_alarm == NULL){
            fault();
            continue;
        }
        RTC_Alarm.Alarm = RTC_ALARM_A;
        RTC_Alarm.AlarmTime.Hours = next_alarm->hour;
        RTC_Alarm.AlarmTime.Minutes = next_alarm->min;
        RTC_Alarm.AlarmTime.Seconds = next_alarm->sec;
        HAL_RTC_SetAlarm_IT(&hrtc, &RTC_Alarm, RTC_FORMAT_BCD);

        HAL_UART_Transmit(&huart1, (uint8_t*)"Alarm time ", (uint16_t)strlen("Alarm time "), 150);

        char hour[2] = {0};
        NumToChar(next_alarm->hour, hour);
        HAL_UART_Transmit(&huart1, (uint8_t*) hour, 2, 50);

        HAL_UART_Transmit(&huart1, (uint8_t*) ":", 1, 5);

        char minutes[2] = {0};
        NumToChar(next_alarm->min, minutes);
        HAL_UART_Transmit(&huart1, (uint8_t*) minutes, 2, 50);

    }

    //HAL_PWR_EnterSLEEPMode(0, PWR_SLEEPENTRY_WFI);

  /* USER CODE END WHILE */

  /* USER CODE BEGIN 3 */

  }
#pragma clang diagnostic pop
  /* USER CODE END 3 */

}

/** System Clock Configuration
*/
void SystemClock_Config(void)
{

  RCC_OscInitTypeDef RCC_OscInitStruct;
  RCC_ClkInitTypeDef RCC_ClkInitStruct;
  RCC_PeriphCLKInitTypeDef PeriphClkInit;

    /**Initializes the CPU, AHB and APB busses clocks 
    */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI|RCC_OSCILLATORTYPE_LSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = 16;
  RCC_OscInitStruct.LSIState = RCC_LSI_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_NONE;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    _Error_Handler(__FILE__, __LINE__);
  }

    /**Initializes the CPU, AHB and APB busses clocks 
    */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_HSI;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_0) != HAL_OK)
  {
    _Error_Handler(__FILE__, __LINE__);
  }

  PeriphClkInit.PeriphClockSelection = RCC_PERIPHCLK_RTC;
  PeriphClkInit.RTCClockSelection = RCC_RTCCLKSOURCE_LSI;
  if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInit) != HAL_OK)
  {
    _Error_Handler(__FILE__, __LINE__);
  }

    /**Configure the Systick interrupt time 
    */
  HAL_SYSTICK_Config(HAL_RCC_GetHCLKFreq()/1000);

    /**Configure the Systick 
    */
  HAL_SYSTICK_CLKSourceConfig(SYSTICK_CLKSOURCE_HCLK);

  /* SysTick_IRQn interrupt configuration */
  HAL_NVIC_SetPriority(SysTick_IRQn, 0, 0);
}

/* USER CODE BEGIN 4 */

/**
     * input format:
     *
     *          day FRIDAY
     *          current time 10:17
     *          opentime 12:32 (up to 6 values)
     *          closetime 16:45 (up to 6 values)
     *
     *
     *          HAL_GPIO_TogglePin(LED_GPIO_Port, LED_Pin);
                HAL_Delay(500);
                HAL_GPIO_WritePin(LED_GPIO_Port, LED_Pin, GPIO_PIN_SET);
                HAL_Delay(500);

                for(int i = 0; i < 4; i++){
                    HAL_GPIO_WritePin(O_STATUS_LED_GPIO_Port, O_STATUS_LED_Pin, GPIO_PIN_SET);
                    HAL_Delay(300);
                    HAL_GPIO_WritePin(O_STATUS_LED_GPIO_Port, O_STATUS_LED_Pin, GPIO_PIN_RESET);
                    HAL_Delay(300);
                }


     * **/
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart){

    if(strstr((char*) uart_1_buf, "open\n") != NULL){
        OpenMechanism();
        memset(uart_1_buf, 0, UART_RECV_BUF_SIZE);
    }
    else
    if(strstr((char*) uart_1_buf, "clos\n") != NULL){
        LockMechanism();
        memset(uart_1_buf, 0, UART_RECV_BUF_SIZE);
    }
    memset(uart_1_buf, 0, UART_RECV_BUF_SIZE);
        /** Set the current date **/
/*
        if(strncmp((char*) receive_buf, "day ", strlen("day ")) == STRINGS_EQUAL){
            if(!SetDate(&receive_buf[strlen("day ")])){
                char* incorrect_date = "Invalid day of week. Set default: SUNDAY.";
                HAL_UART_Transmit(&huart1, (uint8_t*) incorrect_date, (uint16_t) strlen(incorrect_date), 100);
                RTC_Day.WeekDay = RTC_WEEKDAY_SUNDAY;
                HAL_RTC_SetDate(&hrtc, &RTC_Day, RTC_FORMAT_BCD);
            }
        }
        else
*/
        /** Set open-time **/
/*
        if(strncmp((char*) receive_buf, "opentime ", strlen("opentime ")) == STRINGS_EQUAL){
            if(!SetOpenTime(&receive_buf[strlen("opentime ")])){
                char* incorrect_opentime = "Invalid opentime. Set default: 0:00.";
                HAL_UART_Transmit(&huart1, (uint8_t*) incorrect_opentime, (uint16_t) strlen(incorrect_opentime), 100);
                SetOpenTime(0x00);
            }
        }
        else
*/
        /** Set close-time **/
/*
        if(strncmp((char*) receive_buf, "closetime ", strlen("closetime ")) == STRINGS_EQUAL){

        }
*/
}

uint8_t SetOpenTime(uint8_t* received_opentime){
    uint8_t hour[2] = {0};
    uint8_t min[2] = {0};
    uint8_t result = 0;
    int k = 0;
    for(int i = 0; i < (recv_msg_len - strlen("opentime ")) && i < 5; i++){
        //NaN check
        if(*(received_opentime+i) < 48 /*ASCII code of '0'*/
           &&
           *(received_opentime+i) > 58 /*ASCII code of ':'*/)
            return result;
        else
        // skip if byte holds delimiter ':'
        if(*(received_opentime+i) == 58 /*ASCII code of ':'*/){
            continue;
        }
        else
        if(i < 2){
            hour[i] = *(received_opentime+i);
        }
        else{
            min[k] = *(received_opentime+i);
            k++;
        }
    }
    if((hour[0] - 48) > 2 || (hour[1] - 48) > 3)
        return result;
    if((min[0] - 48) > 5)
        return result;
    int set_hour = (hour[0] - 48)*10 + (hour[1] - 48);
    int set_min = (min[0] - 48)*10 + (min[1] - 48);
    RTC_DateTypeDef day;
    HAL_RTC_GetDate(&hrtc, &day, RTC_FORMAT_BCD);
    return result;   // if incorrect opentime
}

uint8_t SetDate(uint8_t* received_date){
    char* date_set = "Date set complete.";
    if(strstr((char*) received_date, "SUN") != NULL){
        RTC_Day.WeekDay = RTC_WEEKDAY_SUNDAY;
        HAL_RTC_SetDate(&hrtc, &RTC_Day, RTC_FORMAT_BCD);

        HAL_UART_Transmit(&huart1, (uint8_t*) date_set, (uint16_t) strlen(date_set), 100);
        return 1;
    } else
    if(strstr((char*) received_date, "MON") != NULL){
        RTC_Day.WeekDay = RTC_WEEKDAY_MONDAY;
        HAL_RTC_SetDate(&hrtc, &RTC_Day, RTC_FORMAT_BCD);
        HAL_UART_Transmit(&huart1, (uint8_t*) date_set, (uint16_t) strlen(date_set), 100);
        return 1;
    } else
    if(strstr((char*) received_date, "TUE") != NULL){
        RTC_Day.WeekDay = RTC_WEEKDAY_TUESDAY;
        HAL_RTC_SetDate(&hrtc, &RTC_Day, RTC_FORMAT_BCD);
        HAL_UART_Transmit(&huart1, (uint8_t*) date_set, (uint16_t) strlen(date_set), 100);
        return 1;
    } else
    if(strstr((char*) received_date, "WED") != NULL){
        RTC_Day.WeekDay = RTC_WEEKDAY_WEDNESDAY;
        HAL_RTC_SetDate(&hrtc, &RTC_Day, RTC_FORMAT_BCD);
        HAL_UART_Transmit(&huart1, (uint8_t*) date_set, (uint16_t) strlen(date_set), 100);
        return 1;
    } else
    if(strstr((char*) received_date, "THU") != NULL){
        RTC_Day.WeekDay = RTC_WEEKDAY_THURSDAY;
        HAL_RTC_SetDate(&hrtc, &RTC_Day, RTC_FORMAT_BCD);
        HAL_UART_Transmit(&huart1, (uint8_t*) date_set, (uint16_t) strlen(date_set), 100);
        return 1;
    } else
    if(strstr((char*) received_date, "FRI") != NULL){
        RTC_Day.WeekDay = RTC_WEEKDAY_FRIDAY;
        HAL_RTC_SetDate(&hrtc, &RTC_Day, RTC_FORMAT_BCD);
        HAL_UART_Transmit(&huart1, (uint8_t*) date_set, (uint16_t) strlen(date_set), 100);
        return 1;
    } else
    if(strstr((char*) received_date, "SAT") != NULL){
        RTC_Day.WeekDay = RTC_WEEKDAY_SATURDAY;
        HAL_RTC_SetDate(&hrtc, &RTC_Day, RTC_FORMAT_BCD);
        HAL_UART_Transmit(&huart1, (uint8_t*) date_set, (uint16_t) strlen(date_set), 100);
        return 1;
    } else
    return 0;   //if incorrect date
}

void fault(void){
    HAL_GPIO_TogglePin(O_STATUS_LED_GPIO_Port, O_STATUS_LED_Pin);
    HAL_Delay(200);
    HAL_GPIO_WritePin(O_STATUS_LED_GPIO_Port, O_STATUS_LED_Pin, GPIO_PIN_RESET);
    HAL_Delay(200);
}

void NumToChar(uint8_t Num, char* Char){
    uint8_t char_digit_offset = 48;
    uint8_t temp1 = (Num >> 4);
    uint8_t temp2 = (uint8_t) (Num & 0x0F);
    Char[0] = (char) (temp1 + char_digit_offset);
    Char[1] = (char) (temp2 + char_digit_offset);
}

void configAlarm(void){

    HAL_RTC_GetTime(&hrtc, &RTC_Time, RTC_FORMAT_BCD);
    HAL_Delay(10);
    HAL_RTC_GetDate(&hrtc, &RTC_Day, RTC_FORMAT_BCD);
    HAL_Delay(10);

    struct day_schedule* _p_day;

    if(RTC_Day.WeekDay == RTC_WEEKDAY_SUNDAY && user_schedule.sun_enable){
        _p_day = &user_schedule.sunday;
        choseAlarm(_p_day);
    } else
    if(RTC_Day.WeekDay == RTC_WEEKDAY_MONDAY && user_schedule.mon_enable){
        _p_day = &user_schedule.monday;
        choseAlarm(_p_day);
    } else
    if(RTC_Day.WeekDay == RTC_WEEKDAY_TUESDAY && user_schedule.tue_enable){
        _p_day = &user_schedule.tuesday;
        choseAlarm(_p_day);
    } else
    if(RTC_Day.WeekDay == RTC_WEEKDAY_WEDNESDAY && user_schedule.wed_enable){
        _p_day = &user_schedule.wednesday;
        choseAlarm(_p_day);
    } else
    if(RTC_Day.WeekDay == RTC_WEEKDAY_THURSDAY && user_schedule.thu_enable){
        _p_day = &user_schedule.thursday;
        choseAlarm(_p_day);
    } else
    if(RTC_Day.WeekDay == RTC_WEEKDAY_FRIDAY && user_schedule.fri_enable){
        _p_day = &user_schedule.friday;
        choseAlarm(_p_day);
    } else
    if(RTC_Day.WeekDay == RTC_WEEKDAY_SATURDAY && user_schedule.sat_enable){
        _p_day = &user_schedule.saturday;
        choseAlarm(_p_day);
    }
}

void choseAlarm(struct day_schedule* day){
    next_alarm = NULL;
    for(int i = 0; i < ALARM_QUANTITY; i++){
            /**OPEN SCENARIO**/
            if(day->open_time[i].hour >= RTC_Time.Hours
               && day->open_time[i].min > RTC_Time.Minutes
               && sw_closed
                  /*redundant. This check should be done then data gets from USART(Bluetooth)*/
               && day->open_time[i].hour <= day->close_time[i].hour
               && day->open_time[i].min < day->close_time[i].min)
            {
                /**default initialization time**/
                if(day->open_time[i].hour == 0x00
                   && day->open_time[i].min == 0x00){
                        continue;
                }
                /**=========**/
                action = OPEN;
                next_alarm = &(day->open_time[i]);
                alarm_chosen = 1;
                break;
            } else
                /**CLOSE SCENARIO**/
            if(day->close_time[i].hour >= RTC_Time.Hours
               && day->close_time[i].min > RTC_Time.Minutes
               && !sw_closed
                  /*redundant. This check should be done then data gets from USART(Bluetooth)*/
               && day->close_time[i].hour >= day->open_time[i].hour
               && day->close_time[i].min > day->open_time[i].min)
            {
                /**default initialization time**/
                if(day->close_time[i].hour == 0x00
                   && day->close_time[i].min == 0x00)
                        continue;
                /**=========**/
                action = CLOSE;
                next_alarm = &(day->close_time[i]);
                alarm_chosen = 1;
                break;
            }
    }
}

void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin){
    if(HAL_GPIO_ReadPin(I_CTRL_BUT_GPIO_Port, I_CTRL_BUT_Pin)){
        if(sw_closed == 1){
            OpenMechanism();
        }
        else
        if(sw_closed == 0)
        {
            LockMechanism();
        }
    }
}

void HAL_RTC_AlarmAEventCallback(RTC_HandleTypeDef *hrtc){

    if(action == CLOSE)
    {
        LockMechanism();
    }
    else
    if(action == OPEN)
    {
        OpenMechanism();
    }
    alarm_chosen = 0;
}



/**
 * This function sends impulses to stepper motor driver to close the lock
 */
void LockMechanism(void){

    HAL_GPIO_WritePin(O_STEP_IN1_GPIO_Port, O_STEP_IN1_Pin, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(O_STEP_IN2_GPIO_Port, O_STEP_IN2_Pin, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(O_STEP_IN3_GPIO_Port, O_STEP_IN3_Pin, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(O_STEP_IN4_GPIO_Port, O_STEP_IN4_Pin, GPIO_PIN_RESET);

    HAL_GPIO_WritePin(O_DRVR_SLEEP_GPIO_Port, O_DRVR_SLEEP_Pin, GPIO_PIN_SET);
    HAL_Delay(STEP_SPEED_DELAY);
    for(int i = 0; i < IMPULSE_TO_LOCK; i++){
        HAL_GPIO_WritePin(O_STEP_IN1_GPIO_Port, O_STEP_IN1_Pin, GPIO_PIN_SET);
        HAL_GPIO_WritePin(O_STEP_IN2_GPIO_Port, O_STEP_IN2_Pin, GPIO_PIN_RESET);
        HAL_GPIO_WritePin(O_STEP_IN3_GPIO_Port, O_STEP_IN3_Pin, GPIO_PIN_SET);
        HAL_GPIO_WritePin(O_STEP_IN4_GPIO_Port, O_STEP_IN4_Pin, GPIO_PIN_RESET);
        HAL_Delay(STEP_SPEED_DELAY);
        HAL_GPIO_WritePin(O_STEP_IN1_GPIO_Port, O_STEP_IN1_Pin, GPIO_PIN_RESET);
        HAL_GPIO_WritePin(O_STEP_IN2_GPIO_Port, O_STEP_IN2_Pin, GPIO_PIN_SET);
        HAL_GPIO_WritePin(O_STEP_IN3_GPIO_Port, O_STEP_IN3_Pin, GPIO_PIN_SET);
        HAL_GPIO_WritePin(O_STEP_IN4_GPIO_Port, O_STEP_IN4_Pin, GPIO_PIN_RESET);
        HAL_Delay(STEP_SPEED_DELAY);
        HAL_GPIO_WritePin(O_STEP_IN1_GPIO_Port, O_STEP_IN1_Pin, GPIO_PIN_RESET);
        HAL_GPIO_WritePin(O_STEP_IN2_GPIO_Port, O_STEP_IN2_Pin, GPIO_PIN_SET);
        HAL_GPIO_WritePin(O_STEP_IN3_GPIO_Port, O_STEP_IN3_Pin, GPIO_PIN_RESET);
        HAL_GPIO_WritePin(O_STEP_IN4_GPIO_Port, O_STEP_IN4_Pin, GPIO_PIN_SET);
        HAL_Delay(STEP_SPEED_DELAY);
        HAL_GPIO_WritePin(O_STEP_IN1_GPIO_Port, O_STEP_IN1_Pin, GPIO_PIN_SET);
        HAL_GPIO_WritePin(O_STEP_IN2_GPIO_Port, O_STEP_IN2_Pin, GPIO_PIN_RESET);
        HAL_GPIO_WritePin(O_STEP_IN3_GPIO_Port, O_STEP_IN3_Pin, GPIO_PIN_RESET);
        HAL_GPIO_WritePin(O_STEP_IN4_GPIO_Port, O_STEP_IN4_Pin, GPIO_PIN_SET);
        HAL_Delay(STEP_SPEED_DELAY);
    }
    HAL_GPIO_WritePin(O_STEP_IN1_GPIO_Port, O_STEP_IN1_Pin, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(O_STEP_IN2_GPIO_Port, O_STEP_IN2_Pin, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(O_STEP_IN3_GPIO_Port, O_STEP_IN3_Pin, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(O_STEP_IN4_GPIO_Port, O_STEP_IN4_Pin, GPIO_PIN_RESET);
    HAL_Delay(STEP_SPEED_DELAY);

    HAL_GPIO_WritePin(O_DRVR_SLEEP_GPIO_Port, O_DRVR_SLEEP_Pin, GPIO_PIN_RESET);
    HAL_UART_Transmit(&huart1, (uint8_t*)"Locked", (uint16_t) strlen("Locked"), 50);
    sw_closed = 1;
}

/**
 * This function sends impulses to stepper motor driver to open the lock
 */
void OpenMechanism(){

    HAL_GPIO_WritePin(O_STEP_IN1_GPIO_Port, O_STEP_IN1_Pin, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(O_STEP_IN2_GPIO_Port, O_STEP_IN2_Pin, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(O_STEP_IN3_GPIO_Port, O_STEP_IN3_Pin, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(O_STEP_IN4_GPIO_Port, O_STEP_IN4_Pin, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(O_DRVR_SLEEP_GPIO_Port, O_DRVR_SLEEP_Pin, GPIO_PIN_SET);
    HAL_Delay(STEP_SPEED_DELAY);

    for(int i = 0; i < IMPULSE_TO_OPEN; i++){
        HAL_GPIO_WritePin(O_STEP_IN1_GPIO_Port, O_STEP_IN1_Pin, GPIO_PIN_SET);
        HAL_GPIO_WritePin(O_STEP_IN2_GPIO_Port, O_STEP_IN2_Pin, GPIO_PIN_RESET);
        HAL_GPIO_WritePin(O_STEP_IN3_GPIO_Port, O_STEP_IN3_Pin, GPIO_PIN_RESET);
        HAL_GPIO_WritePin(O_STEP_IN4_GPIO_Port, O_STEP_IN4_Pin, GPIO_PIN_SET);
        HAL_Delay(STEP_SPEED_DELAY);
        HAL_GPIO_WritePin(O_STEP_IN1_GPIO_Port, O_STEP_IN1_Pin, GPIO_PIN_RESET);
        HAL_GPIO_WritePin(O_STEP_IN2_GPIO_Port, O_STEP_IN2_Pin, GPIO_PIN_SET);
        HAL_GPIO_WritePin(O_STEP_IN3_GPIO_Port, O_STEP_IN3_Pin, GPIO_PIN_RESET);
        HAL_GPIO_WritePin(O_STEP_IN4_GPIO_Port, O_STEP_IN4_Pin, GPIO_PIN_SET);
        HAL_Delay(STEP_SPEED_DELAY);
        HAL_GPIO_WritePin(O_STEP_IN1_GPIO_Port, O_STEP_IN1_Pin, GPIO_PIN_RESET);
        HAL_GPIO_WritePin(O_STEP_IN2_GPIO_Port, O_STEP_IN2_Pin, GPIO_PIN_SET);
        HAL_GPIO_WritePin(O_STEP_IN3_GPIO_Port, O_STEP_IN3_Pin, GPIO_PIN_SET);
        HAL_GPIO_WritePin(O_STEP_IN4_GPIO_Port, O_STEP_IN4_Pin, GPIO_PIN_RESET);
        HAL_Delay(STEP_SPEED_DELAY);
        HAL_GPIO_WritePin(O_STEP_IN1_GPIO_Port, O_STEP_IN1_Pin, GPIO_PIN_SET);
        HAL_GPIO_WritePin(O_STEP_IN2_GPIO_Port, O_STEP_IN2_Pin, GPIO_PIN_RESET);
        HAL_GPIO_WritePin(O_STEP_IN3_GPIO_Port, O_STEP_IN3_Pin, GPIO_PIN_SET);
        HAL_GPIO_WritePin(O_STEP_IN4_GPIO_Port, O_STEP_IN4_Pin, GPIO_PIN_RESET);
        HAL_Delay(STEP_SPEED_DELAY);
    }
    HAL_GPIO_WritePin(O_STEP_IN1_GPIO_Port, O_STEP_IN1_Pin, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(O_STEP_IN2_GPIO_Port, O_STEP_IN2_Pin, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(O_STEP_IN3_GPIO_Port, O_STEP_IN3_Pin, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(O_STEP_IN4_GPIO_Port, O_STEP_IN4_Pin, GPIO_PIN_RESET);
    HAL_Delay(STEP_SPEED_DELAY);

    HAL_GPIO_WritePin(O_DRVR_SLEEP_GPIO_Port, O_DRVR_SLEEP_Pin, GPIO_PIN_RESET);
    HAL_UART_Transmit(&huart1, (uint8_t*)"Opened", (uint16_t) strlen("Opened"), 50);
    sw_closed = 0;
}

/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @param  None
  * @retval None
  */
void _Error_Handler(char * file, int line)
{
  /* USER CODE BEGIN Error_Handler_Debug */

  /* User can add his own implementation to report the HAL error return state */
  while(1) 
  {
      HAL_GPIO_TogglePin(O_STATUS_LED_GPIO_Port, O_STATUS_LED_Pin);
      HAL_Delay(100);
  }
  /* USER CODE END Error_Handler_Debug */ 
}

#ifdef USE_FULL_ASSERT

/**
   * @brief Reports the name of the source file and the source line number
   * where the assert_param error has occurred.
   * @param file: pointer to the source file name
   * @param line: assert_param error line source number
   * @retval None
   */
void assert_failed(uint8_t* file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
    ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */

}

#endif

/**
  * @}
  */ 

/**
  * @}
*/ 

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
