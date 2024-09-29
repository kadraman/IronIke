#include "ZGBMain.h"
#include "Math.h"

UINT8 next_state = StateGame;

UINT8 GetTileReplacement(UINT8* tile_ptr, UINT8* tile) {
	if (current_state == StateLevel1 || current_state == StateLevel2) {
		*tile = 0;
		switch (*tile_ptr) {
			case 101: 	return SpriteEnemy1;
			case 102:	return SpriteEnemy2;
			//case 51: 	return SpriteJewell1;
			case 52: 	return SpriteFlag;
		}
		*tile = *tile_ptr;
	}
	return 255u;
}