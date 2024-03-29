#ifndef ZGBMAIN_H
#define ZGBMAIN_H

#define STATES \
_STATE(StateGame)\
STATE_DEF_END

#define SPRITES \
_SPRITE_COLOR(SpritePlayer,	oebbi)\
_SPRITE_COLOR(SpriteAttack, oebbi)\
_SPRITE_COLOR(SpriteOcti,   octi)\
_SPRITE_COLOR(SpriteParticle, particles)\
_SPRITE_COLOR(SpriteFlag, flag)\
SPRITE_DEF_END

#include "ZGBMain_Init.h"

#endif