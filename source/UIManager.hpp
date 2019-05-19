#pragma once

#include "Sprite.hpp"

class UIManager{
    static const int maxHP = 2;
    Sprite boxes[maxHP];
    Sprite hpSprites[maxHP];
    static const int nameTileLength = 16;
    Sprite name[nameTileLength];
    
    public:
    
    int hp = maxHP;

    UIManager(){
        Start();
    }

    void Start(){
        std::pair<int,int> nameInitialPos = {150, 160};

        for(int i = 0; i < maxHP; i++){
            boxes[i] = Sprite(3, 10 + i*12, SCREEN_HEIGHT - 15, false);
            hpSprites[i] = Sprite(1, 12 + i*12, SCREEN_HEIGHT - 13, false);
        }

        for(int row = 0; row < 2; row++){
            for(int col = 0; col < 8; col++){
                name[row*col + col] = Sprite(4 + row*col+col, nameInitialPos.first +8*col, nameInitialPos.second + 8*row, false);
            }
        }
    }

    void Draw(){
        //NAME DISPLAY
        for(int i = 0; i<nameTileLength; i++){
            name[i].PlaceSprite();
        }

        //HP DISPLAY
        for(int i = 0; i < maxHP; i++){
            boxes[i].PlaceSprite();
            if(i >= hp){
                hpSprites[i].hide = true;
            }
            else {
                hpSprites[i].hide = false;
            }
             hpSprites[i].PlaceSprite();
        }
    }

    int GetMaxHP(){
        return maxHP;
    }
};