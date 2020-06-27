#include "logger.h"

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include <ctype.h>

static char current_file[128];

//==============================================//
#define FILE_PATH "D:/_Programming_Projects/C/AsciiLand_Rewrite/logs/"
#define FILE_EXT "_LOG.asclog"
//==============================================//

static void getFullFilePath(char *reciever, const char *name)
{
    char buf[128];
    strcpy(buf, FILE_PATH);
    strcat(buf, name);
    strcat(buf, FILE_EXT);
    strcpy(reciever, buf);
}

void Logger_Init()
{
    time_t raw_time = time(NULL);
    long int time_seconds = (long)raw_time;
    char time_seconds_char[16];
    itoa(time_seconds, time_seconds_char, 10);
    getFullFilePath(current_file, time_seconds_char);
}

void Logger_LogMessage(const char *logType, const char *msg, ...)
{
    static int msg_count = 0;
    
    FILE *f = fopen(current_file, "a");
    char log_type[32];
    strcpy(log_type, logType);
    
    va_list args;
    va_start(args, msg);
    
    char format[100];
    vsprintf(format, msg, args);
    
    for (int ii = 0; ii < strlen(log_type); ii++) {
        log_type[ii] = toupper(log_type[ii]);
    }
    
    if (f)
    {
        fprintf(f, "(%d)[%s] %s\n", msg_count, log_type, format);
        msg_count++;
        fclose(f);
    }
    
    va_end(args);
}

void Logger_Close()
{
}
