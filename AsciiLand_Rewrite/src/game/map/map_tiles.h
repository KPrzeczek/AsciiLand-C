#ifndef MAP_TILES_H
#define MAP_TILES_H

#include "../util/console/console_colours.h"
#include "../util/maths/v2.h"

typedef struct MapTiles_CharColourPair {
    char key;
    Console_Colour colour;
} MapTiles_CharColourPair;

typedef struct MapTiles_CharVecPair {
    char key;
    V2 v2;
} MapTiles_CharVecPair;

extern const char solids[];
extern const char items[];
extern const MapTiles_CharVecPair movements[];
extern const MapTiles_CharColourPair colours[];

Console_Colour MapTiles_GetColour(const char key);
V2 MapTiles_GetDirection(const char key);

int MapTiles_MovementsContain(const char c);
int MapTiles_SolidsContain(const char c);
int MapTiles_ItemsContain(const char c);

#endif //MAP_TILES_H
