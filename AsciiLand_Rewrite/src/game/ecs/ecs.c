#include "ecs.h"

unsigned int World_CreateEntity(World *world)
{
    for (unsigned int id = 0; id < MAX_ENTITIES; id++) {
        if (world->mask[id] == COMPONENT_NONE) {
            return id;
        }
    }
    
    // TODO(kmp): Throw Error (no more space for new entity)
    return MAX_ENTITIES;
}

void World_DestroyEntity(World *world, unsigned int id)
{
    world->mask[id] = COMPONENT_NONE;
}

int World_HasComponent(World *world, unsigned int id, ComponentFlag flag)
{
    return ((world->mask[id] & flag) == flag);
}
