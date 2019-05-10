#include <nds.h>
#include <maxmod9.h>
#include <vector>
#include <time.h>

#include "Block.hpp"
#include "Avatar.hpp"
#include "Ball.hpp"

#include "block.h"

#include "soundbank.h"
#include "soundbank_bin.h"

#define AVATAR_Y 170

int Sprite::mainID = 0;
int Sprite::subID = 0;

int main() {
	swiWaitForVBlank();
	srand(time(NULL));
	touchPosition touch;
	
	videoSetMode(MODE_0_2D);
	videoSetModeSub(MODE_0_2D);

	// map bank A for use with the background
	vramSetBankA(VRAM_A_MAIN_SPRITE);
	vramSetBankD(VRAM_D_SUB_SPRITE);

	//init the graphic system
	oamInit(&oamMain, SpriteMapping_1D_32, false);
	oamInit(&oamSub, SpriteMapping_1D_32, false);

	//init audio
	mmInitDefaultMem((mm_addr)soundbank_bin);

	//init background
	//setBackdropColorSub	(	RGB15(15,15,31)	);
	//bgInitSub(3, BgType_Text4bpp, BgSize_T_256x256, 0, 0);

	//load the palettes
	dmaCopy((u8*)blockPal, SPRITE_PALETTE, sizeof(blockPal));
	dmaCopy((u8*)blockPal, SPRITE_PALETTE_SUB, sizeof(blockPal));

	//consoleDemoInit();

	//Level layout in the top screen
	std::vector<Block> blocks;
	std::pair<int, int> initialPos (40, 40);
	std::pair<int, int> blockOffset(18, 10);
	int maxColumns = 10;
	int maxRows = 5;
	
	for(int row = 0; row < maxRows; row++){
		for(int col = 0; col < maxColumns; col++){
			blocks.push_back(Block(2, initialPos.first + col * blockOffset.first, initialPos.second + row * blockOffset.second));
		}
	}

	//Avatar
	Avatar avatar = Avatar(4, 100, AVATAR_Y);

	//Ball
	Ball ball = Ball(100, AVATAR_Y - 6);
	ball.avatar = &avatar;
	ball.blockList = &blocks;

	//load sfx
	mmLoadEffect(SFX_HIT);
	mmLoadEffect(SFX_KILL);
	
	while(1) {

		scanKeys();

		int held = keysHeld();

		if(keysDown() & KEY_UP){
			mmEffect(SFX_HIT);
		}

		if(held & KEY_TOUCH){
			touchRead(&touch);
		}
		else if(held & KEY_RIGHT){
			avatar.MoveRight();
		}
		else if(held & KEY_LEFT){
			avatar.MoveLeft();
		}
		else if(held & KEY_A){
			ball.ChangeState(1);
		}

		if(held & KEY_START) break;

		//Draw the blocks
		for(unsigned int i = 0; i < blocks.size(); i++){
			blocks[i].Draw();
		}

		//Draw the avatar
		avatar.Draw();

		//Ball
		ball.Update();
		ball.Draw();

		swiWaitForVBlank();

		
		oamUpdate(&oamMain);
		oamUpdate(&oamSub);
	}

	return 0;
}
