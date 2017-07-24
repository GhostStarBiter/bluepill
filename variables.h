//
// Created by mrgreen on 05.07.17.
//

#ifndef __VARIABLES_H
#define __VARIABLES_H

#include <stdint-gcc.h>

#define STEP_SPEED_DELAY 3
#define IMPULSE_TO_LOCK 40
#define IMPULSE_TO_OPEN 40
#define CLOSE 1
#define OPEN 0
#define ALARM_QUANTITY 6
#define UART_RECV_BUF_SIZE 5
#define UART_RECV_MSG_SIZE 20
#define STRINGS_EQUAL 0
#define ON_START_WAIT_TO_CLOSE 10000

//static uint8_t lock_button;
//static uint8_t ctrl_button;

struct time{
    uint8_t hour;
    uint8_t min;
    uint8_t sec;
};

struct day_schedule{
    struct time open_time[ALARM_QUANTITY];
    struct time close_time[ALARM_QUANTITY];
};

struct week_schedule{
    uint8_t sun_enable;
    uint8_t mon_enable;
    uint8_t tue_enable;
    uint8_t wed_enable;
    uint8_t thu_enable;
    uint8_t fri_enable;
    uint8_t sat_enable;
    struct day_schedule sunday;
    struct day_schedule monday;
    struct day_schedule tuesday;
    struct day_schedule wednesday;
    struct day_schedule thursday;
    struct day_schedule friday;
    struct day_schedule saturday;
};

void InitSchedule(struct week_schedule*);

#endif //BLUEPILL_VARIABLES_H
