#include "Collision.h"

Collider::Collider(Vec2 position, Vec2 dimensions)
:pos{position}, dim{dimensions}{
	vel = {0,0};
}

Collider::Collider(){
	pos = {0,0};
	dim = {0,0};
	vel = {0,0};
}

bool check_collision(Collider* a, Collider* b){

	return (a->pos.x < b->pos.x + b->dim.x &&
	a->pos.x + a->dim.x > b->pos.x &&
	a->pos.y < b->pos.y + b->dim.y &&
	a->pos.y + a->dim.y > b->pos.y);

}