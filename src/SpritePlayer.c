#include "Banks/SetAutoBank.h"
#include "main.h"

#include "gb/gb.h"

#include "ZGBMain.h"
#include "Sprite.h"
#include "Keys.h"
#include "Scroll.h"
#include "SpriteManager.h"
#include "Print.h"


#define DEBUG_CONTROLS

const UINT8 anim_idle[] = {5, 0, 1, 2, 3, 4};				// The first number indicates the number of frames
const UINT8 anim_walk[] = {8, 5, 6, 7, 8, 9, 10, 11, 12};
const UINT8 anim_jump[] = {3, 13, 14, 15};
const UINT8 anim_fall[] = {3, 16, 17, 18};
const UINT8 anim_attack[] = { 3, 19, 20, 211 };

typedef enum {
	OEBBI_STATE_NORMAL,
	OEBBI_STATE_JUMPING,
	OEBBI_STATE_FALLING,
	OEBBI_STATE_ATTACK
} OEBBI_STATE;
OEBBI_STATE oebbi_state;
INT16 oebbi_accel_y;

Sprite* attack_sprite;

extern Sprite* game_over_particle;

extern UINT16 reset_x;
extern UINT16 reset_y;
extern UINT8 level;
// 

void START() {
	SetSpriteAnim(THIS, anim_idle, 3u);

	oebbi_accel_y = 0;

	scroll_target = THIS;

	oebbi_state = OEBBI_STATE_NORMAL;

	attack_sprite = 0;
}

void Die(Sprite* sprite, UINT8 idx) {
	SpriteManagerRemove(idx);
	game_over_particle = SpriteManagerAdd(SpriteParticle, sprite->x, sprite->y);
	//scroll_target = 0;
	// if lives < x
	reset_x = 32;
	reset_y = 112;
	SpriteManagerAdd(idx, reset_x, reset_y);
}

UINT8 tile_collision;
void CheckCollisionTile(Sprite* sprite, UINT8 idx) {
	DPRINT_POS(0, 1);
	DPrintf("%d  ", tile_collision);
	if (tile_collision == 50u) { // spikes
		Die(sprite, idx);
	}
	else if (tile_collision == 53u) { // flag/treasure/crown
		if (level == 1) {
			//SetState(StateWin);
		}
		else {
			//level++;
			//reset_x = 32;
			//reset_y = 112;
			SetState(StateGame);
		}
	}
}

void MoveOebbi(Sprite* sprite, UINT8 idx) {
	if (KEY_PRESSED(J_RIGHT)) {
		tile_collision = TranslateSprite(sprite, 1 << delta_time, 0);
		THIS->mirror = NO_MIRROR;
		CheckCollisionTile(sprite, idx);
	}
	else if (KEY_PRESSED(J_LEFT)) {
		tile_collision = TranslateSprite(sprite, -1 << delta_time, 0);
		THIS->mirror = V_MIRROR;
		CheckCollisionTile(sprite, idx);
	}
	else if (KEY_PRESSED(J_UP)) {
		//tile_collision = TranslateSprite(sprite, 0, -1 << delta_time);
		//CheckCollisionTile(sprite, idx);
	}
	else if (KEY_PRESSED(J_DOWN)) {
		//tile_collision = TranslateSprite(sprite, 0, 1 << delta_time);
		//CheckCollisionTile(sprite, idx);
	}
}

void UpdateAttackPos() {
	attack_sprite->mirror = THIS->mirror;
	if (THIS->mirror == V_MIRROR)
		attack_sprite->x = THIS->x - 16u;
	else
		attack_sprite->x = THIS->x + 16u;
	attack_sprite->y = THIS->y;
}

