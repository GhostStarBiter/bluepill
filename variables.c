//
// Created by mrgreen on 12.07.17.
//

#include "variables.h"

void InitSchedule(struct week_schedule* schedule){
    /**Enable setting the schedule for days of week**/
    schedule->sun_enable = 1;
    schedule->mon_enable = 1;
    schedule->tue_enable = 1;
    schedule->wed_enable = 1;
    schedule->thu_enable = 1;
    schedule->fri_enable = 1;
    schedule->sat_enable = 1;
    //
    for(int i = 0; i < ALARM_QUANTITY; i++){
                /**SUNDAY**/
        schedule->sunday.open_time[i].hour = 0x0;
        schedule->sunday.open_time[i].min = 0x0;
        schedule->sunday.open_time[i].sec = 0x0;

        schedule->sunday.close_time[i].hour = 0x0;
        schedule->sunday.close_time[i].min = 0x0;
        schedule->sunday.close_time[i].sec = 0x0;
                /**MONDAY**/
        schedule->monday.open_time[i].hour = 0x0;
        schedule->monday.open_time[i].min = 0x0;
        schedule->monday.open_time[i].sec = 0x0;

        schedule->monday.close_time[i].hour = 0x0;
        schedule->monday.close_time[i].min = 0x0;
        schedule->monday.close_time[i].sec = 0x0;
                /**TUESDAY**/
        schedule->tuesday.open_time[i].hour = 0x0;
        schedule->tuesday.open_time[i].min = 0x0;
        schedule->tuesday.open_time[i].sec = 0x0;

        schedule->tuesday.close_time[i].hour = 0x0;
        schedule->tuesday.close_time[i].min = 0x0;
        schedule->tuesday.close_time[i].sec = 0x0;
                /**WEDNESDAY**/
        schedule->wednesday.open_time[i].hour = 0x0;
        schedule->wednesday.open_time[i].min = 0x0;
        schedule->wednesday.open_time[i].sec = 0x0;

        schedule->wednesday.close_time[i].hour = 0x0;
        schedule->wednesday.close_time[i].min = 0x0;
        schedule->wednesday.close_time[i].sec = 0x0;
                /**THURSDAY**/
        schedule->thursday.open_time[i].hour = 0x0;
        schedule->thursday.open_time[i].min = 0x0;
        schedule->thursday.open_time[i].sec = 0x0;

        schedule->thursday.close_time[i].hour = 0x0;
        schedule->thursday.close_time[i].min = 0x0;
        schedule->thursday.close_time[i].sec = 0x0;
                /**FRIDAY**/
        schedule->friday.open_time[i].hour = 0x0;
        schedule->friday.open_time[i].min = 0x0;
        schedule->friday.open_time[i].sec = 0x0;

        schedule->friday.close_time[i].hour = 0x0;
        schedule->friday.close_time[i].min = 0x0;
        schedule->friday.close_time[i].sec = 0x0;
                /**SATURDAY**/
        schedule->saturday.open_time[i].hour = 0x0;
        schedule->saturday.open_time[i].min = 0x0;
        schedule->saturday.open_time[i].sec = 0x0;

        schedule->saturday.close_time[i].hour = 0x0;
        schedule->saturday.close_time[i].min = 0x0;
        schedule->saturday.close_time[i].sec = 0x0;
    }

    schedule->sunday.open_time[0].hour = 0x15;
    schedule->sunday.open_time[0].min = 0x32;

    schedule->sunday.close_time[0].hour = 0x15;
    schedule->sunday.close_time[0].min = 0x35;

    schedule->sunday.open_time[1].hour = 0x15;
    schedule->sunday.open_time[1].min = 0x40;

    schedule->sunday.close_time[1].hour = 0x15;
    schedule->sunday.close_time[1].min = 0x45;

}

