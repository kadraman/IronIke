#include "Banks/SetAutoBank.h"

#include "ZGBMain.h"
#include "Scroll.h"
#include "SpriteManager.h"
#include "Print.h"
#include "Sound.h"

IMPORT_TILES(font);
IMPORT_MAP(map);
IMPORT_MAP(hud);

// 50 - spikes
UINT8 collision_tiles[] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 50, 0};

UINT8 level;

Sprite* attack_particle;

void START() {

	scroll_target = SpriteManagerAdd(SpritePlayer, 50, 80);
	InitScroll(BANK(map), &map, collision_tiles, 0);
	INIT_HUD(hud);

	NR52_REG = 0x80; //Enables sound, you should always setup this first
	NR51_REG = 0xFF; //Enables all channels (left and right)
	NR50_REG = 0x77; //Max volume
}

void UPDATE() {
}

