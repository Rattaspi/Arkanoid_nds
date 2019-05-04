#pragma once

#include <nds.h>
#include "Sprite.hpp"

class Block {
    private:
    Sprite blocks[2];

    public:

    Block(){}

    Block (int imageToUse, int x, int y){
        //b = Sprite(&oamMain, imageToUse, x, y, true);
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
};