#include "systems.h"

#include "../game.h"
#include "../map/map_tiles.h"

#include "../util/maths/v2.h"
#include "../util/maths/math_funcs.h"
#include "../util/console/console_colours.h"
#include "../util/game/game_funcs.h"

#include "../tools/loader.h"
#include "../tools/logger.h"

#include <windows.h>

/*
  ** What was I thinking when I thought it'd be a good idea to put all the logic in the game into a huuuuuge .c file? seriously...
*/

#define PHYSICS_MASK (COMPONENT_POSITION | COMPONENT_VELOCITY)
void System_Physics(World *world)
{
    PositionComponent *p;
    VelocityComponent *v;
    
    for (unsigned int entity_id = 0; entity_id < MAX_ENTITIES; entity_id++)
    {
        if (World_HasComponent(world, entity_id, PHYSICS_MASK))
        {
            p = &world->position_components[entity_id];
            v = &world->velocity_components[entity_id];
            
            p->x += v->x;
            p->y += v->y;
        }
    }
}

#define INPUT_MASK (COMPONENT_INPUT)
void System_Input(World *world)
{
    InputComponent *i;
    
    for (unsigned int entity_id = 0; entity_id < MAX_ENTITIES; entity_id++)
    {
        if (World_HasComponent(world, entity_id, INPUT_MASK))
        {
            i = &world->input_components[entity_id];
            
            // Movement
            i->move_input_y = 0;
            i->move_input_x = 0;
            if (GetAsyncKeyState(i->move_up))    i->move_input_y--;
            if (GetAsyncKeyState(i->move_down))  i->move_input_y++;
            if (GetAsyncKeyState(i->move_left))  i->move_input_x--;
            if (GetAsyncKeyState(i->move_right)) i->move_input_x++;
            
            // Inventory
            i->inv_scroll_input = 0;
            i->inv_use_item = 0;
            if (GetAsyncKeyState(i->inv_scroll_up))    i->inv_scroll_input++;
            if (GetAsyncKeyState(i->inv_scroll_down))  i->inv_scroll_input--;
            if (GetAsyncKeyState(i->inv_use_item_key)) i->inv_use_item = 1;
            i->inv_scroll_amt += i->inv_scroll_input;
        }
    }
}

#define MOVEMENT_MASK (COMPONENT_VELOCITY | COMPONENT_INPUT)
void System_Movement(World *world)
{
    VelocityComponent *v;
    InputComponent *i;
    
    for (unsigned int entity_id = 0; entity_id < MAX_ENTITIES; entity_id++)
    {
        if (World_HasComponent(world, entity_id, MOVEMENT_MASK))
        {
            v = &world->velocity_components[entity_id];
            i = &world->input_components[entity_id];
            
            v->x = i->move_input_x;
            v->y = i->move_input_y;
        }
    }
}

#define RENDER_MASK (COMPONENT_POSITION | COMPONENT_SPRITE)
void System_Render(World *world)
{
    PositionComponent *p;
    SpriteComponent *s;
    
    for (unsigned int entity_id = 0; entity_id < MAX_ENTITIES; entity_id++)
    {
        if (World_HasComponent(world, entity_id, RENDER_MASK))
        {
            p = &world->position_components[entity_id];
            s = &world->sprite_components[entity_id];
            
            WORD col = MapTiles_GetColour(s->sprite);
            Game_PutCharCol(p->x+1, p->y+2, s->sprite, col);
        }
    }
}

#define COLLISION_MASK (COMPONENT_POSITION | COMPONENT_VELOCITY | COMPONENT_COLLIDER)
void System_Collision(World *world, MapData *map)
{
    PositionComponent *p;
    VelocityComponent *v;
    
    for (unsigned int entity_id = 0; entity_id < MAX_ENTITIES; entity_id++)
    {
        if (World_HasComponent(world, entity_id, COLLISION_MASK))
        {
            p = &world->position_components[entity_id];
            v = &world->velocity_components[entity_id];
            
            if (MapTiles_SolidsContain(Map_GetAtV2(map, V2_Make(p->x, p->y))) ||
                p->x == -1 || p->y == -1 ||
                p->x == 49 || p->y == 25) {
                p->x -= v->x;
                p->y -= v->y;
            }
        }
    }
}

