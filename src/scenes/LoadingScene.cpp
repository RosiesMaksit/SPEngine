#include <scenes/list.hpp>

#include <pad.hpp>
#include <game.hpp>
#include <registry/registry.hpp>
#include <parser.hpp>
#include <storage.hpp>


LoadingScene::LoadingScene() : Scene() {}


int LoadingScene::Initialize() {
    if (Scene::Initialize()) return -1;

    Parser::ReadFile("LOADING");
    fps = 0;

    registry.CreateEntity();
    registry.CreateEntity();

    registry.positions[0] = {96, 106, 4};
    registry.sizes[0] = {128, 28};
    registry.flags[0] |= (VISUAL);

    registry.positions[1] = {0, 0, 5};
    registry.sizes[1] = {WIDTH, HEIGHT};
    registry.colors[1] = {0, 0, 0};
    registry.flags[1] |= (VISUAL | PRIMITIVE | COLOR);


    Parser::CheckRead(0); 

    uint32_t* load = Parser::LoadFile();
    TIM_IMAGE tim;
    GetTimInfo(load, &tim);

    LoadImage(tim.prect, tim.paddr);
    DrawSync(0);

    if (tim.mode & 0x8) LoadImage(tim.crect, tim.caddr);

    registry.textures[0] = {*tim.prect, *tim.crect, 0, 0, 1, {128, 28} (uint8_t)tim.mode};

    return 0;
}

void LoadingScene::Update() {
    fps++;

    if (fps > 300) {
        Game::SetScene(new MenuScene(), 0, 0);
    }
    
}
