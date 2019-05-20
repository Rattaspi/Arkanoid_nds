#pragma once

#include "Sprite.hpp"
#include "BigSprite.hpp"

class UIManager{
    static const int maxHP = 2;
    Sprite boxes[maxHP];
    Sprite hpSprites[maxHP];
    static const int nameLength = 2;
    BigSprite name[nameLength];

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

        std::pair<int, int> namePos = {130, 155};
        for (int i = 0; i < nameLength; i++){
            name[i] = BigSprite(3+i, namePos.first + 64*i, namePos.second, false);
            name[i].priority = 0;
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

        for(int i = 0; i < nameLength; i++){
            name[i].PlaceSprite();
        }
    }

    int GetMaxHP(){
        return maxHP;
    }

    void Deallocate(){
        for(int i = 0; i < maxHP; i++){
            hpSprites[i].Deallocate();
        }
        for(int i = 0; i < nameLength; i++){
            name[i].Deallocate();
        }

    }
};