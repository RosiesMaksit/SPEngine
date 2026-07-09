#ifndef REGISTRY_HPP
#define REGISTRY_HPP

#include "components.hpp"

#define MAX_OBJECTS 512

#define COMPONENT_LIST \
    X(Position, positions) \
    X(Size, sizes) \
    X(Rotation, rotations) \
    X(Color, colors) \
    X(Mesh, meshes) \
    X(Texture, textures)

class Registry {
    public:

    #define X(type, list) type list[MAX_OBJECTS];
    COMPONENT_LIST
    #undef X
    uint8_t flags[MAX_OBJECTS] = {0};

    int CreateEntity();

    void DestroyEntity(int id);
};

#endif