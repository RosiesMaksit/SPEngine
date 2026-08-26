#include <pad.hpp>

uint8_t   Pad::pad_buff[2][34];
PADTYPE*  Pad::pad;
PadButton Pad::bindings[12];
uint32_t  Pad::states;

int Pad::Initialize() {
    InitPAD(pad_buff[0], 34, pad_buff[1], 34);
    StartPAD();
    ChangeClearPAD(0);

    bindings[START] = PAD_START;
    bindings[CANCEL] = PAD_TRIANGLE;
    bindings[LEFT] = PAD_LEFT;
    bindings[RIGHT] = PAD_RIGHT;
    bindings[UP] = PAD_UP;
    bindings[DOWN] = PAD_DOWN;
    bindings[USE] = PAD_CROSS;
    bindings[ACTION] = PAD_CIRCLE;
    bindings[INFO] = PAD_L1;
    bindings[ITEMS] = PAD_L2;
    bindings[MODIFIER] = PAD_R1;
    bindings[UTILITY] = PAD_R2;

    pad = (PADTYPE *)pad_buff[0];

    return 0;
}

void Pad::Update() {
    states = 0;
    if (!pad->stat) for (int i = 0, x = 0; x < STATES_MAX; i = 1 << x, x++) {
        if (!(pad->btn & bindings[i])) states |= i;
    }
}