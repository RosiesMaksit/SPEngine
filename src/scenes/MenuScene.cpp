#include <scenes/list.hpp>

#include <pad.hpp>
#include <game.hpp>
#include <scene.hpp>
#include <registry/registry.hpp>
#include <parser.hpp>
#include <storage.hpp>

MenuScene::MenuScene() : Scene() {
    cam_pos.vx = 0;
    cam_pos.vy = 0;
    cam_pos.vz = 0;
    selected_item = 0;
    can_navigate = 1;
}

MenuScene::~MenuScene() {
    registry.DestroyEntity(ids[0]);
    registry.DestroyEntity(ids[1]);
}


int MenuScene::Initialize() {
    if (Scene::Initialize()) return -1;

    ids[0] = registry.CreateEntity();
    ids[1] = registry.CreateEntity();

    // Первый пункт меню (Старт)
    registry.positions[ids[0]] = {100, 80, 155};
    registry.sizes[ids[0]]     = {120, 20}; // Нормальный размер для кнопки
    // Обязательно взводим PRIMITIVE и COLOR, чтобы рендер пустил по нужной ветке и применил цвет
    registry.flags[ids[0]]    |= (VISUAL | PRIMITIVE | COLOR); 

    // Второй пункт меню (Выход)
    registry.positions[ids[1]] = {100, 110, 155};
    registry.sizes[ids[1]]     = {120, 20};
    registry.flags[ids[1]]    |= (VISUAL | PRIMITIVE | COLOR);

    // Стартовая инициализация цветов (первый активен, второй погашен)
    registry.colors[ids[0]] = {200, 200, 0};  // Желтый
    registry.colors[ids[1]] = {64, 64, 64};   // Серый

    return 0;
}

void MenuScene::Update() {
    // Навигация
    if (Pad::states & UP) {
        if (can_navigate && selected_item > 0) {
            selected_item--;
            can_navigate = 0;
        }
    } else if (Pad::states & DOWN) {
        if (can_navigate && selected_item < 1) {
            selected_item++;
            can_navigate = 0;
        }
    } else if (!(Pad::states & (UP | DOWN))) {
        can_navigate = 1; // Сброс блокировки, когда отпустили кнопки
    }

    // Динамическая смена цветов кнопок
    if (selected_item == 0) {
        registry.colors[ids[0]] = {155, 109, 128};  // Выбран (Желтый)
        registry.colors[ids[1]] = {64, 64, 64};   // Не выбран (Серый)
    } else {
        registry.colors[ids[0]] = {64, 64, 64};   // Не выбран (Серый)
        registry.colors[ids[1]] = {200, 200, 0};  // Выбран (Желтый)
    }

    // Реакция на нажатие (вход в игру или переключение)
    if (Pad::states & START) {
        if (selected_item == 0) {
            Game::SetScene(new TestScene(), 0);
        } else if (selected_item == 1) {
            // Сюда можно повесить, например, возврат на какую-то другую сцену
        }
    }
}
