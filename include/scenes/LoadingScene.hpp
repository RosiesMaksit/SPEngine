#ifndef LOADSCENE_HPP
#define LOADSCENE_HPP

#include <scene.hpp>

class LoadingScene : public Scene {
    uint16_t fps;
    
    public:
    LoadingScene();

    int  Initialize() override;
    void Update()     override;
};

#endif
