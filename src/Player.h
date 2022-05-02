#pragma once
#include "SDL.h"
#include "Collision.h"
#include "Structs.h"
#include "Tileset.h"
#include "Weapon.h"

class Player{
public:
	Collider hitbox;
	Keybinds binds;
	Weapon weapon;
	double moveSpeed;
	double sprintMulti = 1.5f;
	double doorInteractDist = 95;
	int health = 5;
	bool isSprinting = false;
	double holdDamageFramesFor = 0;
	
	Player(Collider i_hitbox, Keybinds i_keybinds, Weapon i_weapon, double i_moveSpeed);
};
