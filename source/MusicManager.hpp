#pragma once

#include "soundbank.h"

#define SONG_DURATION 2050

class MusicManager {
private:
    int songCounter = 0;
public:
    MusicManager(){
        mmEffect(SFX_BG);
    }

    void Update(){
        if(songCounter > SONG_DURATION){
            mmEffect(SFX_BG);
            songCounter = 0;
        }
        songCounter++;
    }
};