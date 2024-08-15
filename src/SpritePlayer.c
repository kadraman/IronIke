#include "SpritePlayer.h"
#include "Keys.h"
#include "Scroll.h"
#include "Print.h"
#include "Sound.h"
#include "Banks/SetAutoBank.h"
#include "gb/gb.h"


// player animations - the first number indicates the number of frames
const UINT8 anim_idle[] = {4, 0, 1, 2, 3};				
const UINT8 anim_walk[] = {3, 4, 5, 6};
const UINT8 anim_jump[] = {1, 7};
const UINT8 anim_fall[] = {1, 7};
const UINT8 anim_attack1[] = {1, 9};
const UINT8 anim_attack2[] = {1, 8};
const UINT8 anim_climb[] = {3, 11, 12, 13};
const UINT8 anim_hit[] = {1, 14};
const UINT8 anim_dead[] = {1, 14}; // TBD
const UINT8 anim_victory[] = {1, 14}; // TBD

static AnimationState lastState, currentState;

INT16 accel_y;

Sprite* player_sprite;

extern Sprite* attack_particle;
extern UINT8 attack_particle_anim_walkning;

INT8 shoot_cooldown = 0;

UINT8 g_lives;
UINT8 reset_x;
UINT8 reset_y;

static void SetAnimationState(AnimationState state) {
	lastState = currentState;
	currentState = state;
	switch (state) {
		case IDLE:    	SetSpriteAnim(THIS, anim_idle, 10); break;
		case WALK:    	SetSpriteAnim(THIS, anim_walk, WALK_ANIM_SPEED); break;
		case JUMP:    	SetSpriteAnim(THIS, anim_jump, 10); break;
		case FALL:    	SetSpriteAnim(THIS, anim_fall, 10); break;
		case ATTACK:	if (lastState == JUMP) {
							SetSpriteAnim(THIS, anim_attack2, 10);
						} else {
							SetSpriteAnim(THIS, anim_attack1, 10);
						}
						break;
		case HIT:		SetSpriteAnim(THIS, anim_hit, 10); break;
		case DEAD:    	SetSpriteAnim(THIS, anim_hit, 10); break;
		case VICTORY: 	SetSpriteAnim(THIS, anim_victory, 8); break;
	}
}

static void RevertAnimationState(void) {
	SetAnimationState(lastState);
}

static AnimationState GetAnimationState(void) {
	return currentState;
}

void START() {
	player_sprite = THIS;
	SetSpriteAnim(THIS, anim_idle, 3u);
	accel_y = 0;
	scroll_target = THIS;
	lastState = currentState = IDLE;
	SetAnimationState(currentState);
	g_lives = MAX_LIVES;
	//g_jewell_counter = 0;
	reset_x = 20;
	reset_y = 80;
}

void Hit(Sprite* sprite, UINT8 idx) {
	if (currentState != HIT) {
		PlayFx(CHANNEL_1, 10, 0x4f, 0xc7, 0xf3, 0x73, 0x86);
		currentState = HIT;
		attack_particle = SpriteManagerAdd(SpriteParticle, sprite->x, sprite->y);

		if (--g_lives == 0) {
			SetState(StateGameOver);
		} else {
			Hud_Update();
		}	
	}		
}

void Collected(Sprite* sprite, UINT8 idx) {
	PlayFx(CHANNEL_1, 10, 0x4f, 0xc7, 0xf3, 0x73, 0x86);
	SpriteManagerRemoveSprite(sprite);
	g_jewell_counter++;
}

UINT8 tile_collision;
void CheckCollisionTile(Sprite* sprite, UINT8 idx) {
	if (tile_collision == 50u) { // spikes
		Hit(sprite, idx);
	} else if (tile_collision == 51u) { // jewell
		//Collected(sprite, idx);
	} else if (tile_collision == 52u) { // flag
		// go to next level or complete game
		if (g_level_current == MAX_LEVEL) {
			SetState(StateWin);
		} else {
			g_level_current++;
			//reset_x = 32;
			//reset_y = 112;
			SetState(StateGame);
		}
	}
}

