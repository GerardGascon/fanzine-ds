#include "common.h"
#include <libtwl/card/card.h>
#include <libtwl/mem/memExtern.h>
#include <libtwl/rtos/rtosIrq.h>
#include <libtwl/rtos/rtosThread.h>
#include <libtwl/ipc/ipcSync.h>
#include <libtwl/ipc/ipcFifoSystem.h>
#include "core/Environment.h"
#include "errorDisplay/ImageDisplay.h"
#include "errorDisplay/VBlank.h"
#include <nds.h>

#include "Empty.h"
#include "IntroTop.h"
#include "IntroBottom.h"
#include "Pages1.h"
#include "Pages2.h"
#include "Pages3.h"
#include "Pages4.h"
#include "Pages5.h"
#include "Pages6.h"
#include "Pages7.h"
#include "Pages8.h"
#include "Pages9.h"
#include "Pages10.h"
#include "Pages11.h"
#include "Pages12.h"
#include "Pages13.h"
#include "Pages14.h"
#include "Pages15.h"
#include "Pages16.h"
#include "Pages17.h"
#include "Pages18.h"
#include "Pages19.h"
#include "Pages20.h"
#include "Pages21.h"
#include "Pages22.h"
#include "Pages23.h"
#include "Pages24.h"
#include "Pages25.h"
#include "Pages26.h"
#include "Pages27.h"
#include "Pages28.h"
#include "Pages29.h"
#include "Pages30.h"
#include "Pages30.h"
#include "Pages31.h"
#include "Pages34.h"
#include "Pages35.h"
#include "Pages36.h"
#include "Pages37.h"
#include "Pages38.h"
#include "Pages39.h"
#include "Pages40.h"

#define NTR_CMD_ID_GAME_DISABLE_SCRAMBLING      0xFC00000000000000ull

struct page {
	const unsigned int* bitmap;
	const unsigned short* palette;
};

struct page pages[] = {
	{ EmptyBitmap, EmptyPal }, { Pages1Bitmap, Pages1Pal },
	{ Pages2Bitmap, Pages2Pal }, { Pages3Bitmap, Pages3Pal },
	{ Pages4Bitmap, Pages4Pal }, { Pages5Bitmap, Pages5Pal },
	{ Pages6Bitmap, Pages6Pal }, { Pages7Bitmap, Pages7Pal },
	{ Pages8Bitmap, Pages8Pal }, { Pages9Bitmap, Pages9Pal },
	{ Pages10Bitmap, Pages10Pal }, { Pages11Bitmap, Pages11Pal },
	{ Pages12Bitmap, Pages12Pal }, { Pages13Bitmap, Pages13Pal },
	{ Pages14Bitmap, Pages14Pal }, { Pages15Bitmap, Pages15Pal },
	{ Pages16Bitmap, Pages16Pal }, { Pages17Bitmap, Pages17Pal },
	{ Pages18Bitmap, Pages18Pal }, { Pages19Bitmap, Pages19Pal },
	{ Pages20Bitmap, Pages20Pal }, { Pages21Bitmap, Pages21Pal },
	{ Pages22Bitmap, Pages22Pal }, { Pages23Bitmap, Pages23Pal },
	{ Pages24Bitmap, Pages24Pal }, { Pages25Bitmap, Pages25Pal },
	{ Pages26Bitmap, Pages26Pal }, { Pages27Bitmap, Pages27Pal },
	{ Pages28Bitmap, Pages28Pal }, { Pages29Bitmap, Pages29Pal },
	{ Pages30Bitmap, Pages30Pal }, { Pages31Bitmap, Pages31Pal },
	{ EmptyBitmap, EmptyPal }, { EmptyBitmap, EmptyPal },
	{ Pages34Bitmap, Pages34Pal }, { Pages35Bitmap, Pages35Pal },
	{ Pages36Bitmap, Pages36Pal }, { Pages37Bitmap, Pages37Pal },
	{ Pages38Bitmap, Pages38Pal }, { Pages39Bitmap, Pages39Pal },
	{ Pages40Bitmap, Pages40Pal }, { EmptyBitmap, EmptyPal },
};

