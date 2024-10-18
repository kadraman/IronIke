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
	HIDE_HUD;
	//InitScroll(BANK(gameover), &gameover, 0, 0);
	INIT_BKG(gameover);
	//PlayMusic(gameover, 0);
}

void UPDATE() {
	if(KEY_TICKED(J_START) | KEY_TICKED(J_A) | KEY_TICKED(J_B)) {
		SetState(StateGame);
	}
}