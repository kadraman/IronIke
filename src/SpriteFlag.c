#include "Banks/SetAutoBank.h"
#include "main.h"

#include "SpriteManager.h"

const UINT8 anim_flag_idle[] = {1, 0};
const UINT8 anim_flag_enabled[] = {6, 0, 1, 2, 3, 4, 5};

extern UINT16 reset_x;
extern UINT16 reset_y;

void START() {
	SetSpriteAnim(THIS, anim_flag_enabled, 20u);
}

void UPDATE() {
	/*if(THIS->anim_data == anim_flag_idle) {
		if(reset_x == THIS->x && reset_y == THIS->y) {
			SetSpriteAnim(THIS, anim_flag_enabled, 5u);
		}
	}*/
}

void DESTROY() {
}