static int totalPairs = 0;
static int currentPair = -1;

/// @brief Switches the DSpico into unscrambled game mode and disables scrambling.
static void disableScrambling()
{
    // Map slot 1 to arm9
    mem_setDsCartridgeCpu(EXMEMCNT_SLOT1_CPU_ARM9);

    // Switch the DSpico into unscrambled game mode
    card_romSetCmd(NTR_CMD_ID_GAME_DISABLE_SCRAMBLING);
    card_romStartXfer(MCCNT1_DIR_READ | MCCNT1_RESET_OFF | MCCNT1_CLK_6_7_MHZ | MCCNT1_LEN_0 | MCCNT1_CMD_SCRAMBLE |
        MCCNT1_LATENCY2(0) | MCCNT1_CLOCK_SCRAMBLER | MCCNT1_LATENCY1(24), false);
    card_romWaitBusy();

    // Set the seed of the scrambler to zero. As a result, it will only ever produce zero's.
    // This means that even if a command is send with scrambling enabled, it will have no effect.
    REG_MCCNT1 = 0;
    REG_MCSCR0 = 0;
    REG_MCSCR1 = 0;
    REG_MCSCR2 = 0;
    REG_MCCNT1 = MCCNT1_RESET_OFF | MCCNT1_APPLY_SCRAMBLE_SEED | MCCNT1_CLOCK_SCRAMBLER | MCCNT1_READ_DATA_DESCRAMBLE;
}

static void vblankIrq(u32 irqMask)
{
    VBlank::NotifyIrq();
}

void drawCurrentPage(ImageDisplay& display) {
    display.DrawTop(pages[currentPair * 2].bitmap, pages[currentPair * 2].palette);
    display.DrawBottom(pages[currentPair * 2 + 1].bitmap, pages[currentPair * 2 + 1].palette);
}

void nextPage(ImageDisplay& display) {
	if (currentPair >= totalPairs - 1)
		return;

	currentPair++;
	//mmEffectCancel(paper_handle);
	//paper_handle = mmEffectEx(&paper);
	drawCurrentPage(display);
}

void previousPage(ImageDisplay& display) {
	if (currentPair <= 0)
		return;

	currentPair--;
	//mmEffectCancel(paper_handle);
	//paper_handle = mmEffectEx(&paper);
	drawCurrentPage(display);
}

int main(int argc, char* argv[])
{
    Environment::Initialize();
    //mem_setDsCartridgeCpu(EXMEMCNT_SLOT1_CPU_ARM9);

    //rtos_initIrq();
    rtos_startMainThread();
    ipc_initFifoSystem();

    //VBlank::Init();

    while (ipc_getArm7SyncBits() != 7);

    disableScrambling();

    //rtos_setIrqFunc(RTOS_IRQ_VBLANK, vblankIrq);
    //rtos_enableIrqMask(RTOS_IRQ_VBLANK);

	touchPosition touchXY;

	ImageDisplay display;
    display.DrawTop(IntroTopBitmap, IntroTopPal);
    display.DrawBottom(IntroBottomBitmap, IntroBottomPal);

	totalPairs = sizeof(pages) / (2 * sizeof(pages[0]));
    while(1) {
		swiWaitForVBlank();
		scanKeys();
		int keys = keysDown();
		int keysHeldNow = keysHeld();

		if (keys & KEY_A) {
			nextPage(display);
		}

		if (keys & KEY_B) {
			previousPage(display);
		}

		if (keysHeldNow & KEY_TOUCH) {
			touchRead(&touchXY);

			nextPage(display);
			if (touchXY.py >= 192/2) {
				nextPage(display);
			} else {
				previousPage(display);
			}
		}
	}
}
