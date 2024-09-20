#include "SpritePlayer.h"
#include "Keys.h"
#include "Scroll.h"
#include "Print.h"
#include "Sound.h"
#include "Banks/SetAutoBank.h"
#include "gb/gb.h"


// player animations - the first number indicates the number of frames
const UINT8 anim_idle[] = {4, 0, 1, 2, 3};		
const UINT8 anim_idle_shoot[] = {1, 4};		
const UINT8 anim_walk[] = {3, 5, 6, 7};
const UINT8 anim_walk_shoot[] = {1, 8};
const UINT8 anim_jump[] = {1, 9};
const UINT8 anim_fall[] = {1, 9};
const UINT8 anim_jump_shoot[] = {1, 10};
const UINT8 anim_climb[] = {2, 12, 13};
const UINT8 anim_climb_idle[] = {1, 11};
const UINT8 anim_hit[] = {6, 14, 15, 14, 15, 14, 15};
const UINT8 anim_dead[] = {2, 14, 15}; // TBD
const UINT8 anim_victory[] = {2, 14, 15}; // TBD


Sprite* player_sprite;
PlayerData* player_data;
extern Sprite* attack_particle;
static PlayerState curPlayerState, prevPlayerState;
static AnimationState lastAnimState, currentAnimState;

INT16 accel_y;
UINT8 g_lives;
UINT8 reset_x;
UINT8 reset_y;
INT8 shoot_cooldown;
UINT8 bg_hidden;

static void SetPlayerState(PlayerState state) {
	prevPlayerState = curPlayerState;
	curPlayerState = state;
}

static PlayerState GetPlayerState() {
	return curPlayerState;
}

static bool GetPlayerStateEquals(PlayerState ps) {
	return curPlayerState == ps;
}

static void SetAnimationState(AnimationState state) {
	lastAnimState = currentAnimState;
	currentAnimState = state;
	switch (state) {
		case WALK:    		SetSpriteAnim(THIS, anim_walk, WALK_ANIM_SPEED); break;
		case WALK_IDLE:    	SetSpriteAnim(THIS, anim_idle, DEFAULT_ANIM_SPEED);	break;
		case JUMP:    		SetSpriteAnim(THIS, anim_jump, DEFAULT_ANIM_SPEED); break;
		case FALL:    		SetSpriteAnim(THIS, anim_fall, DEFAULT_ANIM_SPEED); break;
		case ATTACK:		if (lastAnimState == JUMP) {
								SetSpriteAnim(THIS, anim_jump_shoot, DEFAULT_ANIM_SPEED);
							} else {
								SetSpriteAnim(THIS, anim_walk_shoot, DEFAULT_ANIM_SPEED);
							}
							break;
		case CLIMB:			SetSpriteAnim(THIS, anim_climb, DEFAULT_ANIM_SPEED); break;
		case CLIMB_IDLE:	SetSpriteAnim(THIS, anim_climb_idle, DEFAULT_ANIM_SPEED); break; 			
		case HIT:			SetSpriteAnim(THIS, anim_hit, HIT_ANIM_SPEED);	break;
		case DEAD:    		SetSpriteAnim(THIS, anim_hit, HIT_ANIM_SPEED); break;
		case VICTORY: 		SetSpriteAnim(THIS, anim_victory, VICTORY_ANIM_SPEED); break;
	}
}

static AnimationState GetAnimationState(void) {
	return currentAnimState;
}


void Hit(Sprite* sprite, UINT8 idx) {
	if (GetPlayerState() != PLAYER_STATE_HIT) {
		SetPlayerState(PLAYER_STATE_HIT);
		gbt_stop();
		NR52_REG = 0x80; //Enables sound, you should always setup this first
		NR51_REG = 0xFF; //Enables all channels (left and right)
		NR50_REG = 0x77; //Max volume
		PlayFx(CHANNEL_1, 10, 0x5b, 0x7f, 0xf7, 0x15, 0x86);
		SetAnimationState(HIT);
		//--g_lives;
		player_data->Lives = player_data->Lives-1
		Hud_Update();
	}		
}

