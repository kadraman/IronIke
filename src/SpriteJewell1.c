#include <stdbool.h>
#include "Banks/SetAutoBank.h"
#include "Sound.h"
#include "main.h"
#include "SpriteManager.h"

#include "GlobalVars.h"

const UINT8 anim_jewell[] = {1, 0};

extern Sprite* player_sprite;
typedef struct {
    bool collected;
} CustomData;


void START() {
	CustomData* data = (CustomData*)THIS->custom_data;
	data->collected = false;
	SetSpriteAnim(THIS, anim_jewell, 20u);
}

void UPDATE() {
	CustomData* data = (CustomData*)THIS->custom_data;
	if (CheckCollision(THIS, player_sprite)) {
		PlayFx(CHANNEL_1, 10, 0x00, 0x81, 0x83, 0xA3, 0x87);
		data->collected = true;
		SpriteManagerRemoveSprite(THIS);
	}
}

void DESTROY() {
}