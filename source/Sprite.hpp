#pragma once

#include <nds.h>

class Sprite {
    private:
    bool alocated;
    
    public:
    OamState* oam;
    int id;
    u16* gfx; //loaded graphic

    uint16 x;
    uint16 y;
    int width;
    int height;
    int priority = 3;
    //this is the palette index if
    //multiple palettes or the alpha value if bmp sprite
    int palette = 1;
    SpriteSize size;
    SpriteColorFormat colorFormat;
    int rotation = -1; //between [0,31]
    bool rotDoubleSize = false;
    bool hide = false;
    bool hflip = false;
    bool vflip = false;
    bool mosaic = false;

    Sprite(OamState* oam,
            int id,
            int x,
            int y,
            int width,
            int height,
            SpriteSize size,
            SpriteColorFormat colorFormat
           ){
        this->oam = oam;
        this->id = id;
        this->x = x;
        this->y = y;
        this->size = size;
        this->colorFormat = colorFormat;

        AllocateImage();
        SPRITE_PALETTE_SUB[1] = RGB15(0,10,1);
    }

    void AllocateImage(){
        gfx = oamAllocateGfx(&oamSub, SpriteSize_32x8, SpriteColorFormat_256Color);
        alocated = true;

        for(int i = 0; i < width*height; i++){
            gfx[i] = 1 | (1<<8);
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

    void UpdatePosition(uint16 x, uint16 y){
        this->x = x;
        this->y = y;
    }
};