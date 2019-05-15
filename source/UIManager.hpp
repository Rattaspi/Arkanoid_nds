#pragma once

#include "Sprite.hpp"

class UIManager{
    static const int maxHP = 2;
    Sprite boxes[maxHP];
    Sprite hpSprites[maxHP];
    
    public:
    
    int hp = maxHP;

    UIManager(){
        Start();
    }

    void Start(){
        for(int i = 0; i < maxHP; i++){
            boxes[i] = Sprite(3, 10 + i*12, SCREEN_HEIGHT - 15, false);
            hpSprites[i] = Sprite(1, 12 + i*12, SCREEN_HEIGHT - 13, false);
        }
    }

    void Draw(){
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