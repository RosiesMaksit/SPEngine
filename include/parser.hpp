#ifndef PARSER_HPP
#define PARSER_HPP

#define SECTORS_BUFFER 256

#include <types.hpp>


class Parser {

    static uint32_t buffer[SECTORS_BUFFER * 512];
    static CdlFILE infofile;

    public:
    // СИДИ
    static int  InitializeCD();

    static void ReadFile(const char* name);
    static int  CheckRead(uint8_t mode);
    static uint32_t* LoadFile();
    static uint16_t GetSizeFile();
    
    // МЕМОРИ
    static int InitializeMemoryCard();

    // САУНД (ну типа музыка с диска чтобы оперативку не засорять)

};

#endif