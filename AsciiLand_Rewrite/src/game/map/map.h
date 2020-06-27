#ifndef MAP_H
#define MAP_H

#include "../util/maths/v2.h"
#include "../tools/loader.h"

typedef struct MapData {
    V2 world_position;
    char *contents;
    char *title;
} MapData;

MapData *Map_LoadMapData(FileData *data);
void Map_ReloadMapData(MapData *map, FileData *data);
void Map_Unload(MapData *data);

void Map_Set(MapData *map, FileData *data);
void Map_Draw(MapData *map);

char Map_GetAtI(MapData *map, int index);
char Map_GetAtV2(MapData *map, V2 v);

int Map_IsCharAtI(MapData *map, int i, char c);
int Map_IsCharAtV2(MapData *map, V2 v, char c);

#endif //MAP_H
