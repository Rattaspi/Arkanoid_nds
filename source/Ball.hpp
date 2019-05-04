#pragma once


#include "Sprite.hpp"

class Ball {
    Sprite topSprite;
    Sprite botSprite;

    std::pair<int,int> position;
    int speed = 2;

    public:
    Ball(){}

    Ball(int x, int y){
        position.first = x;
        position.second = y;

        topSprite = Sprite(1, x, y - SCREEN_HEIGHT, true);
        botSprite = Sprite(1, x, y, false);
    }

    void Update(){
        position.second -= speed;
    }

    void Draw(){
        topSprite.PlaceSprite(position.first, position.second - SCREEN_HEIGHT);
        botSprite.PlaceSprite(position.first, position.second);
    }
};