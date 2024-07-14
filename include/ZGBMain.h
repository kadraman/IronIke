#ifndef ZGBMAIN_H
#define ZGBMAIN_H

#define STATES \
_STATE(StateGame)\
_STATE(StateGameOver)\
_STATE(StateWin)\
STATE_DEF_END

#define SPRITES \
_SPRITE_COLOR(SpritePlayer,	    oebbi)\
_SPRITE_COLOR(SpritePunch,      punch)\
_SPRITE_COLOR(SpriteEnemy1,     enemy1)\
_SPRITE_COLOR(SpriteParticle,   particles)\
_SPRITE_COLOR(SpriteFlag,       flag)\
_SPRITE_COLOR(SpriteJewell1,    jewell1)\
_SPRITE_COLOR(SpriteBox1,       box1)\
SPRITE_DEF_END

#include "ZGBMain_Init.h"

#endif