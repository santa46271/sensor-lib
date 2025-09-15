#include "date_time.h"
#include <stdio.h>

#define IS_VALID_MIN(MIN) ((MIN)>=0 && (MIN)<60)
#define IS_VALID_HOUR(HOUR) ((HOUR)>=0 && (HOUR)<24)
#define IS_VALID_DAY(DAY) ((DAY)>=1 && (DAY)<32)

int get_quarter_date(Month month) {
    return (month-1)/3;
}

int get_quarter_in_month(Month month) {
    return (month-1)%3;
}

int init_time(Time* tm, int hour, int min) {
    if (tm == NULL) return 1;
    if(!IS_VALID_MIN(min) || !IS_VALID_HOUR(hour)) return 1;

    tm->hour = hour;
    tm->quarter_hour = min/15+1;
    return 0;
}

int set_month(Date_time* dt, Month month) {
    if (dt == NULL) return 1;

    *dt &= ~0xF000;
    *dt |= (get_quarter_date(month) & 0x03) <<12;
    *dt |= (get_quarter_in_month(month) & 0x03) <<14;
    return 0;
}

int set_day(Date_time* dt, int day) {
    if(dt == NULL) return 1;
    if(!IS_VALID_DAY(day)) return 1;

    *dt &= ~(0x1F<<7);
    *dt |= (day & 0x1F)<<7;
    return 0;
}

int set_time(Date_time* dt, Time tm) {
    if (dt == NULL) return 1;

    *dt &= ~0x7F;
    *dt |= tm.hour & 0x1F;
    *dt |= ((tm.quarter_hour-1) & 0x03)<<5;
    return 0;
}

int set_date(Date_time* dt, Date date) {
    if (dt == NULL) return 1;

    int err = set_month(dt, date.month);
    if (err) return err;
    
    err = set_day(dt, date.day);

    return err;
}

Month get_month(Date_time* dt) {
    if(dt == NULL) return UNKNOWN_MONTH;
    int quarter = (*dt>>12) & 0x03;
    int quarter_in_month = (*dt>>14) & 0x03;
    return quarter*3 + quarter_in_month + 1;
}

Date get_date(Date_time* dt) {
    Date date;
    if (dt == NULL) {
        date.day =0;
        date.month = UNKNOWN_MONTH;
        return date;
    }

    date.day = (*dt>>7) & 0x1F;
    date.month = get_month(dt);

    return date;
}

Time get_time(Date_time* dt) {
    Time time;
    if (dt == NULL) {
        time.hour =0;
        time.quarter_hour = UNKNOWN_QUARTER;
    }

    time.hour = (*dt) & 0x1F;
    time.quarter_hour = ((*dt >> 5) & 0x03) + 1;

    return time;
}
