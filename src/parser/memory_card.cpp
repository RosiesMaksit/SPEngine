#include <parser.hpp>
#include <string.h>

// Размер одного сектора на карте памяти PlayStation обычно равен 128 байтам
#define SECTOR_SIZE 128
#define CARD_CHANNEL 0  // Слот 1 (обычно 0 или 1)
#define TARGET_SECTOR 5 // Номер сектора для теста

#define O_RDONLY 0x0001
#define O_WRONLY 0x0002
#define O_CREAT  0x0200

int Parser::InitializeMemoryCard() {
    InitCARD(0);
    StartCARD();
    _bu_init();
    _new_card();

    return InitializeSound();
}

int Parser::CheckCard(uint8_t card) {
    _card_info(card);
    return _card_wait(card);
}

void Parser::SaveCard(uint8_t card, const char* name, uint32_t* buffer, uint16_t size) {
    char buf[20]; // 15 символов на имя
    sprintf(buf, "bu%d0:%s", card, name);
    int fd = open(buf, 0x0002 | 0x0200);
}

void Parser::test(void) {
    // 1. Инициализация «железа» и драйвера файловой системы
    InitCARD(1);
    StartCARD();
    _bu_init(); // <--- Магия здесь: регистрирует префиксы "bu00:" и "bu10:"
    _new_card();

    // 2. Проверка карты в первом слоте (опционально, но полезно)
    // _card_info инициализирует чтение структуры файловой системы с карты
    _card_info(0); 
    _card_wait(0); // Обязательно ждем завершения опроса карты

    // Данные для записи (например, структура сохранения)
    char my_save_data[256] = "PLAYER_LEVEL=50;GOLD=99999;";
    
    // 3. ЗАПИСЬ ФАЙЛА
    // Создаем и открываем файл на первой карте памяти.
    // Имя файла на PS1 обычно имеет строгий формат, например, "bu00:BESLEM-99999SAV"
    int fd = open("bu00:TEST_SAVE", O_WRONLY | O_CREAT);
    if (fd >= 0) {
        // Записываем данные в файл
        write(fd, my_save_data, sizeof(my_save_data));
        // Закрываем файл, чтобы BIOS сбросил кэш на физическую карту
        close(fd);
    }

    // 4. ЧТЕНИЕ ФАЙЛА
    char read_buffer[256] = {0};
    int fd_read = open("bu00:TEST_SAVE", O_RDONLY);
    if (fd_read >= 0) {
        read(fd_read, read_buffer, sizeof(read_buffer));
        close(fd_read);
        // Теперь в read_buffer лежат считанные данные
    }

    // Освобождаем подсистему карты
    StopCARD();
}
