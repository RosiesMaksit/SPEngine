#ifndef RENDER_HPP
#define RENDER_HPP

#include <types.hpp>

class Registry;

class Render {
    friend class Geometry;
    static DB db[2];
    static uint8_t dbx;
    static uint16_t tilex;
    static uint8_t tpagex;
    static uint16_t timx;

    static Registry* registry;

    static void flip();

    public:
    
    static int Initialize();

    static void SetRegistry(Registry* reg);
    
    static void Present();
};

#endif