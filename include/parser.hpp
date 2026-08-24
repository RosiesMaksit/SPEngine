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

    static int CheckCard(uint8_t card);

    static void Save(const char* name, uint32_t* buffer, uint16_t size, uint8_t card);

    // САУНД (ну типа музыка с диска чтобы оперативку не засорять)
    static int InitializeSound();

    static void Play(uint8_t num);
    static void PlayFrom(uint8_t num, uint32_t second);

    static void SetVolume(uint16_t L, uint16_t R);

    static void Pause();
    static void Resume();
    static void Stop();

    static uint8_t IsPlaying();

};

#endif