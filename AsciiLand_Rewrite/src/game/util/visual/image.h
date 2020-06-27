#ifndef IMAGE_H
#define IMAGE_H

#include "../console/console_funcs.h"

typedef struct Image {
    char *text;
} Image;

void Img_Init(Image *img);
void Img_LoadFromFile(Image *img, const char *name);
void Img_SetContents(Image *img, char *content);

void Img_SetDrawColour(Console_Colour colour);

void Img_Destroy(Image *img);
void Img_Draw(int x, int y, Image *img);
char Img_At(Image *img, int index);

static const char *getFullFilepath(const char *name);

#endif //IMAGE_H
