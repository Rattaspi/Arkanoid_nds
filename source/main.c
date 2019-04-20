/*---------------------------------------------------------------------------------

   Simple tilemap demo
   -- WinterMute

---------------------------------------------------------------------------------*/
#include <nds.h>

#include "tilemap.h"
#include "stars.h"

#define PAD_Y (uint16)170

typedef struct Point {
		uint16 x;
		uint16 y;
	} Point;

int main(void) {

	touchPosition touch;
	
	Point pos = {SCREEN_WIDTH/2,SCREEN_HEIGHT/2};
	
	//videoSetMode(MODE_0_2D);
	videoSetModeSub(MODE_0_2D);

	// map bank A for use with the background
	vramSetBankA(VRAM_A_MAIN_SPRITE);
	vramSetBankD(VRAM_D_SUB_SPRITE);

	//init the graphic system
	oamInit(&oamMain, SpriteMapping_1D_32, false);
	oamInit(&oamSub, SpriteMapping_1D_32, false);

	//load the sprites
	u16* gfx = oamAllocateGfx(&oamMain, SpriteSize_16x16, SpriteColorFormat_256Color);
	u16* gfxSub = oamAllocateGfx(&oamSub, SpriteSize_32x8, SpriteColorFormat_256Color);

	int i = 0;
	for(i = 0; i < 16 * 32 / 2; i++)
	{
		gfx[i] = 1 | (1 << 8);
		gfxSub[i] = 1 | (1 << 8);
	}

	SPRITE_PALETTE[1] = RGB15(31,0,0);
	SPRITE_PALETTE_SUB[1] = RGB15(0,31,0);

	while(1) {

		scanKeys();

		int held = keysHeld();

		if(held & KEY_TOUCH){
			touchRead(&touch);
		}
		else if(held & KEY_RIGHT){
			pos.x += 1;
		}
		else if(held & KEY_LEFT){
			pos.x -= 1;
		}

		if(held & KEY_START) break;

		/*
		oamSet(&oamMain, //main graphics engine context
			0,           //oam index (0 to 127)  
			touch.px, touch.py,   //x and y pixle location of the sprite
			0,                    //priority, lower renders last (on top)
			0,					  //this is the palette index if multiple palettes or the alpha value if bmp sprite	
			SpriteSize_16x16,     
			SpriteColorFormat_256Color, 
			gfx,                  //pointer to the loaded graphics
			-1,                  //sprite rotation data  
			false,               //double the size when rotating?
			false,			//hide the sprite?
			false, false, //vflip, hflip
			false	//apply mosaic
			);              
		
		*/

		oamSet(&oamSub, 
			0, 
			pos.x, PAD_Y,
			0, 
			0,
			SpriteSize_32x8, 
			SpriteColorFormat_256Color, 
			gfxSub, 
			-1, 
			false, 
			false,			
			false, false, 
			false	
			);              
	
		swiWaitForVBlank();

		
		oamUpdate(&oamMain);
		oamUpdate(&oamSub);
	}

	return 0;
}
