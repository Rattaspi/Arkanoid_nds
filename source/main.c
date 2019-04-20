#include <nds.h>
#include <stdio.h>

//image includes
#include "tilemap.h"
#include "stars.h"

void Init();
void InitBackground();

const int DMA_CHANNEL = 3;

int main () {
	//https://patater.com/files/projects/manual/manual.html
	Init();
	printf("Boiiii");
	return 0;
}

void Init(){
	powerOn(POWER_ALL_2D);
	videoSetMode(MODE_5_2D);
	vramSetBankA(VRAM_A_MAIN_BG);

	int bg3 = bgInit(3, BgType_Bmp8, BgSize_B8_256x256, 0, 0);

	/*
	powerOn(POWER_ALL_2D);
	//lcdMainOnBottom();

	vramSetMainBanks(VRAM_A_MAIN_BG_0x06000000,
                     VRAM_B_MAIN_BG_0x06020000,
                     VRAM_C_SUB_BG_0x06200000,
                     VRAM_D_LCD);

	//Main screen
	videoSetMode(MODE_5_2D |
				 DISPLAY_BG3_ACTIVE);
	
	//Sub screen
	videoSetModeSub(MODE_5_2D |
					// DISPLAY_BG2_ACTIVE |
					DISPLAY_BG3_ACTIVE);

	InitBackground();

	//DISPLAY BACKGROUNDS
	dmaCopyHalfWords(DMA_CHANNEL, starsBitmap, (uint16*) BG_BMP_RAM(0), starsBitmapLen);
	// dmaCopyHalfWords(DMA_CHANNEL, bitmap2, (uint16*) BG_BMP_RAM(8), bitmmap2length);
	dmaCopyHalfWords(DMA_CHANNEL, starsBitmap, (uint16*) BG_BMP_RAM_SUB(0), starsBitmapLen);
	*/
}

void InitBackground() {
	//***BACKGROUND 3
	REG_BG3CNT = BG_BMP16_256x256 |
				 BG_BMP_BASE(0) | //Memory place
				 BG_PRIORITY(3); //Lowest priority

	//BG3 transformation matring (identity)
	REG_BG3PA = 1 << 8;
	REG_BG3PB = 0;
	REG_BG3PC = 0;
	REG_BG3PD = 1 << 8;

	//BG3 position;
	REG_BG3X = 20 << 8;
	REG_BG3Y = 0;

	//***BACKGROUND 2
	// REG_BG2CNT = BG_BMP16_128x128 |
	// 			 BG_BMP_BASE(8) |
	// 			 BG_PRIORITY(3);
	
	// //BG2 transformation matrix (identity)
	// REG_BG2PA = 1 << 8;
	// REG_BG2PB = 0;
	// REG_BG2PC = 0;
	// REG_BG2PD = 1 << 8;

	// //BG2 position
	// REG_BG2X = -(SCREEN_WIDTH / 2 - 32) << 8;
	// REG_BG2Y = -32 << 8;

	//***BACKGROUND 3 IN SUB SCREEN
	REG_BG3CNT_SUB = BG_BMP16_256x256 |
					 BG_BMP_BASE(0) |
					 BG_PRIORITY(3);
	REG_BG3PA_SUB = 1 << 8;
	REG_BG3PB_SUB = 0;
	REG_BG3PC_SUB = 0;
	REG_BG3PD_SUB = 1 << 8;

	REG_BG3X_SUB = 0;
	REG_BG3Y_SUB = 0;
}