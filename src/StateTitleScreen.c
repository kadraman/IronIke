#include "Banks/SetAutoBank.h"
#include "main.h"

#include <gb/gb.h>
#include "ZGBMain.h"
#include "Scroll.h"
#include "Keys.h"
#include "Music.h"

#include "GlobalVars.h"

IMPORT_MAP(titlescreen);

//DECLARE_MUSIC(titles);

void START() {
	InitScroll(BANK(titlescreen), &titlescreen, 0, 0);
	HIDE_WIN;
	//PlayMusic(titles, 0);
}

void UPDATE() {
	if(KEY_TICKED(J_START) | KEY_TICKED(J_A) | KEY_TICKED(J_B)) {
		g_level_current = 1;
		SetState(StateGame);
	}
}