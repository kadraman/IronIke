#include "ZGBMain.h"
#include "Math.h"

UINT8 next_state = StateGame;

UINT8 GetTileReplacement(UINT8* tile_ptr, UINT8* tile) {
	if (current_state == StateGame) {
		*tile = 0;
		switch (*tile_ptr) {
			case 101: return SpriteOcti;
			case 109: return SpriteFlag;
		}
		*tile = *tile_ptr;
	}
	return 255u;
}