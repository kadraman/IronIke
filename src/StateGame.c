#include "Banks/SetAutoBank.h"

#include "ZGBMain.h"
#include "Scroll.h"
#include "SpriteManager.h"
#include "Print.h"
#include "Sound.h"

#include "GlobalVars.h"

extern UINT8 g_level_current;

void START() {

	switch (g_level_current) {
		case 1:
			SetState(StateLevel1);
			break;
		case 2:
			SetState(StateLevel1);
			break;
		default:
			SetState(StateLevel1);
			break;
	}

}

void UPDATE() {

}

