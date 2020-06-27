#ifndef GAME_FUNCS_H
#define GAME_FUNCS_H

#include <windows.h>

typedef enum SEP_DIR {
    SEPDIR_HORIZONTAL,
    SEPDIR_VERTICAL
} SEP_DIR;

void GameFuncs_DrawSeperator(int x, int y, const char *pattern, unsigned int len); // legacy seperator
void GameFuncs_DrawSeperatorCol(int x, int y, char c, unsigned int len, SEP_DIR dir, WORD col);

#endif //GAME_FUNCS_H
