#pragma once

struct Index2{
    int r, c;
};

struct CardinalBool{
	bool N, E, S, W = false;
};

struct Keybinds{
	SDL_Keycode up;
	SDL_Keycode down;
	SDL_Keycode left;
	SDL_Keycode right;
};