#define PORTAL_MASK (COMPONENT_POSITION | COMPONENT_PORTAL_PASS)
void System_Portal(World *world, FileData *data, MapData *map)
{
    PositionComponent *p;
    
    for (unsigned int entity_id = 0; entity_id < MAX_ENTITIES; entity_id++)
    {
        if (World_HasComponent(world, entity_id, PORTAL_MASK))
        {
            p = &world->position_components[entity_id];
            char c = Map_GetAtV2(map, V2_Make(p->x, p->y));
            
            if (MapTiles_MovementsContain(c))
            {
                V2 dir = MapTiles_GetDirection(c);
                
                char file_name[32];
                sprintf(file_name, "maps/%d_%d", map->world_position.x + dir.x, map->world_position.y + dir.y);
                
                Loader_ReloadFromFile(data, file_name);
                if (!data) {
                    Logger_LogMessage("error", "Data was null when reloading file in portal system.");
                    return;
                }
                
                Map_Set(map, data);
                
                // NOTE(kmp): temporary -> this should be a value found in the actual map files (spawnpos)
                p->x = 22;
                p->y = 12;
            }
        }
    }
}

#define DRAW_STATS_MASK (COMPONENT_STATS)
void System_DrawStats(World *world)
{
    StatsComponent *s;
    V2 stats_position   = { 1,  28 };
    
    V2 health_position  = { 3,  29 };
    V2 defense_position = { 8,  29 };
    V2 attack_position  = { 14, 29 };
    V2 mana_position    = { 20, 29 };
    
    for (unsigned int entity_id = 0; entity_id < MAX_ENTITIES; entity_id++)
    {
        if (World_HasComponent(world, entity_id, DRAW_STATS_MASK))
        {
            s = &world->stats_components[entity_id];
            
            // Draw Title
            Game_DrawText(stats_position.x+9, stats_position.y+5, "STATS");
            
            // Draw Seperator
            GameFuncs_DrawSeperatorCol(stats_position.x, stats_position.y+4,
                                       '%', 24, SEPDIR_HORIZONTAL, FG_GREY);
            
            // Draw Subtitles
            Game_DrawTextCol(health_position.x,   health_position.y,    FG_LIGHT_GREEN,   "HP");
            Game_DrawTextCol(defense_position.x,  defense_position.y,   FG_LIGHT_YELLOW,  "DEF");
            Game_DrawTextCol(attack_position.x,   attack_position.y,    FG_LIGHT_RED,     "ATK");
            Game_DrawTextCol(mana_position.x,     mana_position.y,      FG_LIGHT_BLUE,    "MAN");
            
            // Draw Values
            Game_DrawTextCol(health_position.x,   health_position.y+1,  FG_DARK_GREEN,    "%d", s->hp);
            Game_DrawTextCol(defense_position.x,  defense_position.y+1, FG_DARK_YELLOW,   "%d", s->def);
            Game_DrawTextCol(attack_position.x,   attack_position.y+1,  FG_DARK_RED,      "%d", s->atk);
            Game_DrawTextCol(mana_position.x,     mana_position.y+1,    FG_DARK_BLUE,     "%d", s->man);
        }
    }
}

