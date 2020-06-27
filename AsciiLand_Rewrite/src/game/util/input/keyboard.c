#include "keyboard.h"

#include <conio.h>
#include <stdio.h>

char Keyboard_GetKey()
{
    char in = ' ';
    if (kbhit())
        in = getch();
    return in;
}

char Keyboard_WaitForKey()
{
    char in = ' ';
    while (!kbhit())
        in = getch();
    return in;
}

void Keyboard_WaitForNoKey()
{
    while (kbhit());
}
