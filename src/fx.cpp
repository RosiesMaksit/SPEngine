#include <game.hpp>
#include <render.hpp>
#include <geometry.hpp>
#include <pad.hpp>
#include <registry/registry.hpp>
#include <scene.hpp>
#include <fx.hpp>


Scene*    Fx::prevScene;
Scene*    Fx::nextScene;
Registry* Fx::registry;

int       Fx::countFrame;
int       Fx::maxFrame;
uint8_t   Fx::typeFx;
bool      Fx::isActive;

uint16_t  Fx::elid[512];
uint16_t  Fx::countElements;


int Fx::Initialize() {
    prevScene = nullptr;
    nextScene = nullptr;

    countFrame = 0;
    maxFrame = 0;
    typeFx = 255;
    isActive = 0;

    countElements = 0;

    return 0;
}

void Fx::SetFrame(uint16_t set_frame) {
    maxFrame = set_frame;
}

void Fx::StartFX(Scene* ps, Scene* ns, uint8_t type) {
    prevScene = ps;
    nextScene = ns;
    typeFx = type;
    countFrame = 0;
    countElements = 0;

    registry = prevScene->GetRegistry();

    Update();

    for (uint16_t i = 0; i < countElements; i++) {
        registry->DestroyEntity(elid[i]);
    }

}

void Fx::Update() {
    for (;;) {
        if (countFrame == maxFrame / 2 + 1) {
            nextScene->Initialize();
            Game::currentScene = nextScene;
            registry = nextScene->GetRegistry();
            Render::SetRegistry(registry);
            Geometry::SetRegistry(registry);
            countElements = 0;
        }

        if (countFrame >= maxFrame) {
            if (prevScene) {
                delete prevScene;
                prevScene = nullptr;
            }

            return;
        }

        Game::currentScene->Update();
        Run[typeFx]();
        Geometry::Update();
        Render::Present();

        countFrame++;
    }

}








void Fx::Fade() {
    if (!countElements) {
        elid[countElements] = registry->CreateEntity();
        registry->positions[elid[countElements]] = {0, 0, 1};
        registry->sizes[elid[countElements]] = {WIDTH, HEIGHT};
        registry->colors[elid[countElements]] = {0, 0, 0};
        registry->flags[elid[countElements]] |= (VISUAL | PRIMITIVE | COLOR | TRANS | ABR_2);
        countElements++;
    }
    Color* c = &registry->colors[elid[countElements-1]];
    uint16_t phase = maxFrame / 3;

    if (countFrame < phase) {
        uint8_t val = (255 * countFrame) / phase;
        *c = {val, val, val};

    } else if (countFrame > phase * 2) {
        uint16_t current_fade_frame = maxFrame - countFrame;
        uint8_t val = (255 * current_fade_frame) / phase;
        *c = {val, val, val};

    } else {
        *c = {255, 255, 255};
    }
}







void Fx::Swipe() {

}







void Fx::Chessboard() {

    static uint8_t countX = 8;
    static uint8_t countY = 6;

    static uint16_t maxW = WIDTH / countX;
    static uint16_t maxH = HEIGHT / countY;

    if (!countElements) {
        for (uint16_t y = 0; y < countY; y++) {
            for (uint16_t x = 0; x < countX; x++) {
                elid[countElements] = registry->CreateEntity();
                registry->positions[elid[countElements]] = {x * maxW, y * maxH, 1};
                registry->sizes[elid[countElements]] = {0, 0};
                registry->colors[elid[countElements]] = (((x + y) & 1)) ?  Color{255, 222, 77} : Color{76, 205, 153};
                registry->flags[elid[countElements]] |= (VISUAL | PRIMITIVE | COLOR);

                countElements++;
            }
        }

    }

    static uint16_t curW;
    static uint16_t curH;

    if (countFrame < maxFrame / 3) {
        curW = maxW * countFrame / (maxFrame / 3);
        curH = maxH * countFrame / (maxFrame / 3);


    } else if (countFrame < maxFrame / 3 * 2) {
        curW = maxW;
        curH = maxH;


    } else if (countFrame < maxFrame) {
        curW = maxW * (maxFrame - countFrame) / (maxFrame / 3);
        curH = maxH * (maxFrame  - countFrame) / (maxFrame / 3);
    }

    for (uint16_t i = 0; i < countElements; i++) {
        uint16_t id = elid[i];

        Position* pos = &registry->positions[id];
        Size* size = &registry->sizes[id];

        uint8_t gx = i % countX;
        uint8_t gy = i / countX;

        pos->vx = (gx * maxW) + ((maxW - curW) / 2);
        pos->vy = (gy * maxH) + ((maxH - curH) / 2);

        size->w = curW;
        size->h = curH;
    }
}






