#ifndef STORAGE_HPP
#define STORAGE_HPP

#include <types.hpp>

typedef struct {
    uint8_t* data;
    uint16_t size;
    // uint16_t reserved;
} File;

class Storage {
    static File files[64];
    static uint64_t flags_files;

    static uint32_t buffer[512];
    static uint16_t size_buffer;
    static uint16_t cursor_buffer;

    static uint32_t (&scratchpad)[256]; // 4кб

    public:

    static int Initialize();

    // FILE
    static int AddFile(uint16_t size, uint32_t* data); // ну ~35к байт прям дохера
    static File* GetFile(uint8_t id);
    static void RemoveFile(uint8_t id);

    // SCRATCHPAD
    static int InitializeScratchpad();
};

#endif