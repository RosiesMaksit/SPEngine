#include <parser.hpp>

int Parser::InitializeMemoryCard() {
    InitCARD(0);
    StartCARD();
    _bu_init();

    return InitializeSound();
}

int Parser::CheckCard(uint8_t card) {
    card = card << 4;

    _card_info(card);

    _card_wait(card);

    for(;;) {
        int status = _card_status(card);

        if (status == 2) {
            _card_load(card);
            _card_wait(card);
            continue;
        }

        if (status == -2) {
            _card_clear(card);
            _card_wait(card);

            _card_load(card);
            _card_wait(card);
        }

        if (status == 1) return 0;

        if (status == -1) break;
        if (status == -3) break;

    }

    return 1;
}

void Parser::Save(const char* name, uint32_t* buffer, uint16_t size, uint8_t card) {
    open("bu00:", _O_CREAT | _O_WRONLY);
}