#ifndef TESTSCENE
#define TESTSCENE

#include <scene.hpp>

class TestScene : public Scene {
    Position velocity;
    int objId;
    int testId;

    uint8_t onClick;

    public:
    TestScene();

    int  Initialize() override;
    void Update()     override;
};

#endif