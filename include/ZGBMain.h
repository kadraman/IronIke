#ifndef ZGBMAIN_H
#define ZGBMAIN_H

#define STATES \
_STATE(StateGame)\
_STATE(StateLevel1)\
_STATE(StateLevel2)\
_STATE(StateTimeUp)\
_STATE(StateGameOver)\
_STATE(StateWin)\
STATE_DEF_END

#define SPRITES \
_SPRITE_COLOR(SpritePlayer,	    player)\
_SPRITE_COLOR(SpriteAttack1,    attack1)\
_SPRITE_COLOR(SpriteBullet,     bullet)\
_SPRITE_COLOR(SpriteKunai,      kunai)\
_SPRITE_COLOR(SpriteCoin,       coin)\
_SPRITE_COLOR(SpriteEnemy1,     enemy1)\
_SPRITE_COLOR(SpriteEnemy2,     enemy2)\
_SPRITE_COLOR(SpriteParticle,   particles)\
_SPRITE_COLOR(SpriteFlag,       flag)\
_SPRITE_COLOR(SpriteJewell1,    jewell1)\
_SPRITE_COLOR(SpriteAmmo,       ammo)\
SPRITE_DEF_END

#include "ZGBMain_Init.h"

#endif // ZGBMAIN_H