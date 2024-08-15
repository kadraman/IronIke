#ifndef SPRITEPLAYER_H
#define SPRITEPLAYER_H

#include "main.h"
#include "SpriteManager.h"
#include "Sprite.h"
#include "ZGBMain.h"
#include "GlobalVars.h"
#include "zgb_utils.h"
#include "Hud.h"

//#define DEBUG_CONTROLS

#define WALK_ANIM_SPEED 10

typedef enum {
    NORMAL,
	IDLE, 
	WALK, 
	JUMP,
	FALL,
    ATTACK,
    HIT,
	DEAD,
	VICTORY
} AnimationState;

#endif // SPRITEPLAYER_H