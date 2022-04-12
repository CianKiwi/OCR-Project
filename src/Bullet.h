#pragma once
#include "Collision.h"

class Bullet {
public:
	Vec2 pos;
	Vec2 vel;
	
	Bullet(Vec2 position, Vec2 direction, double speed);
};