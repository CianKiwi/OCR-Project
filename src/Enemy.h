#pragma once
#include "Collision.h"
#include "Tileset.h"

class Enemy {
public:
	Collider hitbox;

	Enemy(Collider i_hitbox);
	Enemy();
};