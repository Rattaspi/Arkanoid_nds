#include <nds.h>
#include <maxmod9.h>
#include <iostream>
#include <vector>
#include <time.h>

#include "Block.hpp"
#include "Avatar.hpp"
#include "Ball.hpp"
#include "UIManager.hpp"
#include "BigSprite.hpp"
#include "BackgroundSpritesManager.hpp"
#include "MusicManager.hpp"

#include "block.h"
#include "bigImages.h"
#include "bgMain.h"
#include "bgSub.h"
#include "bgSub1.h"
#include "bgSub2.h"

#include "soundbank.h"
#include "soundbank_bin.h"

#define AVATAR_Y 170

int Sprite::mainID = 0;
int Sprite::subID = 0;

Avatar avatar;
Ball ball;
UIManager UI;
BackgroundSpritesManager bgsp;
MusicManager mm;

BigSprite gameover1, gameover2;

//Level layout in the top screen
std::vector<Block> blocks;
std::pair<int, int> initialPos (40, 40);
std::pair<int, int> blockOffset(18, 10);
int maxColumns = 10;
int maxRows = 5;

std::vector<const unsigned int*> bgSequence;
std::vector<u16*> bgPalettesSequence;
uint8_t displayBg = 0;
uint8_t displayBgFrameCounter = 0;
int bgMain;
int bgSub;

bool playing = true;
bool gameOver = false;

void* soundPtr = nullptr;

void Init();
void Step();
void Quit();
void Gameover();
void Restart();
bool CheckWinCondition();

int main() {
	for (int i = 0; i < 50; i++){
		Init();
		for(int j = 0; j < 1; j++){
			Step();
		}
		Quit();
	}

	Init();	
	while(playing) {
		Step();
	}
	Quit();
	return 0;
}


void Init(){
	Sprite::mainID = 0;
	Sprite::subID = 0;

	swiWaitForVBlank();
	srand(time(NULL));
	displayBg = 0;
	
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
	//mmInitDefaultMem((mm_addr)soundbank_bin);
	mm_ds_system sys;
    // number of modules in your soundbank (defined in output header)
    sys.mod_count = MSL_NSONGS;
    // number of samples in your soundbank (defined in output header)
    sys.samp_count= MSL_NSAMPS;
    // memory bank, allocate BANKSIZE (or NSONGS+NSAMPS) words
    soundPtr = sys.mem_bank = (mm_word*)malloc( MSL_BANKSIZE * 4 );
    // select fifo channel
    sys.fifo_channel = FIFO_MAXMOD;
    // initialize maxmod
    mmInit( &sys );
    mmSoundBankInMemory( (mm_addr)soundbank_bin);

	//init background
	setBackdropColor(RGB15(15,15,31));
	setBackdropColorSub(RGB15(15,15,31));

	bgMain = bgInit(3, BgType_Bmp8, BgSize_B8_256x256, 0, 0);
	dmaCopy(bgMainBitmap, bgGetGfxPtr(bgMain), sizeof(bgMainBitmap));

	bgSub = bgInitSub(3, BgType_Bmp8, BgSize_B8_256x256, 0, 0);
	dmaCopy(bgSubBitmap, bgGetGfxPtr(bgSub), sizeof(bgSubBitmap));

	bgSequence.push_back(bgSubBitmap);
	bgSequence.push_back(bgSub1Bitmap);
	bgSequence.push_back(bgSubBitmap);
	bgSequence.push_back(bgSub2Bitmap);


	//load the palettes (background)
	dmaCopy((u16*)bgMainPal, BG_PALETTE, sizeof(bgMainPal));
	dmaCopy((u16*)bgSubPal, BG_PALETTE_SUB, sizeof(bgSubPal));
	bgPalettesSequence.push_back((u16*)bgSubPal);
	bgPalettesSequence.push_back((u16*)bgSub1Pal);
	bgPalettesSequence.push_back((u16*)bgSubPal);
	bgPalettesSequence.push_back((u16*)bgSub2Pal);

	//load the palettes (sprites)
	dmaCopy((u8*)blockPal, SPRITE_PALETTE, sizeof(blockPal));
	dmaCopy((u8*)blockPal, SPRITE_PALETTE_SUB, sizeof(blockPal));
	dmaCopy((u8*)bigImagesPal, SPRITE_PALETTE + 10, sizeof(bigImagesPal));
	dmaCopy((u8*)bigImagesPal, SPRITE_PALETTE_SUB + 10, sizeof(bigImagesPal));

	//load sfx
	//mmLoadEffect(SFX_HIT);
	//mmLoadEffect(SFX_KILL);
	mmLoadEffect(SFX_BG);
	//mmLoadEffect(SFX_LOSE);

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

	//Gameover display
	int xp = 60;
	int yp = 50;
	gameover1 = BigSprite(1, xp, yp, true);
	gameover1.hide = true;
	gameover1.priority = 0;
	gameover2 = BigSprite(2, xp+64, yp, true);
	gameover2.hide = true;
	gameover2.priority = 0;

	//Init the bg music
	mm = MusicManager();

	playing = true;
}

