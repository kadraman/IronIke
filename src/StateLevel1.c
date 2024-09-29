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

// 50 - spikes
// 52 - flag
UINT8 collision_tiles_l1[] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 50, 52, 0};

static ItemPos level_items [] = 
{
	{1, 1},
	{1, 2},
	{1, 3},
	{1, 4},
	{4, 4}
};
static UINT8 level_item_pos;

#define NUM_ITEMS (sizeof (level_items) / sizeof(level_items[0]))

static void AddItem(UINT16 x, UINT16 y) {
	Sprite *s = SpriteManagerAdd(SpriteJewell1, (x > 0 ? x*8 : 0), (y > 0 ? y*8 : 0));
	//s->lim_x = 400;
	//s->lim_y = 300;
}
static void AddItems()
{
	UINT16 xpos = player_sprite->x;
	UINT8 x = xpos >> 3;

	if (player_sprite->x > 60000) { 
		x = 0;
	}

	ItemPos* ptr = level_items;
	//ItemPos* endPtr = level_items + sizeof(level_items)/sizeof(level_items[0]);
	for (UINT8 i = 0; i < NUM_ITEMS; i++, ptr++) {
	//while ( ptr < endPtr)  {
		//AddItem(level_items[i].x, level_items[i].y);
		AddItem(ptr->x, ptr->y);
	}
	//while ((level_item_pos < NUM_ITEMS) && (level_items[level_item_pos].x < x + 25))
	//{
	//	AddItem(level_items[level_item_pos].x, level_items[level_item_pos].y);
	//	level_item_pos++;
	//}
}

void START() {

	level_item_pos = 0;
	g_player_region = 0;
	//AddItems();
	scroll_target = SpriteManagerAdd(SpritePlayer, 20, 80);
	InitScroll(BANK(level1), &level1, collision_tiles_l1, 0);
	scroll_bottom_movement_limit = 120;

	Hud_Init();

	NR52_REG = 0x80; //Enables sound, you should always setup this first
	NR51_REG = 0xFF; //Enables all channels (left and right)
	NR50_REG = 0x77; //Max volume

}

void UPDATE() {
	Hud_Update();

	UINT16 xpos = player_sprite->x;
	UINT8 x = xpos >> 3; // tile

	// map is 128 (0-127) x 18 (0-17) tiles
	// region is 32 tiles -> 4 regions
	// 1 - 0-31		0-250
	// 2 - 32-63	251-500
	// 3 - 64-95	501-750
	// 4 - 96-127	751-1000
	if (player_sprite->x > 0 && player_sprite->x <= 250 && g_player_region <= 0) {
		SpriteManagerAdd(SpriteJewell1, 11*8, 12*8);
		g_player_region = 1;
	} else if (player_sprite->x > 251 && player_sprite->x <= 500 && g_player_region <= 1) {
		SpriteManagerAdd(SpriteJewell1, 60*8, 3*8);
		g_player_region = 2;
	} else if (player_sprite->x > 501 && player_sprite->x <= 750 && g_player_region <= 2) {
		SpriteManagerAdd(SpriteJewell1, 93*8, 2*8);
		g_player_region = 3;
	} else if (player_sprite->x > 751 && player_sprite->x <= 1000 && g_player_region <= 3) {
		SpriteManagerAdd(SpriteJewell1, 106*8, 4*8);
		g_player_region = 4;
	}


}

