#pragma once
#include "Collision.h"
#include "Tileset.h"

class Enemy {
public:
	Collider hitbox;
	int HP = 1;
	double moveSpeed = 2;
	double AIupdateRate = 300;
	double timeSinceUpdate = 0;
	double detectionDistance = 350;
	double attackRate = 100;
	double timeSinceAttack = 0;
	Vec2 moveTarget;
	
	Enemy(Collider i_hitbox, int health);
	Enemy();
};