#ifndef SCENE_HPP
#define SCENE_HPP

#include <types.hpp>
#include <registry/registry.hpp>

inline void* operator new(decltype(sizeof(0)), void* ptr) noexcept { return ptr; }

class Scene {
    protected:
    Registry registry;

    public:
    VECTOR cam_pos;
    SVECTOR cam_rot;

    virtual int Initialize();
    virtual void Update();
    virtual void Shutdown();
    
    Registry* GetRegistry();

};

#endif