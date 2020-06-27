#include "map_tiles.h"
#include "../util/console/console_funcs.h"

#include <stdlib.h>

const char solids[] = {
    '#', // Wall
    '%', // World Seperator
    'O', // Tree Leaf
    'Y'  // Tree Branch
};

const char items[] = {
    '$', // Food
    '*', // Usable
    '&'  // Equipable
};

const MapTiles_CharVecPair movements[] = {
    { .key = '<', .v2 = { -1,  0 } },
    { .key = '>', .v2 = {  1,  0 } },
    { .key = '^', .v2 = {  0, -1 } },
    { .key = 'v', .v2 = {  0,  1 } }
};

const MapTiles_CharColourPair colours[] = {
    // Player
    { .key = '@', .colour = FG_LIGHT_AQUA },
    
    // Items
    { .key = '$', .colour = FG_BRIGHT_WHITE },
    { .key = '*', .colour = FG_BRIGHT_WHITE },
    { .key = '&', .colour = FG_BRIGHT_WHITE },
    
    // Walls
    { .key = '#', .colour = FG_GREY },
    { .key = '%', .colour = FG_GREY },
    
    // Floors
    { .key = '\'', .colour = FG_DARK_YELLOW },
    
    // Room Movement Tiles
    { '^', FG_LIGHT_MAGENTA },
    { 'v', FG_LIGHT_MAGENTA },
    { '<', FG_LIGHT_MAGENTA },
    { '>', FG_LIGHT_MAGENTA },
    
    // Grass
    { .key = '.', .colour = FG_LIGHT_GREEN },
    { .key = ',', .colour = FG_DARK_GREEN },
    { .key = '|', .colour = FG_DARK_GREEN },
    
    // Tree
    { .key = 'O', .colour = FG_DARK_GREEN },
    { .key = 'Y', .colour = FG_DARK_YELLOW },
    
    // Water
    { .key = 'X', .colour = FG_WHITE },
    { .key = '~', .colour = FG_LIGHT_BLUE },
    { .key = '+', .colour = FG_DARK_BLUE }
};

Console_Colour MapTiles_GetColour(const char key)
{
    for (int ii = 0; ii < sizeof(colours)/sizeof(MapTiles_CharColourPair); ii++) {
        if (colours[ii].key == key) {
            return colours[ii].colour;
        }
    }
    
    return FG_WHITE;
}

V2 MapTiles_GetDirection(const char key)
{
    for (int ii = 0; ii < sizeof(movements)/sizeof(MapTiles_CharVecPair); ii++) {
        if (movements[ii].key == key) {
            return movements[ii].v2;
        }
    }
    
    return V2_Make(0, 0);
}

int MapTiles_SolidsContain(const char c)
{
    for (int ii = 0; ii < sizeof(solids)/sizeof(const char); ii++) {
        if (solids[ii] == c) {
            return 1;
        }
    }
    
    return 0;
}

int MapTiles_ItemsContain(const char c)
{
    for (int ii = 0; ii < sizeof(items)/sizeof(const char); ii++) {
        if (items[ii] == c) {
            return 1;
        }
    }
    
    return 0;
}

int MapTiles_MovementsContain(const char c)
{
    for (int ii = 0; ii < sizeof(movements)/sizeof(MapTiles_CharVecPair); ii++) {
        if (movements[ii].key == c) {
            return 1;
        }
    }
    
    return 0;
}
