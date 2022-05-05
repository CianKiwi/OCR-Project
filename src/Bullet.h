#pragma once
#include "Collision.h"

class Bullet {
public:
	Vec2 pos;
	Vec2 vel;
	double maxDistance = 400;
	double distanceTravelled = 0;
	int damage = 1;
	
	Bullet(Vec2 position, Vec2 direction, double speed);
	Bullet(Vec2 position, Vec2 direction, double speed, double maxDistance, int damage);
};