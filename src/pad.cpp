#include <pad.hpp>

uint8_t   Pad::pad_buff[2][34];
PADTYPE*  Pad::pad;
InputBind Pad::bindings[12];
uint32_t  Pad::states;

int Pad::Initialize() {
    InitPAD(pad_buff[0], 34, pad_buff[1], 34);
    StartPAD();
    ChangeClearPAD(0);

    bindings[0]  = { PAD_START,    START    };
    bindings[1]  = { PAD_TRIANGLE, CANCEL   };
    bindings[2]  = { PAD_LEFT,     LEFT     };
    bindings[3]  = { PAD_RIGHT,    RIGHT    };
    bindings[4]  = { PAD_UP,       UP       };
    bindings[5]  = { PAD_DOWN,     DOWN     };
    bindings[6]  = { PAD_CROSS,    USE      };
    bindings[7]  = { PAD_CIRCLE,   ACTION   };
    bindings[8]  = { PAD_L1,       INFO     };
    bindings[9]  = { PAD_L2,       ITEMS    };
    bindings[10] = { PAD_R1,       MODIFIER };
    bindings[11] = { PAD_R2,       UTILITY  };

    pad = (PADTYPE *)pad_buff[0];

    return 0;
}

void Pad::Update() {
    states = 0;
    if (!pad->stat) for (int i = 0; i < 12; i++) {
        if (!(pad->btn & bindings[i].key)) states |= bindings[i].state;
    }
}