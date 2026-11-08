#include <storage.hpp>
#include <types.hpp>

  File   Storage::files[MAX_COUNT_FILES];
uint64_t Storage::flags_files;
uint32_t Storage::buffer[SIZE_STORAGE_BUFFER];
uint16_t Storage::size_buffer;
uint16_t Storage::cursor_buffer;


int Storage::Initialize() {
    flags_files = 0;
    size_buffer = 0;
    cursor_buffer = 0;

    return InitializeScratchpad();
}

int Storage::AddFile(uint16_t size, uint32_t* data) {
    for (int i = 0; i < MAX_COUNT_FILES; i++) {
        if (flags_files & FLAG(i)) continue;
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