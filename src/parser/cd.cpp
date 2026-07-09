#include <parser.hpp>
#include <types.hpp>

uint32_t Parser::buffer[SECTORS_BUFFER * 512];
CdlFILE Parser::infofile;

int Parser::InitializeCD() {
    CdInit();

    return 0;
}

void Parser::ReadFile(const char* name) {
    char buf[20] = "\\"; // 16

    if (!CdSearchFile(&infofile, strcat(strcat(buf, name), ";1"))) return;

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