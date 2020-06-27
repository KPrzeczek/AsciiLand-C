#include "game_funcs.h"
#include "../../game.h"

#include <stdio.h>
#include <windows.h>
#include <string.h>

void GameFuncs_DrawSeperator(int x, int y, 
                             const char *pattern,
                             unsigned int len)
{
    int xx = x;
    for (int ii = 0; ii < len; ii++)
    {
        Game_DrawText(xx, y, pattern);
        xx += strlen(pattern);
    }
}

void GameFuncs_DrawSeperatorCol(int x, int y,
                                char c, unsigned int len,
                                SEP_DIR dir, WORD col)
{
    if (dir == SEPDIR_HORIZONTAL) {
        for (int xx = 0; xx < len; xx++) {
            Game_PutCharCol(x+xx, y, c, col);
        }
    } else if (dir == SEPDIR_VERTICAL) {
        for (int yy = 0; yy < len; yy++) {
            Game_PutCharCol(x, y+yy, c, col);
        }
    }
}
