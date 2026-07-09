#ifndef COMPONENTS_HPP
#define COMPONENTS_HPP

#include <types.hpp>

typedef struct {
    SVECTOR points[3]; 
} Polygon;

using Position = VECTOR;

typedef struct {
    uint16_t w;
    uint16_t h;
} Size;

using Rotation = SVECTOR;

using Color = CVECTOR;


typedef struct {
    Polygon* triangles;
    uint16_t count_triangles;
} Mesh;

typedef struct {
    RECT prect;
    RECT crect;

    uint16_t u_offset;
    uint16_t v_offset;

    uint8_t tpage_mode;
    uint8_t tim_mode;

} Texture;

#endif