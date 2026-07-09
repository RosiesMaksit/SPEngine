#ifndef GAME_HPP
#define GAME_HPP

#include <types.hpp>

class Scene;

class Game {
    friend class Fx;
    static Scene* currentScene;
    static Scene* nextScene;

    static bool   is_fx;

    public:

    static int    Initialize();

    static void   Run();

    static void   SetScene(Scene* newScene, uint8_t type);
};

#endif