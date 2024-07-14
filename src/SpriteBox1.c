#include "Banks/SetAutoBank.h"
#include "Sound.h"
#include "main.h"
#include "SpriteManager.h"

#include "GlobalVars.h"

const UINT8 anim_box[] = {1, 0};

void START() {
	SetSpriteAnim(THIS, anim_box, 20u);
}

void UPDATE() {
}

void DESTROY() {
}