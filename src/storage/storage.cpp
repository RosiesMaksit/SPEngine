#include <storage.hpp>
#include <types.hpp>

  File   Storage::files[64];
uint64_t Storage::flags_files;
uint32_t Storage::buffer[512];
uint16_t Storage::size_buffer;
uint16_t Storage::cursor_buffer;


int Storage::Initialize() {
    flags_files = 0;
    size_buffer = 0;
    cursor_buffer = 0;

    return InitializeScratchpad();
}

int Storage::AddFile(uint16_t size, uint32_t* data) {
    for (int i = 0; i < 64; i++) {
        if (flags_files & FLAG(i)) {
            if (files[i].size == size) {
                if (!memcmp(files[i].data, data, size)) {
                    return i;
                }
            }
            continue;
        }
        File* file = &files[i];
        uint16_t asize = (size + 3) & ~3;
        if (!asize) return -1;
        file->data = (uint8_t*)malloc(asize);
        file->size = size;

        memcpy(file->data, data, asize);

        flags_files |= FLAG(i);
        return i;
    }
    return -1;
}

File* Storage::GetFile(uint8_t id) {
    return &files[id];
}

void Storage::RemoveFile(uint8_t id) {
    free(files[id].data);
    flags_files &= ~FLAG(id);
}