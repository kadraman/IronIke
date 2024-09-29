#ifndef GLOBALVARS_H_
#define GLOBALVARS_H_

#define MAX_LIVES 3
#define MAX_LEVEL 2

extern UINT8 g_level_current;
extern UINT8 g_player_region;

typedef struct {
	UINT8 x;
	UINT8 y;
} ItemPos;

#endif