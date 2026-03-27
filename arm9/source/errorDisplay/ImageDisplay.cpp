#include "ImageDisplay.h"

#include <nds.h>
#include <libtwl/mem/memVram.h>
#include <libtwl/gfx/gfx.h>
#include <libtwl/gfx/gfxBackground.h>
#include <libtwl/sys/sysPower.h>
#include <libtwl/dma/dmaNitro.h>
#include <cstring>
#include "VBlank.h"

ImageDisplay::ImageDisplay() {

}

static void InitVramMapping()
{
    mem_setVramAMapping(MEM_VRAM_AB_TEX_SLOT_1);
    mem_setVramBMapping(MEM_VRAM_AB_MAIN_OBJ_00000);
    mem_setVramCMapping(MEM_VRAM_C_SUB_BG_00000);
    mem_setVramDMapping(MEM_VRAM_D_TEX_SLOT_0);
    mem_setVramEMapping(MEM_VRAM_E_TEX_PLTT_SLOT_0123);
    mem_setVramFMapping(MEM_VRAM_FG_MAIN_BG_00000);
    mem_setVramGMapping(MEM_VRAM_FG_MAIN_BG_04000);
    mem_setVramHMapping(MEM_VRAM_H_SUB_BG_EXT_PLTT_SLOT_0123);
    mem_setVramIMapping(MEM_VRAM_I_SUB_OBJ_00000);
}

void ImageDisplay::DrawTop(const unsigned int* tiles, int tilesLen, const unsigned short* map, int mapLen, const unsigned short* palette, int paletteLen) {
    InitVramMapping();

    dma_ntrCopy32(3, tiles, GFX_BG_SUB, tilesLen);
    dma_ntrCopy32(3, map, (u8*)GFX_BG_SUB + 0x3000, mapLen);
    mem_setVramHMapping(MEM_VRAM_H_LCDC);
    dma_ntrCopy32(3, palette, (void*)0x0689A000, paletteLen);
    mem_setVramHMapping(MEM_VRAM_H_SUB_BG_EXT_PLTT_SLOT_0123);

    VBlank::Wait();

    sys_setMainEngineToBottomScreen();
    REG_DISPCNT_SUB = 0x40211015;
    REG_BG1HOFS_SUB = 0;
    REG_BG1VOFS_SUB = 0;
    REG_BG1CNT_SUB = 0x0680;
    REG_DISPCNT_SUB |= 1 << 9;
    REG_BLDCNT_SUB = 0x3D42;
    REG_BLDALPHA_SUB = 0x10;
    REG_MASTER_BRIGHT_SUB = 0;
}

void ImageDisplay::DrawBottom(const unsigned int* tiles, int tilesLen, const unsigned short* map, int mapLen, const unsigned short* palette, int paletteLen) {

}