#define DRAW_INVENTORY_MASK (COMPONENT_INVENTORY | COMPONENT_INPUT)
void System_DrawInventory(World *world)
{
    InventoryComponent *i;
    InputComponent *in;
    
    V2 inventory_position = { 26, 28 };
    
    for (unsigned int entity_id = 0; entity_id < MAX_ENTITIES; entity_id++)
    {
        if (World_HasComponent(world, entity_id, DRAW_INVENTORY_MASK))
        {
            i =  &world->inventory_components[entity_id];
            in = &world->input_components[entity_id];
            
            // Draw Inventory UI
            Game_DrawText(inventory_position.x + 7, inventory_position.y, "INVENTORY");
            GameFuncs_DrawSeperatorCol(inventory_position.x, inventory_position.y+1, '%', 24, SEPDIR_HORIZONTAL, FG_GREY);
            Game_PutChar(inventory_position.x + 2, inventory_position.y + 3, '>');
            
            // Draw Inventory Items
            for (int ii = 0; ii < i->item_count; ii++) {
                int pos_y = inventory_position.y + 2 + in->inv_scroll_amt + ii;
                WORD draw_col = FG_WHITE;
                
                if (pos_y == inventory_position.y + 3) {
                    i->current_selected_item = ii;
                }
                
                if (pos_y == inventory_position.y + 2 ||
                    pos_y == inventory_position.y + 5) {
                    draw_col = FG_GREY;
                }
                
                ItemStack *stack = &i->items[ii];
                if (stack->item_count > 0 &&
                    pos_y >= inventory_position.y + 2 &&
                    pos_y <= inventory_position.y + 5) {
                    Game_DrawTextCol(inventory_position.x + 4, pos_y, draw_col, stack->held_item.name);
                }
            }
        }
    }
}

#define ITEM_USE_MASK (COMPONENT_INPUT | COMPONENT_INVENTORY | COMPONENT_STATS | COMPONENT_ARMOUR)
void System_InventoryLogic(World *world)
{
    InventoryComponent *inv;
    InputComponent *inp;
    StatsComponent *stat;
    ArmourComponent *arm;
    
    for (unsigned int entity_id = 0; entity_id < MAX_ENTITIES; entity_id++)
    {
        if (World_HasComponent(world, entity_id, ITEM_USE_MASK))
        {
            inv = &world->inventory_components[entity_id];
            inp = &world->input_components[entity_id];
            stat = &world->stats_components[entity_id];
            arm = &world->armour_components[entity_id];
            
            if (inp->inv_use_item)
            {
                inp->inv_use_item = 0;
                ItemStack *item_stack = &inv->items[inv->current_selected_item];
                
                if (item_stack->item_count > 0)
                {
                    switch (item_stack->held_item.type)
                    {
                        case ITEM_TYPE_FOOD: { // Food
                            stat->hp += item_stack->held_item.item_data.food_data.hp_regen_amount;
                            item_stack->item_count--;
                            break;
                        } case ITEM_TYPE_BUFF: { // Buff
                            stat->hp_boost  = item_stack->held_item.item_data.buff_data.hp;
                            stat->def_boost = item_stack->held_item.item_data.buff_data.defense;
                            stat->atk_boost = item_stack->held_item.item_data.buff_data.attack;
                            stat->man_boost = item_stack->held_item.item_data.buff_data.mana;
                            item_stack->item_count--;
                            break;
                        } case ITEM_TYPE_SWORD: { // Sword
                            if (arm->sword.type == ITEM_TYPE_NONE)
                            { 
                                arm->sword = item_stack->held_item;
                                item_stack->item_count--;
                            }
                            else
                            {
                                ItemComponent temp = arm->sword;
                                arm->sword = item_stack->held_item;
                                item_stack->held_item = temp;
                            }
                            break;
                        } case ITEM_TYPE_AMULET: { // Amulet
                            if (arm->amulet.type == ITEM_TYPE_NONE)
                            { 
                                arm->amulet = item_stack->held_item;
                                item_stack->item_count--;
                            }
                            else
                            {
                                ItemComponent temp = arm->amulet;
                                arm->amulet = item_stack->held_item;
                                item_stack->held_item = temp;
                            }
                            break;
                        } case ITEM_TYPE_ARMOUR: { // Armour
                            if (arm->armour.type == ITEM_TYPE_NONE)
                            { 
                                arm->armour = item_stack->held_item;
                                item_stack->item_count--;
                            }
                            else
                            {
                                ItemComponent temp = arm->armour;
                                arm->armour = item_stack->held_item;
                                item_stack->held_item = temp;
                            }
                            break;
                        } case ITEM_TYPE_SHIELD: { // Shield
                            if (arm->shield.type = ITEM_TYPE_NONE)
                            { 
                                arm->shield = item_stack->held_item;
                                item_stack->item_count--;
                            }
                            else
                            {
                                ItemComponent temp = arm->shield;
                                arm->shield = item_stack->held_item;
                                item_stack->held_item = temp;
                            }
                            break;
                        } case ITEM_TYPE_CLOAK: { // Cloak
                            if (arm->cloak.type == ITEM_TYPE_NONE)
                            { 
                                arm->cloak = item_stack->held_item;
                                item_stack->item_count--;
                            }
                            else
                            {
                                ItemComponent temp = arm->cloak;
                                arm->cloak = item_stack->held_item;
                                item_stack->held_item = temp;
                            }
                            break;
                        } case ITEM_TYPE_WAND: { // Wand (acts as the sword but reduces total mana in exchange for more dmg)
                            if (arm->sword.type == ITEM_TYPE_NONE)
                            { 
                                arm->cloak = item_stack->held_item;
                                item_stack->item_count--;
                            }
                            else
                            {
                                ItemComponent temp = arm->cloak;
                                arm->cloak = item_stack->held_item;
                                item_stack->held_item = temp;
                            }
                            break;
                        }
                    }
                    
                    if (item_stack->item_count <= 0)
                    {
                        inv->item_count--;
                        if (inv->item_count > 0) {
                            for (int ii = 0; ii < inv->item_count; ii++) {
                                inv->items[inv->current_selected_item+ii] = inv->items[inv->current_selected_item+ii+1]; // Push all items back to its position
                            }
                        } else {
                            inv->items[inv->current_selected_item] = (const ItemStack) {}; // Delete the item
                        }
                    }
                }
            }
        }
    }
}

