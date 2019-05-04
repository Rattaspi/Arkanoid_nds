#pragma once

#include <stdio.h>
#include <vector>
#include "Sprite.hpp"

class Avatar{
private:
    std::vector<Sprite> blocks;
    std::pair<int,int> position;

    int speed = 2;

public:

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
        position.first += speed;
    }

    void MoveLeft(){
        position.first -= speed;
    }

};