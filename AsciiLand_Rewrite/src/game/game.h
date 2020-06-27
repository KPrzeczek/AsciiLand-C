#ifndef GAME_H
#define GAME_H

#include "util/console/console_colours.h"

#include <windows.h>
#include <stdarg.h>

typedef struct Game_Pixel {
    WORD colour;
    char sprite;
    int x, y;
} Game_Pixel;

typedef enum Game_GameState {
    GAME_ST_NONE,
    GAME_ST_PLAYING,
    GAME_ST_EXIT
} Game_GameState;

void Game_Init(Game_GameState state);
void Game_Run();
void Game_Exit();

void Game_ChangeState(Game_GameState state);

void Game_PutChar(int x, int y, char c);
void Game_PutCharCol(int x, int y, char c, WORD colour);

void Game_DrawText(int x, int y, const char *t, ...);
void Game_DrawTextCol(int x, int y, WORD colour, const char *t, ...);

void Game_ClearScreen();
static void DrawScreen();

#endif //GAME_H
