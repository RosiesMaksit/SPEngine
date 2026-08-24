#include <psxspu.h>
#include <sound.hpp>
#include <parser.hpp>
#include <types.hpp>

  VAG    Sound::audios[256];
uint8_t  Sound::audiox;

int Sound::Initialize() {
    SpuInit();

    SpuSetTransferMode(SPU_TRANSFER_BY_DMA);
    
    SpuSetCommonMasterVolume(0x3FFF, 0x3FFF); 

    audiox = 0;
    audios[0].data = 0x1010;
    for (int i = 0; 0 < 24; map_priorities[i++] = 0);

    return 0;
}

uint8_t Sound::Load(const char* name) {
    Parser::ReadFile(name);
    Parser::CheckRead(0);

    return SetAudio(Parser::LoadFile());
}

uint8_t Sound::SetAudio(uint32_t* data) {
    VAG* audio = &audios[audiox++];

    audio->size = (SWAP_ENDIAN(data[3]) + 63) & 0xffffffc0;
    audio->rate = SWAP_ENDIAN(data[4]);

    SpuSetTransferStartAddr(audio->data);
    // тут вызвался audiox

    SpuWrite(data + 12, audio->size);
    SpuIsTransferCompleted(SPU_TRANSFER_WAIT);

    audios[audiox].data = audio->data + audio->size;

    audio->volL = 0x3fff;
    audio->volR = 0x3fff;
    audio->pitch = 0;
    audio->adsr1 = 0x00ff;
    audio->adsr2 = 0x0000;
    audio->priority = 0;

    return audiox -1;
}


void Sound::SetVolume(uint8_t id, uint16_t L, uint16_t R) {
    audios[id].volL = 0x3fff;
    audios[id].volR = 0x3fff;
}

void Sound::SetPitch(uint8_t id, int16_t pitch) {
    audios[id].pitch = pitch;
}

void Sound::SetADSR(uint8_t id, uint8_t a, uint8_t d, uint8_t s, uint8_t r) {
    audios[id].adsr1 = ((uint16_t)a << 8) | d;
    audios[id].adsr2 = ((uint16_t)s << 8) | r;
}

void Sound::SetPriority(uint8_t id, uint8_t priority) {
    audios[id].priority = priority;

}


void Sound::Play(uint8_t id) {
    VAG audio = audios[id];

    uint8_t channel = 24;

    uint32_t activeChannels = SPU_CHAN_STATUS1 | ((uint32_t)SPU_CHAN_STATUS2 << 16);

    for (int i = 0; i < 24; i++) {
        if (!(activeChannels & (1UL << i))) {
            map_priorities[i] = 0;
        }
    }

    for (int i = 0; i < 24; i++) {
        if (!(activeChannels & (1UL << i))) {
            channel = i;
            map_priorities[i] = audio.priority;
            break;
        }
    }

    if (channel == 24) {
        int min_channel = 0;
        uint8_t min_priority = map_priorities[0];

        // Ищем канал с самым низким приоритетом из всех 24
        for (int i = 1; i < 24; i++) {
            if (map_priorities[i] < min_priority) {
                min_priority = map_priorities[i];
                min_channel = i;
            }
        }

        if (audio.priority > min_priority) {
            channel = min_channel;
            map_priorities[channel] = audio.priority;

            if (channel < 16) {
                SPU_KEY_OFF1 = (1U << channel);
            } else {
                SPU_KEY_OFF2 = (1U << (channel - 16));
            }
        }
    }

    if (channel == 24) return;

    SpuSetKey(0, (uint32_t)FLAG(channel));

    SPU_CH_FREQ(channel) = getSPUSampleRate(audio.rate + audio.pitch);
    SPU_CH_ADDR(channel) = getSPUAddr(audio.data);
    SPU_CH_VOL_L(channel) = audio.volL;
    SPU_CH_VOL_R(channel) = audio.volR;
    SPU_CH_ADSR1(channel) = audio.adsr1;
    SPU_CH_ADSR2(channel) = audio.adsr2;

    SpuSetKey(1, (uint32_t)FLAG(channel));
}