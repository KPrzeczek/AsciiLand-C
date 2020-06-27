#include "map.h"
#include "map_tiles.h"

#include "../game.h"
#include "../display_info.h"
#include "../util/game/game_funcs.h"
#include "../tools/logger.h"

#include <string.h>
#include <stdio.h>

#define MAP_WIDTH 50

MapData *Map_LoadMapData(FileData *data)
{
    MapData *map_data = malloc(sizeof(MapData));
    memset(map_data, 0, sizeof(MapData));
    
    map_data->world_position = data->vecs[0];
    map_data->contents = data->strings[0];
    map_data->title = data->strings[1];
    
    return map_data;
}

void Map_ReloadMapData(MapData *map, FileData *data)
{
    Map_Unload(map);
    map = Map_LoadMapData(data);
}

void Map_Set(MapData *map, FileData *data)
{
    map->world_position = data->vecs[0];
    map->contents = data->strings[0];
    map->title = data->strings[1];
}

void Map_Unload(MapData *data)
{
    if (!data) return;
    if (data->contents) free(data->contents);
    if (data->title) free(data->title);
    free(data);
}

void Map_Draw(MapData *map)
{
    WORD sep_col = MapTiles_GetColour('%');
    
    // Draw Title
    char title[50];
    strcpy(title, map->title);
    strtok(title, "\n");
    for (int xx = 0; xx < SCREEN_WIDTH; xx++) {
        Game_PutCharCol(xx, 0, '=', sep_col);
    }
    Game_DrawText((SCREEN_WIDTH/2)-2-(strlen(title)/2), 0, "[ %s ]", title);
    
    // Draw Top Seperator
    for (int xx = 0; xx < SCREEN_WIDTH; xx++) {
        Game_PutCharCol(xx, 1, '%', sep_col);
    }
    
    // Draw Map
    V2 current_pos = { 0, 0 };
    for (int ii = 0; ii < strlen(map->contents); ii++) 
    {
        // Draw Map
        char current_tile = Map_GetAtV2(map, current_pos);
        WORD current_col = MapTiles_GetColour(current_tile);
        Game_PutCharCol(current_pos.x+1, current_pos.y+2, current_tile, current_col);
        
        // Increment XY
        current_pos.x++;
        if (current_pos.x >= MAP_WIDTH) {
            current_pos.x = 0;
            current_pos.y++;
        }
    }
    
    // Side Seperators
    for (int yy = 2; yy < SCREEN_HEIGHT-1; yy++) {  
        Game_PutCharCol(0, yy, '%', sep_col);
    } for (int yy = 2; yy < SCREEN_HEIGHT-1; yy++) {  
        Game_PutCharCol(SCREEN_WIDTH-1, yy, '%', sep_col);
    }
    
    // Draw Map-Info Seperator
    GameFuncs_DrawSeperatorCol(0, current_pos.y+2, '%', SCREEN_WIDTH, SEPDIR_HORIZONTAL, sep_col);
    
    // Draw Stats-Inventory Seperator
    for (int yy = 0; yy < 7; yy++) {
        Game_PutCharCol((int)SCREEN_WIDTH/2, SCREEN_HEIGHT-yy-1, '%', sep_col);
    }
    
    // Draw Bottom Seperator
    GameFuncs_DrawSeperatorCol(0, SCREEN_HEIGHT-1, '%', SCREEN_WIDTH, SEPDIR_HORIZONTAL, sep_col);
}

char Map_GetAtI(MapData *map, int index)
{
    return (map->contents[index]);
}

char Map_GetAtV2(MapData *map, V2 v)
{
    return (map->contents[v.y * MAP_WIDTH + v.x]);
}

int Map_IsCharAtI(MapData *map, int i, char c)
{
    return (Map_GetAtI(map, i) == c);
}

int Map_IsCharAtV2(MapData *map, V2 v, char c)
{
    return (Map_GetAtV2(map, v) == c);
}
