#ifndef TYPES_HPP
#define TYPES_HPP

#include <psxapi.h>
#include <psxgpu.h>
#include <psxgte.h>
#include <psxpad.h>
#include <psxcd.h>
#include <inline_c.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <stdio.h>

#define WIDTH 320
#define HEIGHT 240
#define LENGTH_OT 4096

#define SIGN(X) (((X) > 0) - ((X) < 0))
#define ABS(X)  ((X) < 0 ? -(X) : (X))
#define MAX(A, B) ((A) > (B) ? (A) : (B))
#define MIN(A, B) ((A) < (B) ? (A) : (B))

#define SWAP_ENDIAN(x) ( \
	(((uint32_t) (x) & 0x000000ff) << 24) | \
	(((uint32_t) (x) & 0x0000ff00) <<  8) | \
	(((uint32_t) (x) & 0x00ff0000) >>  8) | \
	(((uint32_t) (x) & 0xff000000) >> 24) \
)

typedef struct {
    DISPENV disp;
    DRAWENV draw;
    uint32_t ot[LENGTH_OT];
    POLY_F4  tiles[512];
    DR_TPAGE dr_tpages[512];
    POLY_FT4 tims [512];
    POLY_F3  polys[512];
} DB;

enum Flags {
    ACTIVE    = 1 << 0,        // 1  (00000001) -              Активен
    VISUAL    = 1 << 1,       // 2  (00000010) - Визуализация включена
    PRIMITIVE = 1 << 2,      // 4  (00000100) -     Примитивы включены
    MESH      = 1 << 3,     // 8 (00010000) -         3D режим включен
    COLOR     = 1 << 4,    // 16 (00100000) -              Цвет включен
    TRANS     = 1 << 5,    // 32 (01000000) -      Прозрачность включена

    ABR_0     = 0 << 6,  // (00) - 50% + 50% (Дефолт, стекло/вода)
    ABR_1     = 1 << 6,  // (01) - 100% + 100% (Аддитивный, огонь/свечение)
    ABR_2     = 2 << 6,  // (10) - 100% - 100% (Вычитание, ДЛЯ ЧЕРНОГО ФЕЙДА)
    ABR_3     = 3 << 6   // (11) - 100% + 25% (Слабая подсветка) ебат такое существует?
    // я честно вообще думал есть только 2 режима - 50/50 и 100/100
};

#define ABR_MASK (3 << 6)
#define SET_ABR(flags, abr_value) (flags = ((flags) & ~ABR_MASK) | (abr_value))
// спс нейронка за эти два макроса

#define FLAG(n) (1ULL << (n))

#endif