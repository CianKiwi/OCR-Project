#pragma once
#include "SDL.h"

struct Index2{
    int r, c;
};

struct CardinalBool{
	bool N, E, S, W = false;
};

bool operator==(const CardinalBool& lhs, const CardinalBool& rhs);

const CardinalBool _NORTH = {1, 0, 0, 0};
const CardinalBool _EAST = {0, 1, 0, 0};
const CardinalBool _SOUTH = {0, 0, 1, 0};
const CardinalBool _WEST = {0, 0, 0, 1};

struct Keybinds{
	SDL_Keycode up;
	SDL_Keycode down;
	SDL_Keycode left;
	SDL_Keycode right;
};