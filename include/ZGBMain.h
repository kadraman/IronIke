#ifndef ZGBMAIN_H
#define ZGBMAIN_H

#define STATES \
_STATE(StateSplash)\
_STATE(StateGame)\
_STATE(StateLevel1)\
_STATE(StateLevel2)\
_STATE(StateTimeUp)\
_STATE(StateGameOver)\
_STATE(StateWin)\
STATE_DEF_END

#define SPRITES \
_SPRITE(SpritePlayer,	  player,     V_MIRROR)\
_SPRITE(SpriteFlag,       flag,       FLIP_NONE)\
_SPRITE(SpriteCoin,       coin,       FLIP_NONE)\
_SPRITE(SpriteEnemy1,     enemy1,     V_MIRROR)\
_SPRITE(SpriteSpirit,     spirit,     V_MIRROR)\
_SPRITE(SpriteAttack1,    attack1,    V_MIRROR)\
_SPRITE(SpriteBullet,     bullet,     V_MIRROR)\
_SPRITE(SpriteKunai,      kunai,      V_MIRROR)\
_SPRITE(SpriteEnemy2,     enemy2,     V_MIRROR)\
_SPRITE(SpriteParticle,   particles,  FLIP_NONE)\
_SPRITE(SpriteJewell1,    jewell1,    FLIP_NONE)\
_SPRITE(SpriteAmmo,       ammo,       FLIP_NONE)\
SPRITE_DEF_END

#include "ZGBMain_Init.h"

#endif // ZGBMAIN_H