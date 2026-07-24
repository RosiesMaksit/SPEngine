#include <scenes/list.hpp>

#include <pad.hpp>
#include <game.hpp>
#include <scene.hpp>
#include <registry/registry.hpp>
#include <parser.hpp>
#include <storage.hpp>


TestScene::TestScene() : Scene() {
    cam_pos.vx = 0;
    cam_pos.vy = 0;
    cam_pos.vz = 0;
}

int TestScene::Initialize() {
    if (Scene::Initialize()) return -1;

    // 1. CD-ROM начинает читать в фоне
    Parser::ReadFile("BALL.TIM");

    // 2. Инициализируем спрайт мяча
    objId = registry.CreateEntity();
    registry.positions[objId] = {100, 100, 2};
    registry.sizes[objId] = {32, 32};
    registry.colors[objId] = {204, 204, 0};
    registry.flags[objId] |= (VISUAL | TRANS);

    // 3. Создаем сущность куба
    testId = registry.CreateEntity();

    // Объявляем геометрию куба прямо в секции данных. 
    // Процессор НЕ тратит время на создание этого массива в рантайме.
    static const Polygon CUBE_DATA[12] = {
        // Передняя грань (Красный)
        {{ {-50, -50,  50}, { 50, -50,  50}, { 50,  50,  50} }},
        {{ {-50, -50,  50}, { 50,  50,  50}, {-50,  50,  50} }},
        // Задняя грань (Зеленый)
        {{ { 50, -50, -50}, {-50, -50, -50}, {-50,  50, -50} }},
        {{ { 50, -50, -50}, {-50,  50, -50}, { 50,  50, -50} }},
        // Верхняя грань (Синий)
        {{ {-50,  50,  50}, { 50,  50,  50}, { 50,  50, -50} }},
        {{ {-50,  50,  50}, { 50,  50, -50}, {-50,  50, -50} }},
        // Нижняя грань (Желтый)
        {{ {-50, -50, -50}, { 50, -50, -50}, { 50, -50,  50} }},
        {{ {-50, -50, -50}, { 50, -50,  50}, {-50, -50,  50} }},
        // Правая грань (Фиолетовый)
        {{ { 50, -50,  50}, { 50, -50, -50}, { 50,  50, -50} }},
        {{ { 50, -50,  50}, { 50,  50, -50}, { 50,  50,  50} }},
        // Левая грань (Циан)
        {{ {-50, -50, -50}, {-50, -50,  50}, {-50,  50,  50} }},
        {{ {-50, -50, -50}, {-50,  50,  50}, {-50,  50, -50} }}
    };

    Mesh mesh;
    mesh.count_triangles = 12;
    
    // НАПРЯМУЮ: просто отдаем указатель на статический массив.
    // Процессор не копирует ни одного байта геометрии!
    mesh.triangles = (Polygon*)CUBE_DATA; 

    registry.meshes[testId] = mesh;
    registry.positions[testId] = {0, 0, 7500}; 
    registry.colors[testId] = {200, 200, 0};
    registry.rotations[testId] = {0, 0, 0};
    registry.flags[testId] |= (VISUAL | MESH);

    // =========================================================================
    // ВСЁ ГОТОВО. ЖДЕМ CD-ROM.
    // =========================================================================
    Parser::CheckRead(0); 

    uint32_t* ball_data = Parser::LoadFile();
    TIM_IMAGE tim;
    GetTimInfo(ball_data, &tim);

    LoadImage(tim.prect, tim.paddr);
    DrawSync(0);

    if (tim.mode & 0x8) LoadImage(tim.crect, tim.caddr);

    registry.textures[objId] = {*tim.prect, *tim.crect, 0, 0, 1, {16, 16}, (uint8_t)tim.mode};

    return 0;
}


void TestScene::Update() {

    Rotation* rot = &registry.rotations[testId];

    rot->vx += 16;
    rot->vy += 16;
    rot->vz += 16;

    Position* pos = &registry.positions[objId];

    if (Pad::states & START) {

    if (Pad::states & UP)    velocity.vy -= 16;
    if (Pad::states & DOWN)  velocity.vy += 16;
    if (Pad::states & LEFT)  velocity.vx -= 16;
    if (Pad::states & RIGHT) velocity.vx += 16;
    } else {
    if (Pad::states & UP)    cam_pos.vy -= 16;
    if (Pad::states & DOWN)  cam_pos.vy += 16;
    if (Pad::states & LEFT)  cam_pos.vx -= 16;
    if (Pad::states & RIGHT) cam_pos.vx += 16;
    }
    if (Pad::states & USE) Game::SetScene(new TestScene(), 2, 3);

    velocity.vx = SIGN(velocity.vx) > 0 ? MIN(velocity.vx, 32) : MAX(velocity.vx, -32);
    velocity.vy = SIGN(velocity.vy) > 0 ? MIN(velocity.vy, 32) : MAX(velocity.vy, -32);

    pos->vx += velocity.vx ? (velocity.vx >> 4) + SIGN(velocity.vx) : 0;
    pos->vy += velocity.vy ? (velocity.vy >> 4) + SIGN(velocity.vy) : 0;

    velocity.vx = (ABS(velocity.vx) <= 8) ? 0 : velocity.vx - 4 * SIGN(velocity.vx);
    velocity.vy = (ABS(velocity.vy) <= 8) ? 0 : velocity.vy - 4 * SIGN(velocity.vy);
}