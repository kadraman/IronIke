#include "Banks/SetAutoBank.h"

#include <gbdk/platform.h>
#include "ZGBMain.h"
#include "Scroll.h"
#include "Sprite.h"
#include "SpriteManager.h"
#include "SpritePlayer.h"

IMPORT_MAP (hud);

// saved last drawn values, to work out what to update on hud
static UINT8 lastLives = 0;
static UINT8 lastBullets = 0;

extern Sprite* player_sprite;

void Hud_Init() BANKED {
    PlayerData* data = (PlayerData*)player_sprite->custom_data;
    INIT_HUD(hud);
    // prime the last values so they all get updated
    lastLives = data->lives + 1;
    lastBullets = data->bullets + 1;
}

static UINT8 getTens (UINT8 full) {
    UINT8 t = 0 ;
    while (full > 9) {
        full -= 10;
        t ++;
    }
    return t;
}

static void PutU16 (UINT16 v, UINT8 at)
{
    UINT8 thous;
    UINT8 hundreds;
    UINT8 tens;
    UINT8 ones;

    thous = v / 1000;
    v -= thous*1000;
    hundreds = v/ 100;
    v -= hundreds * 100;
    tens = getTens((UINT8) v);
    ones = v - tens*10;

     UPDATE_HUD_TILE (at++, 0, 4 + thous);
     UPDATE_HUD_TILE (at++, 0, 4 + hundreds);
     UPDATE_HUD_TILE (at++, 0, 4 + tens);
     UPDATE_HUD_TILE (at++, 0, 4 + ones);
}

void Hud_Update() BANKED {
    PlayerData* data = (PlayerData*)player_sprite->custom_data;
    UINT8 tens;
    UINT8 ones;

    if (lastBullets != data->bullets) {
        lastBullets = data->bullets;
        tens = getTens(data->bullets);
        ones = data->bullets - (tens * 10);
        UPDATE_HUD_TILE (3, 0, 4 + tens);
        UPDATE_HUD_TILE (4, 0, 4 + ones);
    }

    //if (lastLives != g_lives) {
    if (lastLives != data->lives) {
        // update HUD lives
        lastLives = data->lives;
        for (UINT8 i = 0; i < MAX_LIVES; ++i) {
            UPDATE_HUD_TILE(16 + i, 0, i < data->lives ? 1 : 2);
        }
    }

#if(DEBUG_HUD ==1)
    // player position
    //PutU16 (player_sprite->x, 6);
    //PutU16 (player_sprite->y, 6 );
    // current tile
    UINT8 tile = GetScrollTile((player_sprite->x + 8u) >> 3, (player_sprite->y + 16u) >> 3);
    PutU16(tile, 6);
    UINT8 af = player_sprite->anim_frame;
    PutU16(af, 8);
#endif

}

