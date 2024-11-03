#include "Banks/SetAutoBank.h"
#include "main.h"

#include <gb/gb.h>
#include "ZGBMain.h"
#include "Scroll.h"
#include "Keys.h"
#include "Music.h"

#include "GlobalVars.h"

IMPORT_MAP(titles);

//DECLARE_MUSIC(titles);

void START() {
	HIDE_HUD;
	InitScroll(BANK(titles), &titles, 0, 0);
	//PlayMusic(titles, 0);
}

void UPDATE() {
	if (ANY_KEY_PRESSED) {
		g_level_current = 1;
		SetState(StateGame);
	}
}