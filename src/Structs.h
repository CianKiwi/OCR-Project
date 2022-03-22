#pragma once
#include "SDL.h"

struct Index2{
	int r, c;
};

struct CardinalBool{
	bool data[4] = {0,0,0,0};
	bool& N = data[0];
	bool& E = data[1];
	bool& S = data[2];
	bool& W = data[3];
	CardinalBool();
	CardinalBool(bool n, bool e, bool s, bool w);
	void operator=(const CardinalBool& a);
};

bool operator==(const CardinalBool& lhs, const CardinalBool& rhs);

bool operator!=(const CardinalBool& a, const CardinalBool& b);



const CardinalBool _EMPTY(0, 0, 0, 0);
const CardinalBool _FULL(1, 1, 1, 1);
const CardinalBool _NORTH(1, 0, 0, 0);
const CardinalBool _EAST(0, 1, 0, 0);
const CardinalBool _SOUTH(0, 0, 1, 0);
const CardinalBool _WEST(0, 0, 0, 1);

struct Keybinds{
	SDL_Keycode up;
	SDL_Keycode down;
	SDL_Keycode left;
	SDL_Keycode right;
};