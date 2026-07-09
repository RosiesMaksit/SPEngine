#ifndef MENUSCENE_HPP
#define MENUSCENE_HPP

#include <scene.hpp>

class MenuScene : public Scene {
    uint16_t ids[2];          // ID сущностей для пунктов меню
    uint8_t selected_item;    // 0 - первый пункт, 1 - второй пункт
    uint8_t can_navigate;     // Флаг защиты от дребезга кнопок (чтобы меню не летало)

    public:
    MenuScene();
    ~MenuScene();             // Деструктор обязателен, чтобы занулить сущности при смене сцены

    int  Initialize() override;
    void Update()     override;
};

#endif
