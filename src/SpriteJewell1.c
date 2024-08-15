#include "Banks/SetAutoBank.h"
#include "Sound.h"
#include "main.h"
#include "SpriteManager.h"

#include "GlobalVars.h"

const UINT8 anim_jewell[] = {1, 0};

extern Sprite* player_sprite;


void START() {
	SetSpriteAnim(THIS, anim_jewell, 20u);
}

void UPDATE() {

	if (CheckCollision(THIS, player_sprite)) {
		PlayFx(CHANNEL_1, 10, 0x00, 0x81, 0x83, 0xA3, 0x87);
		g_jewell_counter++;
		SpriteManagerRemoveSprite(THIS);
	}
}

void DESTROY() {
}