#ifndef LOGGER_H
#define LOGGER_H

#include <stdarg.h>

static void getFullFilePath(char *reciever, const char *name);
void Logger_Init();
void Logger_LogMessage(const char *logType, const char *msg, ...);
void Logger_Close();

#endif //LOGGER_H
