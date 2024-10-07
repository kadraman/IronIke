#include "Banks/SetAutoBank.h"

#include "ZGBMain.h"
#include "Scroll.h"
#include "SpriteManager.h"
#include "Print.h"
#include "Sound.h"

#include "GlobalVars.h"
#include "SpritePlayer.h"
#include "Hud.h"

IMPORT_TILES(font);
IMPORT_MAP(level1);
IMPORT_MAP(hud);

extern Sprite* player_sprite;
extern UINT16 levelMaxTime;

// 50 - spikes
// 125 - flag
UINT8 collision_tiles_l1[] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 50, 125, 0};

#define MAX_COLLECTABLES 10
UINT16 collectables_taken[MAX_COLLECTABLES + 1];

void START() {
	memset(collectables_taken, 0, sizeof(collectables_taken));
	scroll_target = SpriteManagerAdd(SpritePlayer, 20, 80);
	InitScroll(BANK(level1), &level1, collision_tiles_l1, 0);
	scroll_bottom_movement_limit = 120;
	levelMaxTime = 10;

	Hud_Init();

	NR52_REG = 0x80; //Enables sound, you should always setup this first
	NR51_REG = 0xFF; //Enables all channels (left and right)
	NR50_REG = 0x77; //Max volume

}

void UPDATE() {
	Hud_Update();
}

/*UINT8 IsCollected(Sprite* collectable) BANKED {
        UINT8 i;
        for (i = 1; i != collectables_taken[0] + 1; ++i) {
			if (collectables_taken[i] == collectable->unique_id)
				return i;
        }
        return 255;
}

void TakeCollectable(Sprite* collectable) BANKED {
        collectables_taken[++ collectables_taken[0]] = collectable->unique_id;
}*/