#include <pad.hpp>

uint8_t   Pad::pad_buff[2][34];
PADTYPE*  Pad::pad;
PadButton Pad::bindings[STATES_MAX];
uint16_t  Pad::states;

int Pad::Initialize() {
    InitPAD(pad_buff[0], 34, pad_buff[1], 34);
    StartPAD();
    ChangeClearPAD(0);

    pad = (PADTYPE *)pad_buff[0];

    return 0;
}

void Pad::Update() {
    states = (uint16_t)~pad->btn;
    /*
    states = 0;
    if (!pad->stat) for (int i = 1, x = 0; x < STATES_MAX; i = 1 << x, x++) {
        if (!(pad->btn & bindings[x])) states |= i;
    }
    */
}