#pragma once

#include <nds.h>

#include "block.h"

#define SPRITE_WIDTH 8
#define SPRITE_HEIGHT 8

class Sprite {
    private:
    bool alocated;
    int id;

    public:
    OamState* oam;
    static int mainID;
    static int subID;
    
    int imageToUse; //position in the spritesheet
    u16* gfx; //loaded graphic

    uint16 x;
    uint16 y;
    int width, height;

    int priority = 0;
    int palette = 0;
    static const SpriteSize size = SpriteSize_8x8;
    static const SpriteColorFormat colorFormat = SpriteColorFormat_256Color;
    int rotation = -1; //between [0,31]
    bool rotDoubleSize = false;
    bool hide = false;
    bool hflip = false;
    bool vflip = false;
    bool mosaic = false;

    Sprite(){}
    Sprite(int imageToUse, int x, int y, bool topScreen){
        this->imageToUse = imageToUse;
        this->x = x;
        this->y = y;

        if(topScreen){
            oam = &oamMain;
            id = mainID;
            mainID++;
        }
        else {
            oam = &oamSub;
            id = subID;
            subID++;
        }

        AllocateImage();
    }

    void AllocateImage(){
        gfx = oamAllocateGfx(oam, size, colorFormat);
        
        alocated = true;

        u8* offset = (u8*)blockTiles + (imageToUse * SPRITE_WIDTH*SPRITE_HEIGHT);
        dmaCopy(offset, gfx, SPRITE_WIDTH*SPRITE_HEIGHT);
    }

    void PlaceSprite(){
        oamSet(oam,
                id,
                x, y,
                priority,
                palette,
                size,
                colorFormat,
                gfx,
                rotation,
                rotDoubleSize,
                hide,
                vflip, hflip,
                mosaic
                );
    }

    void PlaceSprite(int x, int y){
        oamSet(oam,
                id,
                x, y,
                priority,
                palette,
                size,
                colorFormat,
                gfx,
                rotation,
                rotDoubleSize,
                hide,
                vflip, hflip,
                mosaic
                );
    }

    void UpdatePosition(uint16 x, uint16 y){
        this->x = x;
        this->y = y;
    }
};