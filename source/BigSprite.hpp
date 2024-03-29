#pragma once

#include <nds.h>
#include "Sprite.hpp"

#include "bigImages.h"

#define BIG_SPRITE_WIDTH 64
#define BIG_SPRITE_HEIGHT 64

class BigSprite {
    private:
    bool alocated;
    int id;

    public:
    OamState* oam;
    
    int imageToUse; //position in the spritesheet
    u16* gfx; //loaded graphic

    uint16 x;
    uint16 y;
    int width, height;

    int priority = 1;
    int palette = 0;
    static const SpriteSize size = SpriteSize_64x64;
    static const SpriteColorFormat colorFormat = SpriteColorFormat_256Color;
    int rotation = -1; //between [0,31]
    bool rotDoubleSize = false;
    bool hide = false;
    bool hflip = false;
    bool vflip = false;
    bool mosaic = false;

    BigSprite(){}
    BigSprite(int imageToUse, int x, int y, bool topScreen){
        this->imageToUse = imageToUse;
        this->x = x;
        this->y = y;

        if(topScreen){
            oam = &oamMain;
            id = Sprite::mainID;
            Sprite::mainID++;
        }
        else {
            oam = &oamSub;
            id = Sprite::subID;
            Sprite::subID++;
        }

        AllocateImage();
    }

    void AllocateImage(){
        gfx = oamAllocateGfx(oam, size, colorFormat);
        
        alocated = true;

        u16* offset = (u16*)bigImagesTiles + ((imageToUse * BIG_SPRITE_WIDTH*BIG_SPRITE_HEIGHT)>>1);
        for(int i = 0; i<BIG_SPRITE_WIDTH*BIG_SPRITE_HEIGHT; i++){
            gfx[i] = offset[i]+(0 | ((offset[i]>>8?10:0)<<8) | (offset[i]&0xFF?10:0));
        }
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

    void Deallocate(){
        oamFreeGfx(oam, gfx);
        DC_FlushRange(gfx,bigImagesTilesLen);

    }
};