#include <game.hpp>
#include <scene.hpp>
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
__attribute__((section(".overlay_space"))) uint8_t scene_memory_shield[64 * 1024]; // хз это надо или оно и так работает

int Game::Initialize() {
    if (Render::Initialize()) return -1;
    if (Geometry::Initialize()) return -1;
    if (Sound::Initialize()) return -1;
    if (Parser::InitializeCD()) return -1;
    if (Storage::Initialize()) return -1;
    if (Pad::Initialize()) return -1;
    if (Fx::Initialize()) return -1;

    Fx::SetFrame(150);
    is_fx = false;

    Parser::ReadFile(STARTSCENE);
    Parser::CheckRead(0);
    uint32_t* buffer_scn = Parser::LoadFile();
    uint16_t size_scn = Parser::GetSizeFile();

    uint32_t* target_ram_address = (uint32_t*)0x800C0000;
    memcpy(target_ram_address, buffer_scn, size_scn);
    while (CdReadSync(1, nullptr) > 0) { 

    }
    FlushCache();

    typedef Scene* (*StartOverlayFunc)(void*, uint32_t);
    StartOverlayFunc startScene = (StartOverlayFunc)0x800C0000;

    uint32_t current_engine_gp;
    __asm__ __volatile__("move %0, $gp" : "=r"(current_engine_gp));

    uint32_t code_size_aligned = (size_scn + 3) & ~3;
    void* object_ram_alloc = (void*)(0x800C0000 + code_size_aligned);

    // Прыгаем в оверлей
    currentScene = startScene(object_ram_alloc, current_engine_gp);

    if (currentScene->Initialize()) {
        return -1;
    }

    Registry* registry = currentScene->GetRegistry();
    
    Render::SetRegistry(registry);
    Geometry::SetRegistry(registry);
    Geometry::SetCamera(&currentScene->cam_pos, &currentScene->cam_rot);

    return 0;
}

void Game::Run() {
    for (;;) {
        Pad::Update();

        currentScene->Update();
        Geometry::Update();
        Render::Present();
    }
}

void Game::SetScene(const char* scene, uint8_t intype, uint8_t outtype) {
    if (is_fx) return;
    
    is_fx = true;
    Fx::StartFX(currentScene, scene, intype, outtype);
    is_fx = false;
    
    typedef void (*RunFunc)();
    RunFunc run_ptr = &Game::Run;

    __asm__ __volatile__(
        "li $sp, 0x801FFFF0\n\t"
        "jr %0\n\t"
        "nop"
        :
        : "r"(run_ptr)
    );
}