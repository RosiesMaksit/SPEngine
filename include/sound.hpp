#ifndef SOUND_HPP
#define SOUND_HPP

typedef struct {
    uint32_t data;
    uint16_t size;
    uint16_t rate;
} VAG;

class Sound {
    static VAG audios[512];
    static uint16_t audiox;

    public:
    static int Initialize();

    // в отличии от реестра отсюда нельзя удалить музыку после загрузки
    static int SetAudio(uint32_t* data);

    static void Play(uint16_t id, uint8_t channel);
};
#endif