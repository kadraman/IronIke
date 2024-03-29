#include "Banks/SetAutoBank.h"

#include "ZGBMain.h"
#include "Scroll.h"
#include "SpriteManager.h"
#include "Print.h"


IMPORT_TILES(font);
IMPORT_MAP(map);

UINT8 collision_tiles[] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 0};

UINT16 reset_x;
UINT16 reset_y;
UINT8 level;

Sprite* game_over_particle;

void START() {
	INIT_CONSOLE(font, 2);

	scroll_target = SpriteManagerAdd(SpritePlayer, 50, 80);
	InitScroll(BANK(map), &map, collision_tiles, 0);
}

void UPDATE() {
}

