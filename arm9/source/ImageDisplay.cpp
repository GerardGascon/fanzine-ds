#include "ImageDisplay.h"

#include <nds.h>
#include <libtwl/mem/memVram.h>
#include <libtwl/gfx/gfx.h>
#include <libtwl/gfx/gfxBackground.h>
#include <libtwl/sys/sysPower.h>
#include <libtwl/dma/dmaNitro.h>
#include <cstring>

static int bg;
static int bgSub;

ImageDisplay::ImageDisplay() {
    videoSetMode(MODE_5_2D);
    vramSetBankA(VRAM_A_MAIN_BG);
    bg = bgInit(3, BgType_Bmp8, BgSize_B8_256x256, 0, 0);

    videoSetModeSub(MODE_5_2D);
    vramSetBankC(VRAM_C_SUB_BG);
    bgSub = bgInitSub(3, BgType_Bmp8, BgSize_B8_256x256, 0, 0);
}

static u8 temp[192 * 192] __attribute__((aligned(4)));
static void drawBitmap8(const void* lz77Data, const u16* palette, void* paletteDst, u16* vram, int xOff, int yOff) {
    decompress(lz77Data, temp, LZ77);
    DC_FlushRange(temp, 192*192);

    if (palette) {
        DC_FlushRange((void*)palette, 256*2);
        dmaCopy(palette, paletteDst, 256 * 2); // 256 colors, 2 bytes each
    }

    // Copy each line to VRAM
    for (int y = 0; y < 192; y++) {
        dmaCopy((u16*)&temp[y*192], &vram[(y + yOff) * 256/2 + xOff], 192);
    }
}

void ImageDisplay::DrawTop(const unsigned int* bitmap, const unsigned short* palette) {
    drawBitmap8(bitmap, palette, BG_PALETTE, bgGetGfxPtr(bg), 16, 0);
}

void ImageDisplay::DrawBottom(const unsigned int* bitmap, const unsigned short* palette) {
    drawBitmap8(bitmap, palette, BG_PALETTE_SUB, bgGetGfxPtr(bgSub), 16, 0);
}
