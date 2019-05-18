#pragma once

#include "nds.h"
#include "BigSprite.hpp"

class BackgroundSpritesManager{

    BigSprite cloud, cloud2, cloud3;
    std::pair<int, int> cloudPos = {SCREEN_WIDTH, (int)std::round(rand()%80) + 10 };
    std::pair<int, int> cloud2Pos = {(int)(rand()%SCREEN_WIDTH), (int)(rand()%80) + 20 };
    std::pair<int, int> cloud3Pos = {(int)(rand()%SCREEN_WIDTH), (int)(rand()%80) + 20 };

    int speed = 1;

    int cloud2FrameDelay = 3;
    int cloud2FrameCounter = 0;
    int cloud3FrameDelay = 9;
    int cloud3FrameCounter = 0;
    
public:

    BackgroundSpritesManager(){
        cloud = BigSprite(0, cloudPos.first, cloudPos.second, true);
        cloud2 = BigSprite(0, cloud2Pos.first, cloud2Pos.second, true);
        cloud3 = BigSprite(0, cloud3Pos.first, cloud3Pos.second, true);
    }

    void Update(){
        //Cloud 1
        cloudPos.first -= speed;
        //Cloud 2
        if(cloud2FrameCounter > cloud2FrameDelay){
            cloud2Pos.first -= speed;
            cloud2FrameCounter = 0;
        }
        if(cloud3FrameCounter > cloud3FrameDelay){
            cloud3Pos.first -= speed;
            cloud3FrameCounter = 0;
        }

        cloud2FrameCounter++;
        cloud3FrameCounter++;
    }

    void Draw(){
        cloud.PlaceSprite(cloudPos.first, cloudPos.second);
        cloud2.PlaceSprite(cloud2Pos.first, cloud2Pos.second);
        cloud3.PlaceSprite(cloud3Pos.first, cloud3Pos.second);
    }
};