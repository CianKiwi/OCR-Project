#pragma once
#include "SDL.h"
#include "Collision.h"
#include "Structs.h"

class Player{
public:
	Collider hitbox;
	Keybinds binds;
	double moveSpeed;
	
	Player(Collider i_hitbox, Keybinds i_keybinds, double i_moveSpeed);
};
