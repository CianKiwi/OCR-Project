#include "Enemy.h"

Enemy::Enemy(Collider i_hitbox, int health)
	: hitbox{ i_hitbox }, HP{health}{
}

Enemy::Enemy() {
}