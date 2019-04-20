#include <nds.h>

class Sprite {
    public:
    OamState* oam;
    int id;
    u16* gfx; //loaded graphic

    uint16 x;
    uint16 y;
    int priority;
    //this is the palette index if
    //multiple palettes or the alpha value if bmp sprite
    int palette;
    SpriteSize size;
    SpriteColorFormat colorFormat;
    int rotation; //between [0,31]
    bool rotDoubleSize;
    bool hide;
    bool hflip;
    bool vflip;
    bool mosaic;

    Sprite(OamState* oam,
            int id,
            int x,
            int y,
            int priority,
            int palette,
            SpriteSize size,
            SpriteColorFormat colorFormat,
            u16* gfx,
            bool hide,
            bool hflip,
            bool vflip){
        this->if = id;
        this->x = x;
        this->y = y;
        this->priority = priority;
        this-> palette = palette;
        this->size = size;
        this->colorFormat = colorFormat;
        this->gfx = gfx;
        this->hide = hide;
        this->hflip = hflip;
        this->vflip = vflip;
        
        rotation = -1;
        rotDoubleSize = false;
        mosaic = false;
    }
}