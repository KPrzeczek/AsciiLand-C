#include "game.h"

#include "states/state_playing.h"
#include "display_info.h"
#include "tools/logger.h"

#include <stdio.h>
#include <stdlib.h>

static Game_Pixel screen[SCREEN_WIDTH * SCREEN_HEIGHT];
static Game_GameState current_state = GAME_ST_NONE;
static HANDLE console;
static WORD default_colour = FG_WHITE | BG_BLACK;

void Game_Init(Game_GameState state)
{
    Logger_Init();
	console = CreateConsoleScreenBuffer(GENERIC_READ | GENERIC_WRITE, 0, NULL, CONSOLE_TEXTMODE_BUFFER, NULL);
	SetConsoleActiveScreenBuffer(console);
    Game_ChangeState(state);
}

int once = 1;

void Game_Run()
{
    while (current_state != GAME_ST_EXIT)
    {
        if (GetAsyncKeyState(VK_ESCAPE)) {
            current_state = GAME_ST_EXIT;
        }
        
        switch (current_state)
        {
            case GAME_ST_NONE: {
                printf("GAME_ERROR :: No state initialized!\n\n");
                break;
            } case GAME_ST_PLAYING: {
                if (once) {
                    StatePlaying_Init();
                    once = 0;
                }
                
                StatePlaying_Input();
                StatePlaying_Update();
                
                Game_ClearScreen();
                StatePlaying_Draw();
                
                break;
            }
        }
        
        DrawScreen();
    }
}

void Game_Exit()
{
    Logger_Close();
}

void Game_ChangeState(Game_GameState state)
{
    current_state = state;
}

void Game_PutChar(int x, int y, char c)
{
    Game_Pixel px = { .x = x, .y = y, .sprite = c, .colour = default_colour };
    screen[y * SCREEN_WIDTH + x] = px;
}

void Game_PutCharCol(int x, int y, char c, WORD colour)
{
    Game_Pixel px = { .x = x, .y = y, .sprite = c, .colour = colour };
    screen[y * SCREEN_WIDTH + x] = px;
}

void Game_DrawText(int x, int y, const char *t, ...)
{
    va_list args;
    va_start(args, t);
    
    char format[100];
    vsprintf(format, t, args);
    
    for (int ii = 0; ii < strlen(format); ii++) {
        Game_PutChar(x + ii, y, format[ii]);
    }
    
    va_end(args);
}

void Game_DrawTextCol(int x, int y, WORD colour, const char *t, ...)
{
    va_list args;
    va_start(args, t);
    
    char format[100];
    vsprintf(format, t, args);
    
    for (int ii = 0; ii < strlen(format); ii++) {
        Game_PutCharCol(x + ii, y, format[ii], colour);
    }
    
    va_end(args);
}

void Game_ClearScreen()
{
    for (int yy = 0; yy < SCREEN_HEIGHT; yy++) {
        for (int xx = 0; xx < SCREEN_WIDTH; xx++) {
            Game_PutChar(xx, yy, ' ');
        }
    }
}

// TODO(kmp): This is the least optimized, slowest, worst, shit ever oh my fuckign god hellp
static void DrawScreen()
{
    for (int ii = 0; ii < SCREEN_WIDTH * SCREEN_HEIGHT; ii++) {
        Game_Pixel *px = &screen[ii];
        COORD pos = { px->x, px->y };
        WriteConsoleOutputAttribute(console, &px->colour, 1, pos, &BYTES_WRITTEN);
        WriteConsoleOutputCharacter(console, &px->sprite, 1, pos, &BYTES_WRITTEN);
    }
}
