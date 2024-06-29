#include "Banks/SetAutoBank.h"
#include "main.h"

#include <gb/gb.h>

#include "Scroll.h"
#include "SpriteManager.h"

const UINT8 anim_enemy1_move[] = {2, 0, 1};

void START() {
	SetSpriteAnim(THIS, anim_enemy1_move, 5u);
	THIS->lim_x = 160u;
	THIS->lim_y = 160u;
}

void UPDATE() {
	if(THIS->mirror == V_MIRROR) {
		//moving left
		if(TranslateSprite(THIS, -1, 0)) {
			THIS->mirror = V_MIRROR;
		} else	if(!scroll_collisions[GetScrollTile((THIS->x >> 3), (THIS->y >> 3) + 2u)]) {
			THIS->mirror = NO_MIRROR;
		}
	} else {
		//moving right
		if(TranslateSprite(THIS, +1, 0)) {
			THIS->mirror = V_MIRROR;
		} else if(!scroll_collisions[GetScrollTile(((THIS->x + THIS->coll_w) >> 3), (THIS->y >> 3) + 2u)]) {
			THIS->mirror = V_MIRROR;
		}
	}
}

void DESTROY() {
}