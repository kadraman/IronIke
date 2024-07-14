#include "Banks/SetAutoBank.h"
#include "main.h"

#include <gb/gb.h>
#include "ZGBMain.h"
#include "Scroll.h"
#include "Keys.h"
#include "Music.h"

#include "GlobalVars.h"

IMPORT_MAP(gameover);

//DECLARE_MUSIC(gameover);

void START() {
	InitScroll(BANK(gameover), &gameover, 0, 0);
	HIDE_WIN;
	//PlayMusic(gameover, 0);
}

void UPDATE() {
	if(KEY_TICKED(J_START) | KEY_TICKED(J_A) | KEY_TICKED(J_B)) {
		g_jewell_counter = 0;
		SetState(StateGame);
	}
}