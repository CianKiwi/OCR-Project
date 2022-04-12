#pragma once
#include "Collision.h"
#include "Tileset.h"

class Enemy {
public:
	Collider hitbox;
	int HP = 1;
	
	Enemy(Collider i_hitbox, int health);
	Enemy();
};