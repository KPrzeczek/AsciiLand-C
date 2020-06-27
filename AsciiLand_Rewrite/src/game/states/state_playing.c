#include "state_playing.h"
#include <windows.h>

#include "../ecs/ecs.h"
#include "../ecs/systems.h"

#include "../game.h"
#include "../map/map.h"

#include "../tools/loader.h"
#include "../tools/logger.h"

#include "../util/visual/image.h"

static World world;
static FileData *map_file;
static MapData *map_data;

void StatePlaying_Init()
{
    // Load Map File
    map_file = Loader_LoadFromFile("maps/0_0");
    if (map_file) {
        map_data = Map_LoadMapData(map_file);
    }
    
    // Allocate memory for world
    memset(&world, 0, sizeof(world));
    
    //==========[Apple Item]===========//
    {
        unsigned int apple_entity = World_CreateEntity(&world);
        world.mask[apple_entity] = COMPONENT_POSITION | COMPONENT_SPRITE | COMPONENT_ITEM;
        
        PositionComponent position = { .x = 30, .y = 11 };
        world.position_components[apple_entity] = position;
        
        SpriteComponent sprite = { .sprite = '$' };
        world.sprite_components[apple_entity] = sprite;
        
        ItemComponent item = {
            .name = "Apple",
            .type = ITEM_TYPE_FOOD,
            .item_data.food_data.hp_regen_amount = 5
        };
        world.item_components[apple_entity] = item;
    }
    //===================================//
    
    //==========[Crystal Ball Item]======//
    {
        unsigned int crystal_ball_entity = World_CreateEntity(&world);
        world.mask[crystal_ball_entity] = COMPONENT_POSITION | COMPONENT_SPRITE | COMPONENT_ITEM;
        
        PositionComponent position = { .x = 26, .y = 20 };
        world.position_components[crystal_ball_entity] = position;
        
        SpriteComponent sprite = { .sprite = '&' };
        world.sprite_components[crystal_ball_entity] = sprite;
        
        ItemComponent item = {
            .name = "Crystal Ball",
            .type = ITEM_TYPE_BUFF,
            .item_data.buff_data.mana = 26
        };
        world.item_components[crystal_ball_entity] = item;
    }
    //===================================//
    
    //==========[Shield Item]============//
    {
        unsigned int shield_entity = World_CreateEntity(&world);
        world.mask[shield_entity] = COMPONENT_POSITION | COMPONENT_SPRITE | COMPONENT_ITEM;
        
        PositionComponent position = { .x = 28, .y = 6 };
        world.position_components[shield_entity] = position;
        
        SpriteComponent sprite = { .sprite = '&' };
        world.sprite_components[shield_entity] = sprite;
        
        ItemComponent item = {
            .name = "Wooden Shield",
            .type = ITEM_TYPE_SHIELD,
            .item_data.shield_data.defense = 12
        };
        world.item_components[shield_entity] = item;
    }
    //===================================//
    
    //==========[Sword Item]=============//
    {
        unsigned int sword_entity = World_CreateEntity(&world);
        world.mask[sword_entity] = COMPONENT_POSITION | COMPONENT_SPRITE | COMPONENT_ITEM;
        
        PositionComponent position = { .x = 4, .y = 20 };
        world.position_components[sword_entity] = position;
        
        SpriteComponent sprite = { .sprite = '&' };
        world.sprite_components[sword_entity] = sprite;
        
        ItemComponent item = {
            .name = "Travellers Sword",
            .type = ITEM_TYPE_SWORD,
            .item_data.sword_data.attack = 11,
            
        };
        world.item_components[sword_entity] = item;
    }
    //===================================//
    
    //==========[Banana Item]============//
    {
        unsigned int banana_entity = World_CreateEntity(&world);
        world.mask[banana_entity] = COMPONENT_POSITION | COMPONENT_SPRITE | COMPONENT_ITEM;
        
        PositionComponent position = { .x = 5, .y = 14 };
        world.position_components[banana_entity] = position;
        
        SpriteComponent sprite = { .sprite = '$' };
        world.sprite_components[banana_entity] = sprite;
        
        ItemComponent item = {
            .name = "Banana",
            .type = ITEM_TYPE_FOOD,
            .item_data.food_data.hp_regen_amount = 7,
        };
        world.item_components[banana_entity] = item;
    }
    //===================================//
    
    //==========[Better Shield Item]=====//
    {
        unsigned int shield_entity = World_CreateEntity(&world);
        world.mask[shield_entity] = COMPONENT_POSITION | COMPONENT_SPRITE | COMPONENT_ITEM;
        
        PositionComponent position = { .x = 45, .y = 23 };
        world.position_components[shield_entity] = position;
        
        SpriteComponent sprite = { .sprite = '&' };
        world.sprite_components[shield_entity] = sprite;
        
        ItemComponent item = {
            .name = "Iron Shield",
            .type = ITEM_TYPE_SHIELD,
            .item_data.shield_data.defense = 15,
        };
        world.item_components[shield_entity] = item;
    }
    //===================================//
    
    //==========[Player Entity]==========//
    {
        unsigned int player_entity = World_CreateEntity(&world);
        world.mask[player_entity] = COMPONENT_POSITION | COMPONENT_VELOCITY | COMPONENT_SPRITE | COMPONENT_INPUT | COMPONENT_COLLIDER | COMPONENT_STATS | COMPONENT_INVENTORY | COMPONENT_PORTAL_PASS | COMPONENT_ARMOUR;
        
        PositionComponent position = { .x = 23, .y = 13 };
        world.position_components[player_entity] = position;
        
        VelocityComponent velocity = { .x = 0, .y = 0 };
        world.velocity_components[player_entity] = velocity;
        
        SpriteComponent sprite = { .sprite = '@' };
        world.sprite_components[player_entity] = sprite;
        
        StatsComponent stats = {
            .hp  = 15,
            .def = 0,
            .atk = 0,
            .man = 10
        };
        world.stats_components[player_entity] = stats;
        
        InventoryComponent inventory = {};
        world.inventory_components[player_entity] = inventory;
        
        ArmourComponent armour = {};
        world.armour_components[player_entity] = armour;
        
        InputComponent input = {
            .move_up = VK_UP,
            .move_down = VK_DOWN,
            .move_left = VK_LEFT,
            .move_right = VK_RIGHT,
            .inv_scroll_up = 0x52,   // R
            .inv_scroll_down = 0x46, // F
            .inv_use_item_key = VK_SPACE
        };
        world.input_components[player_entity] = input;
    }
    //===================================//
}

void StatePlaying_Input()
{
    System_Input(&world);
}

void StatePlaying_Update()
{
    System_Movement(&world);
    System_Physics(&world);
    System_Collision(&world, map_data);
    System_Portal(&world, map_file, map_data);
    System_ItemPickup(&world);
    System_InventoryLogic(&world);
    System_ArmourStatLogic(&world);
}

void StatePlaying_Draw()
{
    Map_Draw(map_data);
    System_Render(&world);
    
    System_DrawStats(&world);
    System_DrawInventory(&world);
}
