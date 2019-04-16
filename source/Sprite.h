#include <nds.h>

class Sprite{
private:
	bool allocated;
  	u16* gfx; //Oam gfx
  	OamState* oam;
public:
  	int x, y;
  	float dx, dy;
	int rotation = -1;
	SpriteColorFormat format = SpriteColorFormat_256Color;
	SpriteSize size = SpriteSize_32x32;
	bool vFlip = false, hFlip = false, hide = false;
	int priority = 0;

	Sprite(int x, int y, int dX, int dY, OamState* oam) {
        this.x = x;
        this.y = y;
        this.dx = dx;
        this.dy = dy;
        this.oam = oam;			
	}
	Sprite(int posX, int posY, int dX, int dY, OamState* oamState, SpriteSize spriteSize, SpriteColorFormat colorFormat) :
		X(posX), Y(posY), dx(dX), dy(dY), oam(oamState), size(spriteSize), format(colorFormat){
	}

	void AllocateInVram(u8* spritePal,int palLen){
		gfx = oamAllocateGfx(oam, size, format);
		dmaCopy(spritePal, SPRITE_PALETTE, palLen*2);
		allocated = true;
	}

	void DeallocateInVram(){
		oamFreeGfx(oam, gfx);
		allocated = false;
	}

	void Draw(u8* spriteTiles, int tilesLen){
		if(!allocated)
		return;

		dmaCopy(spriteTiles, gfx, tilesLen);

		oamSet(oam,        //main graphics engine context
		0,                    //oam index (0 to 127)
		X, Y,                 //x and y pixel location of the sprite
		priority,             //priority, lower renders last (on top)
		0,					          //this is the palette index if multiple palettes or the alpha value if bmp sprite
		size,                 //size of the sprite
		format,               //color format of the sprite
		gfx,                  //pointer to the loaded graphics
		rotation,             //sprite rotation data
		false,                //double the size when rotating?
		hide,			            //hide the sprite?
		vFlip, hFlip,         //vflip, hflip
		false	                //apply mosaic
		);
	}

	~Sprite(){
		if(allocated)
		DeallocateInVram();
	}
};
