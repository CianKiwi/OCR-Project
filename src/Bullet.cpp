#include "Bullet.h"

Bullet::Bullet(Vec2 position, Vec2 direction, double speed)
: pos{position}{
	vel = normalize(direction);
	vel.x *= speed;
	vel.y *= speed;
}

Bullet::Bullet(Vec2 position, Vec2 direction, double speed, double maxDistance, int damage)
: pos{position}, maxDistance{maxDistance}, damage{damage}{
	vel = normalize(direction);
	vel.x *= speed;
	vel.y *= speed;
}