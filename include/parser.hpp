#ifndef PARSER_HPP
#define PARSER_HPP

#define SECTORS_BUFFER 256

#include <types.hpp>

typedef enum {
	CTP_ADDR_CONTROLLER  = 0x01, // PS1 or PS2 controller
	CTP_ADDR_ACCESS_CARD = 0x21, // PS1 Net Yaroze Access Card
	CTP_ADDR_IMODE       = 0x41, // PS1 i-Mode adapter cable
	CTP_ADDR_REMOTE      = 0x61, // PS2 DVD remote receiver
	CTP_ADDR_MEMORY_CARD = 0x81  // PS1 memory card or PocketStation
} CTPDeviceAddress;

class Parser {
    friend class Game;

    static uint32_t buffer[SECTORS_BUFFER * SIZE_BUFFER_DISK];
    static CdlFILE infofile;

    static uint8_t rd[139];
    static uint8_t wr[137];
    static uint8_t req[134];
    static uint8_t selfSlot;

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

    /// @param card - КАРТА (первая 0, вторая 1)
    /// @param sector - СЕКТОР (от 0 до 63) по 128 байт
    /// @param size - РАЗМЕР буфера в байтах
    static void SaveCard(uint8_t card, uint8_t sector, const void* data, uint16_t size);

    /// @param card - КАРТА (первая 0, вторая 1)
    /// @param sector - СЕКТОР (от 0 до 64) по 128 байт
    /// @param data - УКАЗАТЕЛЬ на байты
    static void LoadCard(uint8_t card, uint8_t sector, void* data, uint16_t size);

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