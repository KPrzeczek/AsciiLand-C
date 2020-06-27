#include "image.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../../game.h"

static const char *path = "C:/Users/KrysP/Desktop/AsciiLand_Rewrite/res/txrs/";
static const char *ext = ".ascimg";

Console_Colour draw_colour;

void Img_Init(Image *img)
{
    memset(img, 0, sizeof(Image));
}

void Img_LoadFromFile(Image *img, const char *name)
{
    const char *fullFilePath = getFullFilepath(name);
    FILE *f = fopen(fullFilePath, "r");
    
    char *buf = 0;
    long length;
    
    if (f) {
        fseek(f, 0, SEEK_END);
        length = ftell(f);
        
        fseek(f, 0, SEEK_SET);
        buf = malloc(length + 1);
        
        if (buf)
        {
            fread(buf, 1, length, f);
            *(buf + length) = 0;
        }
        
        fclose(f);
    }
    
    if (buf) {
        img->text = buf;
    }
}

void Img_SetContents(Image *img, char *content)
{
    img->text = content;
}

void Img_SetDrawColour(Console_Colour colour)
{
    draw_colour = colour;
}

void Img_Destroy(Image *img)
{
    if(img == 0) return;
    if(img->text) free(img->text);
    memset(img, 0, sizeof(Image));
}

void Img_Draw(int x, int y, Image *img)
{
    if(!img) return;
    if(!img->text) return;
    
    int yy = y;
    
    char *txt = img->text;
    char *current_line = txt;
    while(current_line)
    {
        char *next_line = strchr(current_line, '\n');
        if (next_line) *next_line = '\0';
        
        Game_DrawText(x, yy, current_line);
        yy++;
        
        if (next_line) *next_line = '\n';
        current_line = next_line ? (next_line + 1) : NULL;
    }
}

char Img_At(Image *img, int index)
{
    return (img->text[index]);
}

static const char *getFullFilepath(const char *name)
{
    char *buf = malloc(100);
    
    if (buf != NULL)
    {
        strcpy(buf, path);
        strcat(buf, name);
        strcat(buf, ext);
    }
    
    return buf;
}
