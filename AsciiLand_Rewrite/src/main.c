#include "game/game.h"
#include <windows.h>
#include <stdio.h>

int main()
{
    Game_ClearScreen();
    
    Game_Init(GAME_ST_PLAYING);
    Game_Run();
    Game_Exit();
    
    getchar();
    getchar();
    return 0;
} 
