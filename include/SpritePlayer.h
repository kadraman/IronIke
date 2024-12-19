#ifndef SPRITEPLAYER_H
#define SPRITEPLAYER_H

#include "main.h"
#include "SpriteManager.h"
#include "Sprite.h"
#include "ZGBMain.h"
#include "GlobalVars.h"
#include "ZGBUtils.h"
#include "Hud.h"

#define DEBUG_CONTROLS 0

#define MAX_DECEL_X 10u
#define GROUNDED_BIT 7u
#define DOUBLE_JUMP_BIT 6u
#define DEFAULT_ANIM_SPEED 10u
#define WALK_ANIM_SPEED 10u
#define HIT_ANIM_SPEED 254u
#define DIE_ANIM_SPEED 254u
#define VICTORY_ANIM_SPEED 8u
#define MAX_BULLETS 255u

typedef enum {
	PLAYER_STATE_IDLE,
	PLAYER_STATE_WALKING,
	PLAYER_STATE_BEFORE_JUMP,
	PLAYER_STATE_JUMPING,
	PLAYER_STATE_DOUBLE_JUMPING,
	PLAYER_STATE_AFTER_JUMP,
	PLAYER_STATE_FALLING,
	PLAYER_STATE_CLIMBING,
	PLAYER_STATE_PUSHING,
	PLAYER_STATE_ATTACKING,
	PLAYER_STATE_HIT,
	PLAYER_STATE_DIE,
	PLAYER_STATE_TIMEOVER
} PlayerState;

typedef enum {
    NORMAL,
	WALK,
	WALK_IDLE,
	BEFORE_JUMP,
	JUMP,
	DOUBLE_JUMP,
	AFTER_JUMP,
	FALL,
	CLIMB,
	CLIMB_IDLE,
	PUSH,
    ATTACK,
    HIT,
	DIE,
	VICTORY
} AnimationState;

typedef struct {
	UINT8 anim_playing;
	UINT8 lives;
	UINT8 bullets;
	UINT8 coins;
	UINT8 spirits;
	UINT8 timeup;
	UINT8 invincible;
} PlayerData;

#endif // SPRITEPLAYER_H