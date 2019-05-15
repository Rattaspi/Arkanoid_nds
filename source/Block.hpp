#pragma once

#include <nds.h>
#include "Sprite.hpp"

class Block {
    private:
    static const int blockWidthInTiles = 2;
    Sprite blocks[blockWidthInTiles];

    public:
    bool killed = false;
    std::pair<int,int> position;

    Block(){}

    Block (int imageToUse, int x, int y){
        //b = Sprite(&oamMain, imageToUse, x, y, true);
        position = {x,y};
        for(unsigned int i = 0; i < (sizeof(blocks)/sizeof(*blocks)); i++){
            blocks[i] = Sprite(imageToUse, x + (i*SPRITE_WIDTH), y, true);
        }
    }

    void Draw(){
        //b.PlaceSprite();
        for(unsigned int i = 0; i <  (sizeof(blocks)/sizeof(*blocks)); i++){
            blocks[i].PlaceSprite();
        }
    }

    int GetWidth(){
        return blockWidthInTiles*8;
    }

    void Kill(){
        killed = true;
        for(unsigned int i = 0; i <  (sizeof(blocks)/sizeof(*blocks)); i++){
            blocks[i].hide = true;
        }
    }

    void Reset(){
        killed = false;
        for(unsigned int i = 0; i <  (sizeof(blocks)/sizeof(*blocks)); i++){
            blocks[i].hide = false;
        }
    }
};