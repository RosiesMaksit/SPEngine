#include <scene.hpp>
#include <types.hpp>

int Scene::Initialize() {
    cam_pos = {0, 0, 0};
    cam_rot = {0, 0, 0};

    return 0;
}

Registry* Scene::GetRegistry() { return &registry; }

// void* Scene::alloc(uint16_t size); эту задачу должен делать Storage

void Scene::Update() {
    
}

void Scene::Shutdown() {
}
