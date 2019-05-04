#include <nds.h>
#include <vector>
#include "Block.hpp"
#include "Avatar.hpp"
#include "Ball.hpp"

#include "block.h"

#define AVATAR_Y 170

int Sprite::mainID = 0;
int Sprite::subID = 0;

int main() {

	touchPosition touch;
	
	videoSetMode(MODE_0_2D);
	videoSetModeSub(MODE_0_2D);

	// map bank A for use with the background
	vramSetBankA(VRAM_A_MAIN_SPRITE);
	vramSetBankD(VRAM_D_SUB_SPRITE);

	//init the graphic system
	oamInit(&oamMain, SpriteMapping_1D_32, false);
	oamInit(&oamSub, SpriteMapping_1D_32, false);

	//init background
	//setBackdropColorSub	(	RGB15(15,15,31)	);
	//bgInitSub(3, BgType_Text4bpp, BgSize_T_256x256, 0, 0);

	//load the palettes
	dmaCopy((u8*)blockPal, SPRITE_PALETTE, sizeof(blockPal));
	dmaCopy((u8*)blockPal, SPRITE_PALETTE_SUB, sizeof(blockPal));

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
	Ball ball = Ball(100, AVATAR_Y + 6);
	
	while(1) {

		scanKeys();

		int held = keysHeld();

		if(held & KEY_TOUCH){
			touchRead(&touch);
		}
		else if(held & KEY_RIGHT){
			avatar.MoveRight();
		}
		else if(held & KEY_LEFT){
			avatar.MoveLeft();
		}

		if(held & KEY_START) break;

		//Draw the blocks
		for(int i = 0; i < blocks.size(); i++){
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