void Shoot() {
	//lastState = currentState;
	//if (currentState == JUMP) {
	//	SetSpriteAnim(THIS, anim_attack1, 15u);
	//} else if (currentState == IDLE || currentState == WALK) {
	//	SetSpriteAnim(THIS, anim_attack1, 15u);
	//} else {
	//	SetSpriteAnim(THIS, anim_attack1, 15u);
	//}
	SetAnimationState(ATTACK);
	Sprite* bullet_sprite = SpriteManagerAdd(SpriteBullet, 0, 0);
	bullet_sprite->mirror = THIS->mirror;
	if(THIS->mirror) 
		bullet_sprite->x = THIS->x - 2u;
	else
		bullet_sprite->x = THIS->x + 7u; 
	bullet_sprite->y = THIS->y + 12u;
	shoot_cooldown = 10;
	//if (THIS->anim_frame == 1) {
	//	currentState = lastState;
	//}
}

void HandleInput(Sprite* sprite, UINT8 idx) {
	if (KEY_PRESSED(J_RIGHT)) {
		tile_collision = TranslateSprite(sprite, 1 << delta_time, 0);
		THIS->mirror = NO_MIRROR;
		CheckCollisionTile(sprite, idx);
		if (currentState != JUMP) SetAnimationState(WALK);
	}
	else if (KEY_PRESSED(J_LEFT)) {
		tile_collision = TranslateSprite(sprite, -1 << delta_time, 0);
		THIS->mirror = V_MIRROR;
		CheckCollisionTile(sprite, idx);
		if (currentState != JUMP) SetAnimationState(WALK);
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
	if (KEY_TICKED(J_A) && currentState != JUMP) {
		SetAnimationState(JUMP);
		accel_y = -50;
	} else {
		// check if now FALLING?
		if ((accel_y >> 4) > 1) {
			SetAnimationState(FALL);
		}
	}
	if (currentState != HIT) {
		if (shoot_cooldown) {
			shoot_cooldown -= 1u;
		} else {
			if (KEY_TICKED(J_B)) {
				Shoot();
			}
		}
	}
}

void UPDATE() {
	UINT8 i;
	Sprite* spr;

	if (currentState == ATTACK && shoot_cooldown == 0) {
		SetAnimationState(lastState);
	}
	if (currentState == HIT) {
		accel_y = 0;
		//if (THIS->anim_frame == 0 /*anim_hit[0]*/) {
			// move player to start/checkpoint
			THIS->x = reset_x;
			THIS->y = reset_y;
			g_jewell_counter = 0;
			ScrollRelocateMapTo(0, 0);
			currentState = IDLE;
		//}
	}
	HandleInput(THIS, THIS_IDX);

	// simple gravity physics until we "collide" with something
	if (accel_y < 40) {
		accel_y += 2;
	}
	tile_collision = TranslateSprite(THIS, 0, accel_y >> 4);
	if (!tile_collision && delta_time != 0 && accel_y < 40) { 
		//do another iteration if there is no collision
		accel_y += 2;
		tile_collision = TranslateSprite(THIS, 0, accel_y >> 4);
	}
	if (tile_collision) {
		accel_y = 0;
		if (currentState == JUMP || currentState == FALL) {
			currentState = IDLE;
		}

		CheckCollisionTile(THIS, THIS_IDX);
	}

	// check sprite collision
	for (i = 0u; i != sprite_manager_updatables[0]; ++i) {
		spr = sprite_manager_sprites[sprite_manager_updatables[i + 1u]];
		if (spr->type == SpriteEnemy1) { // || spr->type == SpriteEnemy2) {
			if (CheckCollision(THIS, spr)) {
				Hit(THIS, THIS_IDX);
			}
		} else if (spr->type == SpriteJewell1) {
			if (CheckCollision(THIS, spr)) {
				Collected(spr, i);
			}
		} else if (spr->type == SpriteFlag) {
			if (CheckCollision(THIS, spr)) {
				// example save_point?
				//reset_x = spr->x;
				//reset_y = spr->y;
			}
		}
	}

	// nothing happening lets revert to idle state
	if (keys == 0 && (!(currentState == JUMP || currentState == FALL || currentState == ATTACK || currentState == HIT))) {
		//SetSpriteAnim(THIS, anim_idle, 15u);
		//currentState = NORMAL;
		SetAnimationState(IDLE);
	} 


}

void DESTROY() {
}




