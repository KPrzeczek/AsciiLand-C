#include "clock.h"

#include <time.h>
#include <stdio.h>

double Clock_TimeAsSeconds()
{
    return time(NULL);
}

void Clock_PrintCurrentTime()
{
    time_t raw_time;
    struct tm *time_info;
    
    time(&raw_time);
    time_info = localtime(&raw_time);
    printf("Current Time: %s\n", asctime(time_info));
}
