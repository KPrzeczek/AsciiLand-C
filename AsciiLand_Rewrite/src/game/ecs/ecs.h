#ifndef ECS_H
#define ECS_H

#define MAX_ENTITIES 32

#include "components/physics/transform_components.h"
#include "components/physics/physics_components.h"
#include "components/inventory/inventory_components.h"
#include "components/visual/sprite_components.h"

typedef struct InputComponent {
    // Player Movement
    int move_up;
    int move_down;
    int move_left;
    int move_right;
    int move_input_x;
    int move_input_y;
    
    // Inventory Controls
    int inv_scroll_up;
    int inv_scroll_down;
    int inv_scroll_input;
    int inv_scroll_amt;
    int inv_use_item_key;
    int inv_use_item;
} InputComponent;

//===================//

typedef enum ComponentFlag {
    COMPONENT_NONE        = 0,
    COMPONENT_POSITION    = 1 << 0,
    COMPONENT_VELOCITY    = 1 << 1,
    COMPONENT_INPUT       = 1 << 2,
    COMPONENT_SPRITE      = 1 << 3,
    COMPONENT_COLLIDER    = 1 << 4,
    COMPONENT_STATS       = 1 << 5,
    COMPONENT_INVENTORY   = 1 << 6,
    COMPONENT_ITEM        = 1 << 7,
    COMPONENT_PORTAL_PASS = 1 << 8,
    COMPONENT_ARMOUR      = 1 << 9,
} ComponentFlag;

typedef struct World {
    int mask[MAX_ENTITIES];
    PositionComponent position_components[MAX_ENTITIES];
    VelocityComponent velocity_components[MAX_ENTITIES];
    InputComponent input_components[MAX_ENTITIES];
    SpriteComponent sprite_components[MAX_ENTITIES];
    StatsComponent stats_components[MAX_ENTITIES];
    InventoryComponent inventory_components[MAX_ENTITIES];
    ItemComponent item_components[MAX_ENTITIES];
    ArmourComponent armour_components[MAX_ENTITIES];
} World;

//=====[Entity]=====//

unsigned int World_CreateEntity(World *world);
void World_DestroyEntity(World *world, unsigned int id);
int World_HasComponent(World *world, unsigned int id, ComponentFlag flag);

//===================//

#endif //ECS_H
