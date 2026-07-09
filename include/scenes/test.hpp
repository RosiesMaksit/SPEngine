#ifndef TESTSCENE
#define TESTSCENE


class Scene;

class TestScene : public Scene {
    Position velocity;
    int objId;
    int testId;

    public:
    TestScene();

    int  Initialize() override;
    void Update()     override;
};

#endif