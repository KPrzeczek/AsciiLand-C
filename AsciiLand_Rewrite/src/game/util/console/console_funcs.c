#include "console_funcs.h"
#include "../../display_info.h"
#include "../../game.h"

#include <windows.h>
#include <stdio.h>

void Console_PrintError(int x, int y, const char *error)
{
    Game_DrawText(x, y, "ERROR :: %s\n", error);
}
