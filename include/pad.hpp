#ifndef PAD_HPP
#define PAD_HPP

#include <types.hpp>

typedef enum {
    START    = 1 << 0,
    CANCEL   = 1 << 1,
    LEFT     = 1 << 2,
    RIGHT    = 1 << 3,
    UP       = 1 << 4,
    DOWN     = 1 << 5,
    USE      = 1 << 6,
    ACTION   = 1 << 7, // альт use
    INFO     = 1 << 8,
    ITEMS    = 1 << 9,
    MODIFIER = 1 << 10,
    UTILITY  = 1 << 11,

    STATES_MAX = 12
} States;

class Pad {
    static uint8_t pad_buff[2][34];
    static PADTYPE* pad;

    public:
    static PadButton bindings[12];
    static uint32_t states;

    static int Initialize();
    static void Update();

    static void Bind(PadButton key, States state);

};

#endif
