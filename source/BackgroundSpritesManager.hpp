#pragma once

#include "nds.h"
#include "BigSprite.hpp"

class BackgroundSpritesManager{

    BigSprite cloud;
    std::pair<int, int> cloudPos = {SCREEN_WIDTH, (int)std::round(rand()%40) + 20 };

    int speed = 1;
    
public:

    BackgroundSpritesManager(){
        cloud = BigSprite(0, cloudPos.first, cloudPos.second, true);
    }

    void Update(){
        cloudPos.first -= speed;
    }

    void Draw(){
        cloud.PlaceSprite(cloudPos.first, cloudPos.second);
    }
};