#include "ZGBMain.h"
#include "Math.h"

UINT16 g_player_score = 0;
UINT8 next_state = StateGame;

UINT8 GetTileReplacement(UINT8* tile_ptr, UINT8* tile) {
	if (current_state == StateLevel1 || current_state == StateLevel2) {
		*tile = 0;
		switch (*tile_ptr) {
			case 101: 	return SpriteEnemy1;
			case 102:	return SpriteEnemy2;
			case 51: 	return SpriteCoin;
			case 52:	return SpriteAmmo;
			case 125: 	return SpriteFlag;
		}
		*tile = *tile_ptr;
	}
	return 255u;
}