void UPDATE() {
	UINT8 i;
	Sprite* spr;

	//DPRINT_POS(0, 0);
	//DPrintf("x:%d y:%d  ", THIS->x, THIS->y);
	//DPRINT_POS(0, 1);
	//DPrintf("%d  ", oebbi_accel_y);

	switch (oebbi_state) {
		case OEBBI_STATE_NORMAL:
			MoveOebbi(THIS, THIS_IDX);

			//Choose idle anim or walk
			if (KEY_PRESSED(J_RIGHT) || KEY_PRESSED(J_LEFT)) {
				SetSpriteAnim(THIS, anim_walk, 15u);
			}
			else {
				SetSpriteAnim(THIS, anim_idle, 15u);
			}

			//Check jumping
			if (KEY_TICKED(J_A)) {
				oebbi_accel_y = -50;
				oebbi_state = OEBBI_STATE_JUMPING;
			}

			//

			//Check falling
			if ((oebbi_accel_y >> 4) > 1) {
				oebbi_state = OEBBI_STATE_FALLING;
			}

			break;

		case OEBBI_STATE_JUMPING:
			SetSpriteAnim(THIS, anim_jump, 15u);
			if (THIS->anim_frame == 2) {
				oebbi_state = OEBBI_STATE_NORMAL;
			}
			else {
				//SetFrame(THIS, 13);
				MoveOebbi(THIS, THIS_IDX);
			}
			break;

		case OEBBI_STATE_FALLING:
			//SetSpriteAnim(THIS, anim_fall, 15u);
			//if (THIS->anim_frame == 3) {
			//	oebbi_state = OEBBI_STATE_NORMAL;
			//}
			//else {
				//SetFrame(THIS, 13);
			//	MoveOebbi(THIS, THIS_IDX);
			//}
			break;

		case OEBBI_STATE_ATTACK:
			if (THIS->anim_frame == 2) {
				oebbi_state = OEBBI_STATE_NORMAL;
				SpriteManagerRemoveSprite(attack_sprite);
			}
			else {
				MoveOebbi(THIS, THIS_IDX);
				UpdateAttackPos();
			}
			break;
	}

	//Simple gravity physics 
	if (oebbi_accel_y < 40) {
		oebbi_accel_y += 2;
	}
	tile_collision = TranslateSprite(THIS, 0, oebbi_accel_y >> 4);
	if (!tile_collision && delta_time != 0 && oebbi_accel_y < 40) { //Do another iteration if there is no collision
		oebbi_accel_y += 2;
		tile_collision = TranslateSprite(THIS, 0, oebbi_accel_y >> 4);
	}
	if (tile_collision) {
		oebbi_accel_y = 0;
		if (oebbi_state == OEBBI_STATE_JUMPING || oebbi_state == OEBBI_STATE_FALLING) {
			oebbi_state = OEBBI_STATE_NORMAL;
		}

		CheckCollisionTile(THIS, THIS_IDX);
	}

	//Check enemy collision
	for (i = 0u; i != sprite_manager_updatables[0]; ++i) {
		spr = sprite_manager_sprites[sprite_manager_updatables[i + 1u]];
		if (spr->type == SpriteOcti) { // || spr->type == SpriteAznar) {
			if (CheckCollision(THIS, spr)) {
				Die(THIS, THIS_IDX);
			}
		}
		else if (spr->type == SpriteFlag) {
			if (CheckCollision(THIS, spr)) {
				reset_x = spr->x;
				reset_y = spr->y;
			}
		}
	}

	if (KEY_TICKED(J_B) && oebbi_state != OEBBI_STATE_ATTACK) {
		SetSpriteAnim(THIS, anim_attack, 15u);
		oebbi_state = OEBBI_STATE_ATTACK;

		attack_sprite = SpriteManagerAdd(SpriteAttack, THIS->x, THIS->y);
		UpdateAttackPos();
	}

	if (keys == 0 && (!(oebbi_state == OEBBI_STATE_JUMPING || oebbi_state == OEBBI_STATE_FALLING || oebbi_state == OEBBI_STATE_ATTACK))) {
		SetSpriteAnim(THIS, anim_idle, 15u);
		oebbi_state = OEBBI_STATE_NORMAL;
	} 
}

void DESTROY() {
}




