#ifndef SCENE_HPP
#define SCENE_HPP

#include <types.hpp>
#include <registry/registry.hpp> // а без никак?

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