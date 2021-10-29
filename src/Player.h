#pragma once
#include "SDL.h"
#include "Collision.h"

struct Keybinds{
	SDL_Keycode up;
	SDL_Keycode down;
	SDL_Keycode left;
	SDL_Keycode right;
};

class Player{
public:
	Collider hitbox;
	Keybinds binds;
	double moveSpeed;
	
	Player(Collider i_hitbox, Keybinds i_keybinds, double i_moveSpeed);
};
