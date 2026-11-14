#include <registry/registry.hpp>

int Registry::CreateEntity () {
    for (int i = 0; i < MAX_OBJECTS; i++) {
        if (flags[i] & ACTIVE) continue;
        flags[i] = ACTIVE;
        return i;
    }
    return -1;
}

void Registry::DestroyEntity(int id) {
    flags[id] = 0;
}