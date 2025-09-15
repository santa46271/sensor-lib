#pragma once

#include <stdint.h>
// typedef union {
//     uint16_t raw;
//     struct {
//         uint16_t dt_day : 5;
//         uint16_t dt_quarter_date : 2;
//         uint16_t dt_month_in_quarter : 2;
    
//         uint16_t tm_hours : 5;
//         uint16_t tm_quarter_time : 2;
    
//     } bits;
// } Date_time;

typedef uint16_t Date_time;

typedef enum Quarter {
    UNKNOWN_QUARTER = 0,
    First_quarter = 1,
    Second_quarter,
    Third_quarter,
    Fourth_quarter
} Quarter;

typedef struct {
    int hour;
    Quarter quarter_hour;
} Time;

typedef enum Month {
    UNKNOWN_MONTH =0,
    Jan =1,
    Feb,
    Mar,
    Apr,
    May,
    Jun,
    Jul,
    Aug,
    Sep,
    Oct,
    Nov,
    Dec
} Month;

typedef struct {
    int day;
    Month month;
} Date;

int init_time(Time* tm, int hour, int min);

int set_month(Date_time* dt, Month month);
int set_day(Date_time* dt, int day);
int set_time(Date_time* dt, Time tm);
int set_date(Date_time* dt, Date date);

Month get_month(Date_time* dt);
Date get_date(Date_time* date_time);
Time get_time(Date_time* date_time);
