#pragma once

#include "soundbank.h"

#define SONG_DURATION 2050

class MusicManager {
private:
    int songCounter = 0;
    int handle;
public:
    MusicManager(){
        handle = mmEffect(SFX_BG);
    }

    ~MusicManager(){
        mmEffectRelease(handle);
    }

    void Update(){
        if(songCounter > SONG_DURATION){
            mmEffectRelease(handle);
            handle = mmEffect(SFX_BG);
            songCounter = 0;
        }
        songCounter++;
    }
};