void Fx::ChessSwipe() {

    static uint8_t countX = 8;
    static uint8_t countY = 6;

    static uint16_t maxW = WIDTH / countX;
    static uint16_t maxH = HEIGHT / countY;

    if (!countElements) {
        for (uint16_t y = 0; y < countY; y++) {
            for (uint16_t x = 0; x < countX; x++) {
                elid[countElements] = registry->CreateEntity();
                registry->positions[elid[countElements]] = {x * maxW, y * maxH, 1};
                registry->sizes[elid[countElements]] = {0, 0};
                registry->colors[elid[countElements]] = (((x + y) & 1)) ?  Color{255, 222, 77} : Color{76, 205, 153};
                registry->flags[elid[countElements]] |= (VISUAL | PRIMITIVE | COLOR);

                countElements++;
            }
        }

    }

    static uint16_t curW;
    static uint16_t curH;

    for (uint16_t i = 0; i < countElements; i++) {
        uint16_t id = elid[i];

        Position* pos = &registry->positions[id];
        Size* size = &registry->sizes[id];

        uint8_t gx = i % countX;
        uint8_t gy = i / countX;

        uint16_t dist = (((countElements - 1) % countX) + ((countElements - 1) / countX));

        uint16_t delay = (gx + gy) * ((maxFrame / 3 * 179) >> 8) / (dist == 0 ? 1 : dist);

        if (countFrame < maxFrame / 3) {

            if (countFrame <= delay) {
                curW = 0;
                curH = 0;
            } else {
                uint16_t frame = countFrame - delay;
                curW = maxW * frame / ((maxFrame / 3 * 77) >> 8);
                curH = maxH * frame / ((maxFrame / 3 * 77) >> 8);

                if (curW > maxW) curW = maxW;
                if (curH > maxH) curH = maxH;
            }

        } else if (countFrame < maxFrame) {

            if (countFrame <= ((maxFrame / 3 * 2) + delay)) {
                curW = maxW;
                curH = maxH;
            } else {
                uint16_t frame = countFrame - delay - (maxFrame / 3 * 2);

                uint16_t lostW = maxW * frame / ((maxFrame / 3 * 77) >> 8);
                uint16_t lostH = maxH * frame / ((maxFrame / 3 * 77) >> 8);

                curW = (maxW > lostW) ? (maxW - lostW) : 0; //curW = maxW * (maxFrame / 3 - delay - frame) / (maxFrame / 3 - delay);
                curH = (maxH > lostH) ? (maxH - lostH) : 0;//curH = maxH * (maxFrame / 3 - delay - frame) / (maxFrame / 3 - delay);
            }

        }

        pos->vx = (gx * maxW) + ((maxW - curW) / 2);
        pos->vy = (gy * maxH) + ((maxH - curH) / 2);

        size->w = curW;
        size->h = curH;
    }
}





void (*Fx::Run[])() = {
    &Fx::Fade,
    &Fx::Swipe,
    &Fx::Chessboard,
    &Fx::ChessSwipe
};