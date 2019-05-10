#pragma once


#include "Sprite.hpp"
#include <cmath>

class Ball {
    private:
    /*
     * 0-> follow the avatar
     * 1-> normal update
     */
    int state = 0;

    bool topScreen;

    Sprite topSprite;
    Sprite botSprite;

    std::pair<int,int> position;
    std::pair<int, int> speed;

    public:
    Avatar* avatar;
    std::vector<Block>* blockList;

    Ball(){}

    Ball(int x, int y){
        position.first = x;
        position.second = y;

        topSprite = Sprite(1, x, y - SCREEN_HEIGHT, true);
        topSprite.hide = true;
        botSprite = Sprite(1, x, y, false);
        topScreen = false;

        speed.first = 2 + std::round(rand()%4 - 2);
        speed.second = -2 - std::round(-rand()%4);
        //printf("%d, %d, %d",speed.first, speed.second, rand());
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
        //Horizontal collision
        if(position.first < 0 || position.first+4 > SCREEN_WIDTH){
            speed.first *= -1;
        }
        //Top collision
        if(topScreen && position.second < 0){
            speed.second *= -1;
        }

        //Object collision
        if(!topScreen){
            
            // (rect1.x < rect2.x + rect2.width &&
            // rect1.x + rect1.width > rect2.x &&
            // rect1.y < rect2.y + rect2.height &&
            // rect1.height + rect1.y > rect2.y)
            
            //Collision with avatar
            if(position.first < avatar->position.first + avatar->GetAvatarLength() &&
                position.first + 4 > avatar->position.first &&
                position.second < avatar->position.second + 8 &&
                position.second + 4 > avatar->position.second){
                
                speed.second = std::abs(speed.second) * -1;
            }
        }
        else {
            for(unsigned int i = 0; i < blockList->size(); i++){
                Block b = (*blockList)[i];
                if(!b.killed){
                    if(position.first < b.position.first + b.GetWidth() &&
                        position.first + 4 > b.position.first &&
                        position.second < b.position.second + 8 &&
                        position.second + 4 > b.position.second){
                        
                        b.Kill();
                        speed.second *= -1;
                    }
                }
            }
        }
    }
};