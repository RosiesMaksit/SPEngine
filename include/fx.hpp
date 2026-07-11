#ifndef FX_HPP
#define FX_HPP

#include <types.hpp>

class Scene;
class Registry;

class Fx {
    static Scene* prevScene;
    static Scene* nextScene;
    static Registry* registry;

    static int countFrame;
    static int maxFrame;
    static uint8_t typeFx;
    static uint8_t typeFxIn;
    static uint8_t typeFxOut;
    static bool isActive;

    // id элементов
    static uint16_t elid[512];
    static uint16_t countElements;

    // Анимации переходы
    static void Fade();
    static void Swipe();
    static void Chessboard();
    static void ChessSwipe();
    static void Test();

    public:
    // старт анимации
    static void StartFX(Scene* ps, Scene* ns, uint8_t in, uint8_t out);
    static void SetFrame(uint16_t set_frame);

    static int Initialize();
    static void Update();

    static void (*Run[])();
};

#endif