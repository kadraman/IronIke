#ifndef HUD_H_
#define HUD_H_

#include "Banks/SetAutoBank.h"
#include "GlobalVars.h"

#define DEBUG_HUD 0

void Hud_Init() BANKED;
void Hud_Update () BANKED;

#endif // HUD_H