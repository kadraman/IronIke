#include "Banks/SetAutoBank.h"
#include "main.h"

#include <gb/gb.h>

#include "SpriteManager.h"
#include "ZGBMain.h"

const UINT8 anim_punch[] = { 2, 0, 1 };

void START() {
	SetFrame(THIS, 1);
}

void UPDATE() {
	UINT8 i;
	Sprite* spr;
	Sprite* spr2;

	for(i = 0u; i != sprite_manager_updatables[0]; ++i) {
		spr = sprite_manager_sprites[sprite_manager_updatables[i + 1u]];
		if (spr->type == SpriteOcti) {
			if(CheckCollision(THIS, spr)) {
				spr2 = SpriteManagerAdd(SpriteParticle, spr->x, spr->y);

				SpriteManagerRemove(i);
			}
		}
	}
}

void DESTROY() {
}