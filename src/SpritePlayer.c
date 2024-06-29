#include "Banks/SetAutoBank.h"
#include "main.h"

#include "gb/gb.h"

#include "ZGBMain.h"
#include "Sprite.h"
#include "Keys.h"
#include "Scroll.h"
#include "SpriteManager.h"
#include "Print.h"
#include "Sound.h"

#include "zgb_utils.h"

//#define DEBUG_CONTROLS
#define MAX_LIVES 3
#define MAX_LEVEL 1

// player animations - the first number indicates the number of frames
const UINT8 anim_idle[] = {5, 0, 1, 2, 3, 4};				
const UINT8 anim_walk[] = {8, 5, 6, 7, 8, 9, 10, 11, 12};
const UINT8 anim_jump[] = {3, 13, 14, 15};
const UINT8 anim_fall[] = {3, 16, 17, 18};
const UINT8 anim_attack[] = {3, 19, 20, 21};
const UINT8 anim_hit[] = {4, 22, 23, 24, 25};

typedef enum {
	OEBBI_STATE_NORMAL,
	OEBBI_STATE_JUMPING,
	OEBBI_STATE_FALLING,
	OEBBI_STATE_ATTACK,
	OEBBI_STATE_HIT
} OEBBI_STATE;
OEBBI_STATE oebbi_state;
INT16 oebbi_accel_y;

Sprite* attack_sprite;

extern Sprite* attack_particle;
extern UINT8 attack_particle_anim_running;
extern UINT8 level;

UINT8 lives;
UINT8 reset_x;
UINT8 reset_y;

void START() {
	SetSpriteAnim(THIS, anim_idle, 3u);
	oebbi_accel_y = 0;
	scroll_target = THIS;
	oebbi_state = OEBBI_STATE_NORMAL;
	attack_sprite = 0;
	lives = MAX_LIVES;
	level = 1;
	reset_x = 50;
	reset_y = 80;
}

void UpdateHudLives() {
	for (UINT8 i = 0; i < MAX_LIVES; ++i)
		UPDATE_HUD_TILE(16 + i, 0, i < lives ? 1 : 2);
}

void Hit(Sprite* sprite, UINT8 idx) {
	if (oebbi_state != OEBBI_STATE_HIT) {
		PlayFx(CHANNEL_1, 10, 0x4f, 0xc7, 0xf3, 0x73, 0x86);
		oebbi_state = OEBBI_STATE_HIT;
		attack_particle = SpriteManagerAdd(SpriteParticle, sprite->x, sprite->y);

		if (--lives == 0) {
			SetState(StateGameOver);
		} else {
			UpdateHudLives();
		}	
	}		
}

UINT8 tile_collision;
void CheckCollisionTile(Sprite* sprite, UINT8 idx) {
	if (tile_collision == 50u) { // spikes
		Hit(sprite, idx);
	}
	else if (tile_collision == 52u) { // flag
		// go to next level or complete game
		if (level == MAX_LEVEL) {
			SetState(StateWin);
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
	// TODO: climb up
	else if (KEY_PRESSED(J_UP)) {
		//tile_collision = TranslateSprite(sprite, 0, -1 << delta_time);
		//CheckCollisionTile(sprite, idx);
	}
	// TODO: climb down
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
	attack_sprite->y = THIS->y + 11u;
}

void UPDATE() {
	UINT8 i;
	Sprite* spr;

	switch (oebbi_state) {
		case OEBBI_STATE_NORMAL:
			MoveOebbi(THIS, THIS_IDX);

			// set walk or idle animation
			if (KEY_PRESSED(J_RIGHT) || KEY_PRESSED(J_LEFT)) {
				SetSpriteAnim(THIS, anim_walk, 15u);
			}
			else {
				SetSpriteAnim(THIS, anim_idle, 15u);
			}

			// pressed A to jump - and not already jumping
			if (KEY_TICKED(J_A) && oebbi_state != OEBBI_STATE_JUMPING) {
				oebbi_accel_y = -50;
				oebbi_state = OEBBI_STATE_JUMPING;
			}

			// check if now falling?
			if ((oebbi_accel_y >> 4) > 1) {
				oebbi_state = OEBBI_STATE_FALLING;
			}

			break;

		case OEBBI_STATE_JUMPING:
			SetSpriteAnim(THIS, anim_jump, 15u);
			MoveOebbi(THIS, THIS_IDX);
			break;

		case OEBBI_STATE_FALLING:
			SetSpriteAnim(THIS, anim_fall, 15u);
			MoveOebbi(THIS, THIS_IDX);
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

		case OEBBI_STATE_HIT:
			SetSpriteAnim(THIS, anim_hit, 10u);
			oebbi_accel_y = 0;
			if (THIS->anim_frame == 3) {
				// move player to start/checkpoint
				attack_sprite = 0;
				THIS->x = reset_x;
				THIS->y = reset_y;
				ScrollRelocateMapTo(0, 0);
				oebbi_state = OEBBI_STATE_NORMAL;
			}
			break;
	}

	// simple gravity physics until we "collide" with something
	if (oebbi_accel_y < 40) {
		oebbi_accel_y += 2;
	}
	tile_collision = TranslateSprite(THIS, 0, oebbi_accel_y >> 4);
	if (!tile_collision && delta_time != 0 && oebbi_accel_y < 40) { 
		//do another iteration if there is no collision
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

	// check enemy collision
	for (i = 0u; i != sprite_manager_updatables[0]; ++i) {
		spr = sprite_manager_sprites[sprite_manager_updatables[i + 1u]];
		if (spr->type == SpriteEnemy1) { // || spr->type == SpriteEnemy2) {
			if (CheckCollision(THIS, spr)) {
				Hit(THIS, THIS_IDX);
			}
		}
		else if (spr->type == SpriteFlag) {
			if (CheckCollision(THIS, spr)) {
				// example save_point?
				//reset_x = spr->x;
				//reset_y = spr->y;
			}
		}
	}

	// pressed B to attack - and not already attacking
	if (KEY_TICKED(J_B) && oebbi_state != OEBBI_STATE_ATTACK) {
		SetSpriteAnim(THIS, anim_attack, 15u);
		oebbi_state = OEBBI_STATE_ATTACK;
		attack_sprite = SpriteManagerAdd(SpritePunch, THIS->x, THIS->y + 11u);
		UpdateAttackPos();
	}

	// nothing happening lets revert to idle state
	if (keys == 0 && (!(oebbi_state == OEBBI_STATE_JUMPING || oebbi_state == OEBBI_STATE_FALLING || oebbi_state == OEBBI_STATE_ATTACK || oebbi_state == OEBBI_STATE_HIT))) {
		SetSpriteAnim(THIS, anim_idle, 15u);
		oebbi_state = OEBBI_STATE_NORMAL;
	} 
}

void DESTROY() {
}




