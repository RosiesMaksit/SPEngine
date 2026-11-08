#include <parser.hpp>
#include <types.hpp>

uint32_t Parser::buffer[SECTORS_BUFFER * SIZE_BUFFER_DISK];
CdlFILE Parser::infofile;

int Parser::InitializeCD() {
    CdInit();

    return InitializeMemoryCard();
}

void Parser::ReadFile(const char* name) {
    char buf[20] = "\\"; // 16
    buf[1] = '\0';
    buf[0] = '\\';

    int i = 0;
    while (name[i] != '\0' && i < 16) {
        buf[i + 1] = name[i];
        i++;
    }
    
    // Дописываем ISO-хвост и закрываем строку
    buf[i + 1] = ';';
    buf[i + 2] = '1';
    buf[i + 3] = '\0';
    if (!CdSearchFile(&infofile, buf)) return;

    CdControl(CdlSetloc, &infofile.pos, __null);

    uint16_t size_file = (infofile.size + 2047) / 2048;

    CdRead(size_file, buffer, CdlModeSpeed);
}

int Parser::CheckRead(uint8_t mode) {
    return CdReadSync(mode,nullptr);
}

uint32_t* Parser::LoadFile() {
    return buffer;
}

uint16_t Parser::GetSizeFile() {
    return (uint16_t)infofile.size;
}