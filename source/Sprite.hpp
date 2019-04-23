#pragma once

#include <nds.h>

#include "block.h"

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
    //int palette = 1;
    SpriteSize size;
    SpriteColorFormat colorFormat;
    int rotation = -1; //between [0,31]
    bool rotDoubleSize = false;
    bool hide = false;
    bool hflip = false;
    bool vflip = false;
    bool mosaic = false;

    unsigned int* tiles;
    int tilesLen;
    unsigned short* palette;
    int paletteLen;

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
        this->width = width;
        this->height = height;
        this->size = size;
        this->colorFormat = colorFormat;

        // this->tiles = tiles;
        // this->tilesLen = tilesLen;
        // this->palette = palette;
        // this->paletteLen = paletteLen;

        AllocateImage();
        //SPRITE_PALETTE_SUB[1] = RGB15(0,10,1);
    }

    void AllocateImage(){
        gfx = oamAllocateGfx(&oamSub, SpriteSize_8x8, SpriteColorFormat_256Color);
        
        alocated = true;

        dmaCopy(blockTiles, gfx, blockTilesLen);
        dmaCopy(blockPal, gfx, blockPalLen);
    }

    void PlaceSprite(){
        oamSet(oam,
                id,
                x, y,
                priority,
                palette[0],
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