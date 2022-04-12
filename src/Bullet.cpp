#include "Bullet.h"

Bullet::Bullet(Vec2 position, Vec2 direction, double speed)
: pos{position}{
	vel = normalize(direction);
	vel.x *= speed;
	vel.y *= speed;
}