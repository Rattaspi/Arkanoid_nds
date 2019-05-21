#pragma once
#include <cmath>

#include "Sprite.hpp"
#include "UIManager.hpp"
#include "soundbank.h"

#define BALL_WIDTH 4
#define BALL_HEIGHT 4


class Ball {
    private:
    /*
     * 0-> follow the avatar
     * 1-> normal update
     */
    int state = 0;

    int handleHit = -1;
    int handleKill = -1;

    bool topScreen;

    Sprite topSprite;
    Sprite botSprite;

    std::pair<int,int> position;
    std::pair<int, int> speed;

    public:
    Avatar* avatar;
    std::vector<Block>* blockList;
    UIManager* UI;

    Ball(){}

    Ball(int x, int y){
        position.first = x;
        position.second = y;

        topSprite = Sprite(1, x, y - SCREEN_HEIGHT, true);
        topSprite.hide = true;
        botSprite = Sprite(1, x, y, false);
        topScreen = false;

        speed.first = std::round(rand()%6 - 3);
        speed.second = -2 - std::round(rand()%3);
        if(speed.first == 0){
            speed.first = 1;
        }
        speed.first = 0;
    }

    void Update(){
        switch(state) {
            case 0:
                position.first = avatar->position.first + (avatar->GetAvatarLength()/2);
                position.second = avatar->position.second - 8;
                break;

            case 1:
                //NORMAL UPDATE
                position.first += speed.first;
                position.second += speed.second;

                ScreenManagement();
                UpdateCollision();

                //Manage die
                if(!topScreen && position.second > SCREEN_HEIGHT){
                    state = 0;
                    UI->hp = UI->hp - 1;
                    if(handleKill != -1){
                        mmEffectRelease(handleKill);
                    }
                    handleKill = mmEffect(SFX_KILL);
                }
                break;

            default:
                break;
        }
    }

    void Draw(){
        topSprite.PlaceSprite(position.first, position.second);
        botSprite.PlaceSprite(position.first, position.second);
    }

    void ChangeState(int state){
        this->state = state;
    }

    void ScreenManagement(){
        if(!topScreen){ //UPDATING IN SUB SCREEN
            if(speed.second < 0 && position.second < 0){
                topSprite.hide = false;
                botSprite.hide = true;
                position.second = SCREEN_HEIGHT;
                topScreen = true;
            }
        }
        else { //UPDATING IN MAIN SCENE
            if(speed.second > 0 && position.second >= SCREEN_HEIGHT){
                topSprite.hide = true;
                botSprite.hide = false;
                position.second = 0;
                topScreen = false;
            }
        }
    }

    void UpdateCollision(){
        //Horizontal collision (screen)
        if(position.first < 0 || position.first+4 > SCREEN_WIDTH){
            speed.first *= -1;
            if(handleHit != -1){
                mmEffectRelease(handleHit);
            }
            handleHit = mmEffect(SFX_HIT);
        }
        //Top collision (screen)
        if(topScreen && position.second < 0){
            speed.second *= -1;
             if(handleHit != -1){
                mmEffectRelease(handleHit);
            }
            handleHit = mmEffect(SFX_HIT);
        }

        //Object collision
        if(!topScreen){
            //Collision with avatar
            if(position.first < avatar->position.first + avatar->GetAvatarLength() &&
                position.first + 4 > avatar->position.first &&
                position.second < avatar->position.second + 8 &&
                position.second + 4 > avatar->position.second){
                
                speed.second = std::abs(speed.second) * -1;
                if(handleHit != -1){
                    mmEffectRelease(handleHit);
                }
                handleHit = mmEffect(SFX_HIT);
            }
        }
        else {
            for(unsigned int i = 0; i < blockList->size(); i++){
                Block& b = (*blockList)[i];
                if(!b.killed){
                    if(position.first < b.position.first + b.GetWidth()+2 &&
                        position.first + 4 > b.position.first &&
                        position.second < b.position.second + 10 &&
                        position.second + 4 > b.position.second){
                        
                        b.Kill();
                        if(handleHit != -1){
                            mmEffectRelease(handleHit);
                        }
                        handleHit = mmEffect(SFX_HIT);

                        std::pair<int,int> blockCenter = {b.position.first + b.GetWidth(), position.second + 4};

                        if(std::abs(blockCenter.first - position.first) <= b.GetWidth()/2){
                            speed.first *= -1;
                        }
                        else{
                            speed.second *= -1;
                        }   
                    }
                }
            }
        }
    }

    void Reset(){
        topSprite.hide = true;
        botSprite.hide = false;
    }

    void Deallocate(){
        topSprite.Deallocate();
        botSprite.Deallocate();
    }
};