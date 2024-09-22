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
IMPORT_MAP(level2);
IMPORT_MAP(hud);

// 50 - spikes
// 52 - flag
UINT8 collision_tiles[] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 50, 52, 0};

UINT8 g_level_current = 1;
extern Sprite* player_sprite; 
Sprite* attack_particle;

void START() {

	switch (g_level_current) {
		case 1:
			scroll_target = SpriteManagerAdd(SpritePlayer, 20, 80);
			InitScroll(BANK(level1), &level1, collision_tiles, 0);
			scroll_bottom_movement_limit = 120;
			break;
		case 2:
			scroll_target = SpriteManagerAdd(SpritePlayer, 20, 80);
			InitScroll(BANK(level2), &level2, collision_tiles, 0);
			scroll_bottom_movement_limit = 120;
			break;
		default:
			scroll_target = SpriteManagerAdd(SpritePlayer, 20, 80);
			InitScroll(BANK(level1), &level1, collision_tiles, 0);
			scroll_bottom_movement_limit = 120;
			break;
	}

	Hud_Init();

	NR52_REG = 0x80; //Enables sound, you should always setup this first
	NR51_REG = 0xFF; //Enables all channels (left and right)
	NR50_REG = 0x77; //Max volume

}

void UPDATE() {
	Hud_Update();
}

