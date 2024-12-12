#include "Banks/SetAutoBank.h"

#include "ZGBMain.h"
#include "Scroll.h"
#include "SpriteManager.h"
#include "Print.h"
#include "Sound.h"
#include "Keys.h"

#include "SpritePlayer.h"
#include "GlobalVars.h"

UINT8 g_level_current = 1;
UINT16 g_level_coins = 0;
UINT16 g_level_spirits = 0;
UINT16 g_level_bullets = 6;
extern UINT16 collectables_taken[];
extern Sprite* player_sprite;
extern UINT16 g_player_score;

//IMPORT_MAP(titlescreen);

void START() {
	//HIDE_HUD;
	//InitScroll(BANK(titlescreen), &titlescreen, 0, 0);
	//gbt_stop();
	//NR52_REG = 0x80; //Enables sound, you should always setup this first
	//NR51_REG = 0xFF; //Enables all channels (left and right)
	//NR50_REG = 0x77; //Max volume
	//PlayMusic(titles, 0);
	SetState(g_level_current+1);
}

void UPDATE() {
	//if (ANY_KEY_PRESSED) {
	//	SetState(g_level_current);
	//}

}

UINT8 IsCollected(Sprite* collectable) BANKED {
	UINT8 i;
	for (i = 1; i != collectables_taken[0] + 1; ++i) {
		if (collectables_taken[i] == collectable->unique_id)
			return i;
	}
	return 255;
}

void TakeCollectable(Sprite* collectable, ItemType itype) BANKED {
	collectables_taken[++ collectables_taken[0]] = collectable->unique_id;
	PlayerData* data = (PlayerData*)player_sprite->custom_data;
	switch (itype) {
		case ITEM_BULLET:
			g_level_bullets++;
			break;
		case ITEM_COIN:
			g_level_coins++;
			break;
		case ITEM_SPIRIT:
			g_level_spirits++;
		default:
			break;
	}
	Hud_Update();
}

