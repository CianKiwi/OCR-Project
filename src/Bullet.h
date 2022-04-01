#pragma once
#include "Collision.h"

class Bullet {
public:
	Collider hitbox;
	bool isFriendly;
	double speed = 15;

	Bullet(Vec2 position, Vec2 direction, bool i_isFriendly);
};