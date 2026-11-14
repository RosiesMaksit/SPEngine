#include <parser.hpp>

int Parser::InitializeSound() {

    SPU_CTRL |= 0x0001;

    SPU_CD_VOL_L = 0x3fff;
    SPU_CD_VOL_R = 0x3fff;

    return 0;
}

void Parser::Play(uint8_t num) {
    uint8_t params[1];
    params[0] = num;

    CdControl(CdlPlay, params, 0);
}

void PlayFrom(uint8_t num, uint32_t second) {

}

void Parser::SetVolume(uint16_t L, uint16_t R) {
    SPU_CD_VOL_L = L>>2;
    SPU_CD_VOL_R = R>>2;
}

void Parser::Pause() {
    CdControl(CdlPause, nullptr, 0);

}
void Parser::Resume() {
    CdControl(CdlPlay, nullptr, 0);

}
void Parser::Stop() {
    CdControl(CdlStop, nullptr, 0); 

}

uint8_t Parser::IsPlaying() {
    uint8_t status = CdStatus();

    return (status & CdlStatPlay) == 0;
}