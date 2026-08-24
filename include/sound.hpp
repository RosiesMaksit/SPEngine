#ifndef SOUND_HPP
#define SOUND_HPP

typedef struct {
    uint32_t data;
    uint16_t size;
    uint16_t rate;
    uint16_t volL;
    uint16_t volR;
    int16_t pitch;
    uint16_t adsr1;
    uint16_t adsr2;
    uint8_t priority;
} VAG;

class Sound {
    static VAG audios[256];
    static uint8_t audiox;
    static uint8_t map_priorities[24];

    public:
    static int Initialize();

    static uint8_t Load(const char* name);
    static uint8_t SetAudio(uint32_t* data);

    static void SetVolume(uint8_t id, uint16_t L, uint16_t R);
    static void SetPitch(uint8_t id, int16_t pitch);
    static void SetADSR(uint8_t id, uint8_t a, uint8_t d, uint8_t s, uint8_t r);
    static void SetPriority(uint8_t id, uint8_t priority);

    static void Play(uint8_t id);
    static void PlayEx(uint8_t id, uint8_t channel, uint16_t L, uint16_t R);
};
#endif