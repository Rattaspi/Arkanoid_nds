#include <nds.h>
#include <maxmod9.h>
#include <vector>
#include <time.h>

#include "Block.hpp"
#include "Avatar.hpp"
#include "Ball.hpp"
#include "UIManager.hpp"
#include "BigSprite.hpp"
#include "BackgroundSpritesManager.hpp"

#include "block.h"
#include "bigImages.h"
#include "bgMain.h"
#include "bgSub.h"

#include "soundbank.h"
#include "soundbank_bin.h"

#define AVATAR_Y 170

int Sprite::mainID = 0;
int Sprite::subID = 0;

BigSprite cloud;
Avatar avatar;
Ball ball;
UIManager UI;
BackgroundSpritesManager bgsp;

//Level layout in the top screen
std::vector<Block> blocks;
std::pair<int, int> initialPos (40, 40);
std::pair<int, int> blockOffset(18, 10);
int maxColumns = 10;
int maxRows = 5;

bool playing = true;
bool gameOver = false;

void Init();
void Update();
void Gameover();
void Restart();

int main() {
	Init();	

	while(playing) {
		Update();
	}

	return 0;
}


void Init(){
	swiWaitForVBlank();
	srand(time(NULL));
	
	videoSetMode(MODE_5_2D);
	videoSetModeSub(MODE_5_2D);

	// map bank A for use with the background
	vramSetBankA(VRAM_A_MAIN_SPRITE);
	vramSetBankB(VRAM_B_MAIN_BG);
	vramSetBankC(VRAM_C_SUB_BG);
	vramSetBankD(VRAM_D_SUB_SPRITE);

	//init the graphic system
	oamInit(&oamMain, SpriteMapping_1D_32, false);
	oamInit(&oamSub, SpriteMapping_1D_32, false);

	//init audio
	mmInitDefaultMem((mm_addr)soundbank_bin);

	//init background
	setBackdropColor(RGB15(15,15,31));
	setBackdropColorSub(RGB15(15,15,31));

	int bgMain = bgInit(3, BgType_Bmp8, BgSize_B8_256x256, 0, 0);
	dmaCopy(bgMainBitmap, bgGetGfxPtr(bgMain), sizeof(bgMainBitmap));

	int bgSub = bgInitSub(3, BgType_Bmp8, BgSize_B8_256x256, 0, 0);
	dmaCopy(bgSubBitmap, bgGetGfxPtr(bgSub), sizeof(bgSubBitmap)*2);


	//load the palettes (background)
	dmaCopy((u16*)bgMainPal, BG_PALETTE, sizeof(bgMainPal));
	dmaCopy((u16*)bgSubPal, BG_PALETTE_SUB, sizeof(bgSubPal));

	
	//load the palettes (sprites)
	dmaCopy((u8*)blockPal, SPRITE_PALETTE, sizeof(blockPal));
	dmaCopy((u8*)blockPal, SPRITE_PALETTE_SUB, sizeof(blockPal));
	dmaCopy((u8*)bigImagesPal, SPRITE_PALETTE + 10, sizeof(bigImagesPal));
	dmaCopy((u8*)bigImagesPal, SPRITE_PALETTE_SUB + 10, sizeof(bigImagesPal));

	//load sfx
	mmLoadEffect(SFX_HIT);
	mmLoadEffect(SFX_KILL);

	//BACKGROUND STUFF
	bgsp = BackgroundSpritesManager();

	//Blocks layout
	for(int row = 0; row < maxRows; row++){
		for(int col = 0; col < maxColumns; col++){
			blocks.push_back(Block(2, initialPos.first + col * blockOffset.first, initialPos.second + row * blockOffset.second));
		}
	}

	//Avatar
	avatar = Avatar(4, 100, AVATAR_Y);

	//Ball
	ball = Ball(100, AVATAR_Y - 6);
	ball.avatar = &avatar;
	ball.blockList = &blocks;

	//init UI manager
	UI = UIManager();
	ball.UI = &UI;

}

void Update(){
	scanKeys();

	int held = keysHeld();
	int down = keysDown();

	if((down & KEY_UP) && gameOver){
		Restart();
	}
	else if(down & KEY_DOWN){
		Gameover();
	}

	if(held & KEY_RIGHT){
		avatar.MoveRight();
	}
	else if(held & KEY_LEFT){
		avatar.MoveLeft();
	}
	else if(held & KEY_A){
		ball.ChangeState(1);
	}

	if(held & KEY_START){
		playing = false;
	}

	//load the palettes (background)
	dmaCopy((u16*)bgMainPal, BG_PALETTE, sizeof(bgMainPal));
	dmaCopy((u16*)bgSubPal, BG_PALETTE_SUB, sizeof(bgSubPal));
	

	if(!gameOver){
		//Draw the blocks
		for(unsigned int i = 0; i < blocks.size(); i++){
			blocks[i].Draw();
		}

		cloud.PlaceSprite();

		//Draw the avatar
		avatar.Draw();

		//Ball
		ball.Update();
		ball.Draw();

		//UI
		UI.Draw();

		//Check end game condition
		if(UI.hp <= 0){
			gameOver = true;
			Gameover();
		}
	}

	//Draw the background
	bgsp.Update();
	bgsp.Draw();

	swiWaitForVBlank();

	
	oamUpdate(&oamMain);
	oamUpdate(&oamSub);
}

void Gameover(){
	gameOver = true;
	for(unsigned int i = 0; i < blocks.size(); i++){
		blocks[i].Kill();
		//Force redraw to hide them
		blocks[i].Draw();
	}
}

void Restart(){
	gameOver = false;
	for(unsigned int i = 0; i < blocks.size(); i++){
		blocks[i].Reset();
	}
	UI.hp = UI.GetMaxHP();
	ball.ChangeState(0);
}