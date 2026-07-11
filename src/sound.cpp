#include <psxspu.h>
#include <sound.hpp>
#include <types.hpp>

   VAG   Sound::audios[512];
uint16_t Sound::audiox;

int Sound::Initialize() {
    SpuInit();

    SpuSetTransferMode(SPU_TRANSFER_BY_DMA);
    
    SpuSetCommonMasterVolume(0x3FFF, 0x3FFF); 

    audiox = 0;
    audios[0].data = 0x1010;

    return 0;
}

int Sound::SetAudio(uint32_t* data) {
    VAG* audio = &audios[audiox++];

    audio->size = (SWAP_ENDIAN(data[3]) + 63) & 0xffffffc0;
    audio->rate = SWAP_ENDIAN(data[4]);

    SpuSetTransferStartAddr(audio->data);

    SpuWrite(data + 12, audio->size);
    SpuIsTransferCompleted(SPU_TRANSFER_WAIT);

    audios[audiox].data = audio->data + audio->size;

    return audiox -1;
}

void Sound::Play(uint16_t id, uint8_t channel) {
    VAG audio = audios[id];

    SpuSetKey(0, (uint32_t)FLAG(channel));

    SPU_CH_FREQ(channel) = getSPUSampleRate(audio.rate);
    SPU_CH_ADDR(channel) = getSPUAddr(audio.data);
    SPU_CH_VOL_L(channel) = 0x3fff;
    SPU_CH_VOL_R(channel) = 0x3fff;
    SPU_CH_ADSR1(channel) = 0x00ff;
    SPU_CH_ADSR2(channel) = 0x0000;

    SpuSetKey(1, (uint32_t)FLAG(channel));
}