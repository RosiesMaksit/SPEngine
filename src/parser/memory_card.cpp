#include <parser.hpp>
#include "registers.h"
#include <pad.hpp>
#include <string.h>

static inline void initsio0();

static void delayMicroseconds(int time);

static inline void selectSIO0Port(int port);

static uint8_t exchangeByte(uint8_t value);

static bool waitForAcknowledge(void);

static size_t exchangeCTPPacket(
	CTPDeviceAddress address,
	const uint8_t    *request,
	uint8_t          *response,
	size_t           reqLength,
	size_t           maxRespLength
);

uint8_t Parser::rd[139];
uint8_t Parser::wr[137];
uint8_t Parser::req[134];
uint8_t Parser::selfSlot;

int Parser::InitializeMemoryCard() {
	initsio0();

    return InitializeSound();
}

int Parser::CheckCard(uint8_t card) {
    StopPAD();
    initsio0();
    selectSIO0Port(card);

    selfSlot = 0xff;
    uint8_t freeSlot = 0xff;

    memset(req, 0, 134);
    req[0] = 82; // 0x52 ('R') - чтение

    int len = exchangeCTPPacket(CTP_ADDR_MEMORY_CARD, req, rd, 3, 139);

    if (len != 139 || rd[138] != 0x47) {
        Pad::Initialize();
        return -1; // карты нет
    }

    if (rd[8] != 'M' || rd[9] != 'C') {
        selfSlot = 0;
        Pad::Initialize();
        return -2; // карта не отформатирована
    }

    // 1. Сканируем все 15 директорий
    for (int i = 1; i < 16; i++) {
        memset(req, 0, 134);
        req[0] = 82; // Чтение
        req[3] = i >> 8;
        req[4] = i & 0xff;

        len = exchangeCTPPacket(CTP_ADDR_MEMORY_CARD, req, rd, 5, 139);

        if (len == 139 && rd[138] == 0x47) {
            uint8_t bt = rd[9];

            // Проверяем, не наш ли это занятый слот (0x51 - первый/единственный блок файла)
            if (bt == 0x51) {
                if (strncmp((const char*)&rd[19], NAMECARD, 20) == 0) {
                    selfSlot = i - 1;
                    break; // Нашли свой сейв, дальше искать нет смысла
                }
            } 
            // Запоминаем первый попавшийся свободный слот на случай, если свой не найдем
            else if ((bt == 0x00 || bt == 0xa1) && freeSlot == 0xff) {
                freeSlot = i - 1;
            }
        }
    }

    // 2. Если свой слот не найден, но есть свободный — занимаем его
    if (selfSlot == 0xff && freeSlot != 0xff) {
        uint16_t dirFrame = freeSlot + 1;

        memset(req, 0, 134);
        req[0] = 0x57; // 'W' (87) - команда записи в карту памяти
        // req[1..2] обычно зарезервированы под адрес/заголовок пакета CTP
        req[3] = dirFrame >> 8;
        req[4] = dirFrame & 0xff;

        // Формируем 128 байт Directory Frame (начиная с req[5])
        uint8_t* frame = &req[5];
        frame[0] = 0x51;             // Помечаем слот как занятый (1 блок)
        frame[4] = 0x00;             // Размер файла: 8192 байта (0x00002000 в Little Endian)
        frame[5] = 0x20;
        frame[6] = 0x00;
        frame[7] = 0x00;
        frame[8] = 0xff;             // Следующего блока нет (0xFFFF)
        frame[9] = 0xff;

        // Копируем наше уникальное имя (смещение 10 в фрейме)
        strncpy((char*)&frame[10], NAMECARD, 20);

        // Считаем контрольную сумму XOR для первых 127 байт фрейма
        uint8_t xorSum = 0;
        for (int b = 0; b < 127; b++) {
            xorSum ^= frame[b];
        }
        frame[127] = xorSum;

        // Отправляем пакет записи (5 байт заголовка + 128 байт данных = 133 байта)
        len = exchangeCTPPacket(CTP_ADDR_MEMORY_CARD, req, rd, 133, 139);

        // Проверяем успешность записи (0x47 == 'G' - Good)
        if (len == 139 && rd[138] == 0x47) {
            selfSlot = freeSlot;
        }
    }

    Pad::Initialize();
    return (selfSlot != 0xff) ? 0 : -1;
}

