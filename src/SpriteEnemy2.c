#include "Banks/SetAutoBank.h"
#include "main.h"

#include <gb/gb.h>

#include "Scroll.h"
#include "SpriteManager.h"

//const UINT8 anim_enemy2_up[] = {2, 0, 1};
//const UINT8 anim_enemy2_down[] = {2, 0, 1};
const UINT8 anim_enemy2_move[] = {2, 0, 1};
const INT8 enemy2Dist = 64u;
struct EnemyInfo
{
    UINT8 wait;
    BYTE goingUp;
    INT8 dist;
};

void START() {
	struct EnemyInfo* data = (struct EnemyInfo*)THIS->custom_data;
	data->goingUp = 1;
    data->dist = enemy2Dist;
	SetSpriteAnim(THIS, anim_enemy2_move, 5u);
	THIS->lim_x = 160u;
	THIS->lim_y = 160u;
}

void UPDATE() {
	struct EnemyInfo* data = (struct EnemyInfo*)THIS->custom_data;
	if (data->wait == 2)
    {
		data->wait = 0;
		data->dist--;

		if (data->dist == 0)
        {
            data->goingUp = !data->goingUp;
            data->dist = enemy2Dist;
        }
		if (data->goingUp)
        {
            //SetSpriteAnim(THIS, anim_enemy2_up, 10u);
            if (TranslateSprite(THIS,0,-1))
            {
                data->goingUp = !data->goingUp;
                data->dist = enemy2Dist;
            }
        }
        else
        {
            //SetSpriteAnim(THIS, anim_enemy2_down, 15u);
            if (TranslateSprite(THIS,0,1))
            {
                data->goingUp = !data->goingUp;
                data->dist = enemy2Dist;
            }
        }
    } 
	data->wait++;
}

void DESTROY() {
}