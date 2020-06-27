#ifndef INVENTORY_COMPONENTS_H
#define INVENTORY_COMPONENTS_H

#define MAX_ITEMS 32

typedef struct StatsComponent {
    // Health
    int hp;
    int hp_boost;
    
    // Defense
    int def;
    int def_boost;
    
    // Attack
    int atk;
    int atk_boost;
    
    // Mana
    int man;
    int man_boost;
} StatsComponent;

typedef enum ItemType {
    ITEM_TYPE_NONE       = 0,
    ITEM_TYPE_FOOD       = 1 << 0,
    ITEM_TYPE_SWORD      = 1 << 1,
    ITEM_TYPE_AMULET     = 1 << 2,
    ITEM_TYPE_ARMOUR     = 1 << 3,
    ITEM_TYPE_SHIELD     = 1 << 4,
    ITEM_TYPE_CLOAK      = 1 << 5,
    ITEM_TYPE_WAND       = 1 << 6,
    ITEM_TYPE_BUFF       = 1 << 7
} ItemType;

typedef struct ItemComponent {
    char name[20];
    int type;
    union { // NOTE(kmp): unions are awesome!
        /* BUFF ITEM DATA */
        struct {
            int hp;
            int attack;
            int defense;
            int mana;
        } buff_data;
        
        /* FOOD ITEM DATA */
        struct {
            int hp_regen_amount;
        } food_data;
        
        /* SWORD ITEM DATA */
        struct {
            int attack;
        } sword_data;
        
        /* AMULET ITEM DATA */
        struct {
            int attack;
        } amulet_data;
        
        /* ARMOUR ITEM DATA */
        struct {
            int defense;
        } armour_data;
        
        /* SHIELD ITEM DATA */
        struct {
            int defense;
        } shield_data;
        
        /* CLOAK ITEM DATA */
        struct {
            int mana;
        } cloak_data;
        
        /* WAND ITEM DATA */
        struct {
            int attack;
            int mana_red;
        } wand_data;
    } item_data;
} ItemComponent;

typedef struct ItemStack {
    ItemComponent held_item;
    unsigned int item_count;
} ItemStack;

typedef struct InventoryComponent {
    int item_count;
    int current_selected_item;
    ItemStack items[MAX_ITEMS];
} InventoryComponent;

typedef struct ArmourComponent {
    ItemComponent armour;
    ItemComponent cloak;
    ///////////////////////
    ItemComponent amulet;
    ///////////////////////
    ItemComponent sword;
    ItemComponent shield;
} ArmourComponent;

#endif //INVENTORY_COMPONENTS_H
