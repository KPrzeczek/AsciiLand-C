#ifndef SYSTEMS_H
#define SYSTEMS_H

#include "ecs.h"
#include "../map/map.h"

void System_Input(World *world);
void System_Movement(World *world);

void System_Physics(World *world);
void System_Collision(World *world, MapData *map);

void System_Portal(World *world, FileData *data, MapData *map);
void System_Render(World *world);

void System_DrawStats(World *world);
void System_DrawInventory(World *world);

void System_InventoryLogic(World *world);
void System_ItemPickup(World *world);
void System_ArmourStatLogic(World *world);

#endif //SYSTEMS_H
