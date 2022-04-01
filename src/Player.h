#pragma once
#include "SDL.h"
#include "Collision.h"
#include "Structs.h"
#include "Tileset.h"

class Player{
public:
	Collider hitbox;
	Keybinds binds;
	double moveSpeed;
	double sprintMulti = 1.5f;
	double doorInteractDist = 95;
	
	Player(Collider i_hitbox, Keybinds i_keybinds, double i_moveSpeed);
};