#define ITEM_MASK (COMPONENT_ITEM | COMPONENT_POSITION)
#define ITEM_PICKUP_MASK (COMPONENT_INVENTORY | COMPONENT_POSITION)
void System_ItemPickup(World *world)
{
    InventoryComponent *i; // Entity Inventory
    PositionComponent *p; // Entity Position
    PositionComponent *ip; // Item Position
    
    for (unsigned int entity_id = 0; entity_id < MAX_ENTITIES; entity_id++)
    {
        if (World_HasComponent(world, entity_id, ITEM_PICKUP_MASK))
        {
            i = &world->inventory_components[entity_id];
            p = &world->position_components[entity_id];
            
            for (unsigned int item_entity_id = 0; item_entity_id < MAX_ENTITIES; item_entity_id++)
            {
                if (item_entity_id == entity_id)
                    continue;
                
                if (World_HasComponent(world, item_entity_id, ITEM_MASK))
                {
                    ip = &world->position_components[item_entity_id];
                    
                    if (p->x == ip->x &&
                        p->y == ip->y)
                    {
                        i->items[i->item_count].held_item = world->item_components[item_entity_id];
                        i->items[i->item_count].item_count++;
                        i->item_count++;
                        World_DestroyEntity(world, item_entity_id);
                    }
                }
            }
        }
    }
}

#define ARMOUR_STAT_LOGIC_MASK (COMPONENT_ARMOUR | COMPONENT_STATS)
void System_ArmourStatLogic(World *world)
{
    ArmourComponent *a;
    StatsComponent *s;
    
    for (unsigned int entity_id = 0; entity_id < MAX_ENTITIES; entity_id++)
    {
        if (World_HasComponent(world, entity_id, ARMOUR_STAT_LOGIC_MASK))
        {
            a = &world->armour_components[entity_id];
            s = &world->stats_components[entity_id];
            
            s->def = a->armour.item_data.armour_data.defense + a->shield.item_data.shield_data.defense + s->def_boost;
            s->man = a->cloak.item_data.cloak_data.mana + s->man_boost;
            
            if (a->sword.type == ITEM_TYPE_SWORD) {
                s->atk = a->sword.item_data.sword_data.attack + a->amulet.item_data.amulet_data.attack + s->atk_boost;
            } else if (a->sword.type == ITEM_TYPE_WAND) {
                s->atk = a->sword.item_data.wand_data.attack + a->amulet.item_data.amulet_data.attack + s->atk_boost;
                s->man -= a->sword.item_data.wand_data.mana_red;
            }
        }
    }
}
