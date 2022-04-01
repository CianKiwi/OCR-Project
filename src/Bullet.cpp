#include "Bullet.h"

Bullet::Bullet(Vec2 position, Vec2 direction, bool i_isFriendly)
	:isFriendly{ i_isFriendly } {
	hitbox.pos = position;
	hitbox.dim = {5, 5};
	hitbox.isDynamic = true;
	hitbox.isActive  = true;
	hitbox.isTrigger = true;
	hitbox.vel = normalize(direction);
	hitbox.vel.x *= speed;
	hitbox.vel.y *= speed;
}