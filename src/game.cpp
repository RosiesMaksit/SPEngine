#include <game.hpp>
#include <scene.hpp>
#include <scenes/list.hpp>
#include <fx.hpp>

#include <render.hpp>
#include <geometry.hpp>

#include <sound.hpp>

#include <pad.hpp>

#include <parser.hpp>
#include <storage.hpp>

#include <types.hpp>

Scene* Game::currentScene;
Scene* Game::nextScene;
bool   Game::is_fx;


int Game::Initialize() {
    if (Render::Initialize()) return -1;
    if (Geometry::Initialize()) return -1;

    if (Parser::InitializeCD()) return -1;
    if (Storage::Initialize()) return -1;

    if (Sound::Initialize()) return -1;
    if (Pad::Initialize()) return -1;

    if (Fx::Initialize()) return -1;
    Fx::SetFrame(150);
    is_fx = false;

    currentScene = new LoadingScene();

    if (currentScene->Initialize()) return -1;

    Registry* registry = currentScene->GetRegistry();
    
    Render::SetRegistry(registry);
    Geometry::SetRegistry(registry);

    for(int i; i < 180; i++);

    return 0;
}

void Game::Run() {
    for (;;) {
        Pad::Update();

        currentScene->Update();
        Geometry::SetCamera(currentScene->cam_pos, currentScene->cam_rot);
        Geometry::Update();
        Render::Present();
    }
}

void Game::SetScene(Scene* newScene, uint8_t type) {
    if (is_fx) {
        delete newScene;
        return;
    }
    
    is_fx = true;
    nextScene = newScene;

    Fx::StartFX(currentScene, nextScene, type);
    is_fx = false;
}