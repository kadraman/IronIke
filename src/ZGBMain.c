#include "ZGBMain.h"
#include "Math.h"
#include "GlobalVars.h"

UINT16 g_player_score = 0;

#ifdef NDEBUG
UINT8 next_state = StateSplash;
#else
UINT8 next_state = StateGame;
#endif

UINT8 GetTileReplacement(UINT8* tile_ptr, UINT8* tile) {
	if (current_state == StateLevel1 || current_state == StateLevel2) {
		*tile = 0;
		switch (*tile_ptr) {
			case 251: *tile = 20; return 255u;
			case 252: *tile = 20; return SpriteFlag;
			case 253: *tile = 20; return SpriteCoin;
			case 254: *tile = 20; return SpriteEnemy1;
			case 255: *tile = 20; return SpriteSpirit;
			default: break;
		}
		*tile = *tile_ptr;
	}
	return 255u;
}

/*UINT8 GetTileReplacement(UINT8* tile_ptr, UINT8* tile) {
	if(current_state == StateLevel1 || current_state == StateLevel2) {
		if((0xffu - *tile_ptr) < N_SPRITE_TYPES) {
			*tile = 0;
			return 0xffu - *tile_ptr;
		}
		*tile = *tile_ptr;
	}

	return 0xffu;
}*/