void Parser::SaveCard(uint8_t card, uint8_t sector, const void* data, uint16_t size) {
    StopPAD();
    initsio0();
    selectSIO0Port(card);

    int absSector = 64 + selfSlot * 64 + sector;

	memset(req, 0x35, 134);

    req[0] = 87;
	req[1] = 0;
	req[2] = 0;
	req[3] = absSector >> 8;
	req[4] = absSector & 0xFF;

    memcpy(&req[5], data, size);

	uint8_t xor_sum = req[1] ^ req[2];
	for (int i = 0; i < 128; i++) {
    	xor_sum ^= req[3 + i];
	}
	req[133] = xor_sum;

    int len = exchangeCTPPacket(CTP_ADDR_MEMORY_CARD, req, wr, 134, 137);

    if (len == 137 && wr[136] == 0x47) {

    }
    
    Pad::Initialize();
}

void Parser::LoadCard(uint8_t card, uint8_t sector, void* data, uint16_t size) {
    StopPAD();
    initsio0();
    selectSIO0Port(card);

    memset(rd, 0, 138);
    
    int absSector = 64 + selfSlot * 64 + sector;

    req[0] = 82;
	req[1] = 0;
	req[2] = 0;
	req[3] = absSector >> 8;
	req[4] = absSector & 0xFF;

    int len = exchangeCTPPacket(CTP_ADDR_MEMORY_CARD, req, rd, 5, 139);

    if (len == 139 && rd[138] == 0x47) {
        memcpy(data, &rd[9], size);
    }
    
    Pad::Initialize();
}

// pizda

static inline void initsio0() {
    SIO_CR(0) = SIO_CR_INTRST;

	SIO_MR(0) = 0
		| SIO_MR_BR_DIV1
		| SIO_MR_CHLEN_8;
	SIO_BR(0) = F_CPU / 250000;
	SIO_CR(0) = 0
		| SIO_CR_TXEN
		| SIO_CR_RXEN
		| SIO_CR_DSRIEN;
	INTC_MASK &= ~(1 << IRQ_SIO0);
}

static void delayMicroseconds(int time) {
	time = (time * 271 + 4) / 8;

	__asm__ volatile(
		".set push\n"
		".set noreorder\n"
		"bgtz  %0, .\n"
		"addiu %0, -2\n"
		".set pop\n"
		: "+r"(time)
	);
}

static inline void selectSIO0Port(int port) {
	uint16_t cr = port ? SIO_CR_PORT_2 : SIO_CR_PORT_1;
	SIO_CR(0)   = (SIO_CR(0) & ~SIO_CR_PORT_BITMASK) | cr;
}

static uint8_t exchangeByte(uint8_t value) {
	while (!(SIO_SR(0) & SIO_SR_TXRDY))
		__asm__ volatile("");

	SIO_DR(0) = value;

	while (!(SIO_SR(0) & SIO_SR_RXRDY))
		__asm__ volatile("");

	return SIO_DR(0);
}

static bool waitForAcknowledge(void) {

	for (int timeout = 100; timeout > 0; timeout -= 5) {
		if (INTC_REG & (1 << IRQ_SIO0)) {
			for (int poll = 20; poll > 0; poll--) {
				if (!(SIO_SR(0) & SIO_SR_DSR))
					break;
			}
			INTC_REG   = ~(1 << IRQ_SIO0);
			SIO_CR(0) |= SIO_CR_ERRRST;

			return true;
		}

		delayMicroseconds(5);
	}

	return false;
}

static size_t exchangeCTPPacket(
	CTPDeviceAddress address,
	const uint8_t    *request,
	uint8_t          *response,
	size_t           reqLength,
	size_t           maxRespLength
) {
	while (SIO_SR(0) & SIO_SR_RXRDY)
		SIO_DR(0);

	INTC_REG   = ~(1 << IRQ_SIO0);
	SIO_CR(0) |= SIO_CR_DTR | SIO_CR_ERRRST;
	delayMicroseconds(300);

	exchangeByte(address);

	size_t respLength = 0;

	for (; respLength < maxRespLength; respLength++) {

		if (!waitForAcknowledge())
			break;

		if (reqLength > 0) {
			*(response++) = exchangeByte(*(request++));
			reqLength--;
		} else {
			*(response++) = exchangeByte(0);
		}
	}

	SIO_CR(0) &= ~SIO_CR_DTR;

	return respLength;
}