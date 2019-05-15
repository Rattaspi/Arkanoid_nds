#pragma once

#include <stdio.h>
#include <vector>
#include "Sprite.hpp"

class Avatar{
private:
    std::vector<Sprite> blocks;
    

    int speed = 4;

public:
    std::pair<int,int> position;

    Avatar(){}

    Avatar(int initialLength, int x, int y){
        position = std::pair<int,int>(x, y);
        for(int i = 0; i < initialLength; i++){
            blocks.push_back(Sprite(0, position.first + i*8, position.second, false));
        }
    }

    void Draw(){
        for(unsigned int i = 0; i <  blocks.size(); i++){
            blocks[i].PlaceSprite(position.first + i*8, position.second);
        }
    }

    void MoveRight(){
        if(position.first + GetAvatarLength() < SCREEN_WIDTH){
            position.first += speed;
        }
    }

    void MoveLeft(){
        if(position.first > 0){
            position.first -= speed;
        }
    }

    int GetAvatarLength(){
        return blocks.size()*8;
    }

};