void Collected(Sprite* sprite, UINT8 idx) {
	PlayFx(CHANNEL_1, 10, 0x4f, 0xc7, 0xf3, 0x73, 0x86);
	SpriteManagerRemoveSprite(sprite);
	g_jewell_counter++;
}

void Shoot() {
	SetAnimationState(ATTACK);
	Sprite* bullet_sprite = SpriteManagerAdd(SpriteBullet, 0, 0);
	bullet_sprite->mirror = THIS->mirror;
	if (THIS->mirror) {
		bullet_sprite->x = THIS->x - 2u;
	} else {
		bullet_sprite->x = THIS->x + 7u; 
	}	
	bullet_sprite->y = THIS->y + 12u;
	shoot_cooldown = 10;
}

UINT8 tile_collision;
void CheckCollisionTile(Sprite* sprite, UINT8 idx) {
	if (tile_collision == 50u) { // spikes
		Hit(sprite, idx);
	} else if (tile_collision == 51u) { // jewell
		Collected(sprite, idx);
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

void HandleInput(Sprite* sprite, UINT8 idx) {
	if (KEY_PRESSED(J_RIGHT)) {
		if (GetPlayerState() == PLAYER_STATE_CLIMBING) {
			UINT8 tile = GetScrollTile((player_sprite->x + 8u) >> 3, (player_sprite->y + 22u) >> 3);
			SetAnimationState(WALK);
			if (tile != 59u && tile != 60u) {
				SetPlayerState(PLAYER_STATE_WALKING);
				SetAnimationState(WALK);
				return;
			}
		} 
		tile_collision = TranslateSprite(sprite, 1 << delta_time, 0);
		THIS->mirror = NO_MIRROR;
		CheckCollisionTile(sprite, idx);
		if (GetPlayerState() != PLAYER_STATE_JUMPING && GetPlayerState() != PLAYER_STATE_CLIMBING) {
			SetPlayerState(PLAYER_STATE_WALKING);
			SetAnimationState(WALK);
		}
	} else if (KEY_PRESSED(J_LEFT)) {
		if (GetPlayerState() == PLAYER_STATE_CLIMBING) {
			UINT8 tile = GetScrollTile((player_sprite->x + 8u) >> 3, (player_sprite->y + 22u) >> 3);
			SetAnimationState(WALK);
			if (tile != 59u && tile != 60u) {
				SetPlayerState(PLAYER_STATE_WALKING);
				SetAnimationState(WALK);
				return;
			}
		} 
		tile_collision = TranslateSprite(sprite, -1 << delta_time, 0);
		THIS->mirror = V_MIRROR;
		CheckCollisionTile(sprite, idx);
		if (GetPlayerState() != PLAYER_STATE_JUMPING && GetPlayerState() != PLAYER_STATE_CLIMBING) {
			SetPlayerState(PLAYER_STATE_WALKING);
			SetAnimationState(WALK);
		}
	}
	if (KEY_PRESSED(J_UP)) {
		UINT8 tile = GetScrollTile((player_sprite->x + 8u) >> 3, (player_sprite->y + 23u) >> 3);
		if (tile == 59u || tile == 60u) {
			accel_y = 0;
			tile_collision = TranslateSprite(THIS, 0, -1 << delta_time);
			CheckCollisionTile(sprite, idx);
			SetPlayerState(PLAYER_STATE_CLIMBING);
			SetAnimationState(CLIMB);
		}
	} else if (KEY_PRESSED(J_DOWN)) {
		UINT8 tile = GetScrollTile((player_sprite->x + 8u) >> 3, (player_sprite->y + 23u) >> 3);
		if (tile == 59u || tile == 60u) {
			accel_y = 0;
			tile_collision = TranslateSprite(THIS, 0, 1 << delta_time);
			CheckCollisionTile(sprite, idx);
			SetPlayerState(PLAYER_STATE_CLIMBING);
			SetAnimationState(CLIMB);
		}
	}
	if (KEY_TICKED(J_A) && (GetPlayerState() != PLAYER_STATE_JUMPING)) {
		SetPlayerState(PLAYER_STATE_JUMPING);
		SetAnimationState(JUMP);
		accel_y = -50;
	} else {
		// check if now FALLING?
		if ((accel_y >> 4) > 1) {
			SetAnimationState(FALL);
		}
	}
	if (GetPlayerState() != PLAYER_STATE_HIT) {
		if (shoot_cooldown) {
			shoot_cooldown -= 1u;
		} else {
			if (KEY_TICKED(J_B)) {
				Shoot();
			}
		}
	}
}

//

void START() {
	player_sprite = THIS;
	player_data->Lives = MAX_LIVES;
	curPlayerState = PLAYER_STATE_IDLE;
	accel_y = 0;
	shoot_cooldown = 0;
	bg_hidden = 0;
	scroll_target = THIS;
	lastAnimState = currentAnimState = WALK_IDLE;
	SetAnimationState(currentAnimState);
	//g_lives = MAX_LIVES;
	reset_x = 20;
	reset_y = 80;
}

void UPDATE() {
	UINT8 i;
	Sprite* spr;

	if (GetPlayerState() == PLAYER_STATE_HIT) {
		accel_y = 0;
		SetAnimationState(HIT);	
		if ((THIS->anim_frame + 1) % 2) {
			if (!bg_hidden) {
				HIDE_BKG;
				bg_hidden = 1;
			}
		} else {
			if (bg_hidden) {
				SHOW_BKG;
				bg_hidden = 0;
				PlayFx(CHANNEL_1, 10, 0x5b, 0x7f, 0xf7, 0x15, 0x86);
			}
		}

		if (THIS->anim_frame == 4) {
			SHOW_BKG;
			if (player_data->Lives == 0) { SetState(StateGameOver); }
			//if (g_lives == 0) { SetState(StateGameOver); }
			// move player to start/checkpoint
			THIS->x = reset_x;
			THIS->y = reset_y;
			g_jewell_counter = 0;
			ScrollRelocateMapTo(0, 0);
			SetPlayerState(PLAYER_STATE_IDLE);
			SetAnimationState(WALK_IDLE);
		}	
	}

	if (currentAnimState == ATTACK && shoot_cooldown == 0) {
		SetAnimationState(lastAnimState);
	}

	if (GetPlayerState() == PLAYER_STATE_CLIMBING) {
		UINT8 tile = GetScrollTile((player_sprite->x + 8u) >> 3, (player_sprite->y + 22u) >> 3);
		if (tile != 59u && tile != 60u) {
			//SetPlayerState(PLAYER_STATE_IDLE);
		}
	}
	HandleInput(THIS, THIS_IDX);

	// simple gravity physics until we "collide" with something and not on a ladder
	if (curPlayerState != PLAYER_STATE_CLIMBING) {
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
			if (currentAnimState == JUMP || currentAnimState == FALL) {
				currentAnimState = WALK_IDLE;
			}
			curPlayerState = PLAYER_STATE_IDLE;
			CheckCollisionTile(THIS, THIS_IDX);
		}
	} else {
		// TBD
	}	

	// check sprite collision
	for (i = 0u; i != sprite_manager_updatables[0]; ++i) {
		spr = sprite_manager_sprites[sprite_manager_updatables[i + 1u]];
		if (spr->type == SpriteEnemy1 || spr->type == SpriteEnemy2) {
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
	if (keys == 0 && (!(GetPlayerState() == PLAYER_STATE_JUMPING || GetPlayerState() == PLAYER_STATE_ATTACKING || GetPlayerState() == PLAYER_STATE_HIT))) {
		if (curPlayerState == PLAYER_STATE_CLIMBING) {
			SetAnimationState(CLIMB_IDLE);
		} else {
			SetAnimationState(WALK_IDLE);
		}	
	} 


}

void DESTROY() {
}