void Step(){
	scanKeys();

	int held = keysHeld();
	int down = keysDown();

	if(held & KEY_RIGHT){
		avatar.MoveRight();
	}
	else if(held & KEY_LEFT){
		avatar.MoveLeft();
	}
	else if(down & KEY_A){
		ball.ChangeState(1);
		if(gameOver){
			Restart();
		}
	}

	if(held & KEY_START){
		playing = false;
	}

	//update the background music
	mm.Update();

	//load the palettes (background)
	dmaCopy((u16*)bgMainPal, BG_PALETTE, sizeof(bgMainPal));
	//animated background (in sub screen)
	if(displayBg == 0 || displayBg == 2){
		dmaCopy(bgSubBitmap, bgGetGfxPtr(bgSub), sizeof(bgSubBitmap));
		dmaCopy((u16*)bgSubPal, BG_PALETTE_SUB, sizeof(bgSubPal));
	}
	else if(displayBg == 1){
		dmaCopy(bgSub1Bitmap, bgGetGfxPtr(bgSub), sizeof(bgSub1Bitmap));
		dmaCopy((u16*)bgSub1Pal, BG_PALETTE_SUB, sizeof(bgSub1Pal));
	}
	else if(displayBg == 3){
		dmaCopy(bgSub2Bitmap, bgGetGfxPtr(bgSub), sizeof(bgSub2Bitmap));
		dmaCopy((u16*)bgSub2Pal, BG_PALETTE_SUB, sizeof(bgSub2Pal));
	}

	//Change background every an arbitrary number of frames
	if(displayBgFrameCounter > 20){
		displayBg++;
		displayBg %= 4;
		displayBgFrameCounter = 0;
	}
	displayBgFrameCounter++;

	if(!gameOver){
		//Draw the blocks
		for(unsigned int i = 0; i < blocks.size(); i++){
			blocks[i].Draw();
		}

		//Draw the avatar
		avatar.Draw();

		//Ball
		ball.Update();
		ball.Draw();

		//UI
		UI.Draw();

		//Check end game condition
		if(UI.hp <= 0 || CheckWinCondition()){
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

void Quit(){
	playing = false;

	mmEffectCancelAll();

	//mmUnloadEffect(SFX_HIT);
	//mmUnloadEffect(SFX_KILL);
	mmUnloadEffect(SFX_BG);
	//mmUnloadEffect(SFX_LOSE);

	free(soundPtr);

	avatar.Deallocate();
	ball.Deallocate();
	UI.Deallocate();
	bgsp.Deallocate();
	gameover1.Deallocate();
	gameover2.Deallocate();

	for(unsigned int i = 0; i < blocks.size(); i++){
		blocks[i].Deallocate();
	}
	blocks.clear();

	bgSequence.clear();
	bgPalettesSequence.clear();

	DC_FlushAll();
	
}

bool CheckWinCondition(){
	for(unsigned int i = 0; i<blocks.size(); i++){
		if(!blocks[i].killed){
			return false;
		}
	}
	return true;
}

void Gameover(){
	gameOver = true;
	mmEffect(SFX_LOSE);
	for(unsigned int i = 0; i < blocks.size(); i++){
		blocks[i].Kill();
		//Force redraw to hide them
		blocks[i].Draw();
	}

	gameover1.hide = false;
	gameover1.PlaceSprite();
	gameover2.hide = false;
	gameover2.PlaceSprite();
}

void Restart(){
	gameOver = false;
	for(unsigned int i = 0; i < blocks.size(); i++){
		blocks[i].Reset();
	}
	UI.hp = UI.GetMaxHP();
	ball.ChangeState(0);

	gameover1.hide = true;
	gameover1.PlaceSprite();
	gameover2.hide = true;
	gameover2.PlaceSprite();
}