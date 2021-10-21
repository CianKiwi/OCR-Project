#include "Collision.h"

Collider::Collider(Vec2 position, Vec2 dimensions)
:pos{position}, dim{dimensions}{
}

Collider::Collider(){
	pos = {0,0};
	dim = {0,0};
}

bool check_collision(Collider* a, Collider* b){

	return (a->pos.x < b->pos.x + b->dim.x &&
	a->pos.x + a->dim.x > b->pos.x &&
	a->pos.y < b->pos.y + b->dim.y &&
	a->pos.y + a->dim.y > b->pos.y);

}

RaycastData check_ray(Vec2 start, Vec2 dir, Collider* c){
	
	//points of collision across the collider (tNear is closest to start)
	Vec2 tNear = {(c->pos.x - start.x) / dir.x, (c->pos.y - start.y) / dir.y};
	Vec2 tFar = {(c->pos.x + c->dim.x - start.x)/dir.x, (c->pos.y + c->dim.y - start.y)/dir.y};
	
	RaycastData data = {false, {0,0}, {0,0}, 0};
	
	//account for where the ray is coming from relative to the collider
	if (tNear.x > tFar.x) std::swap(tNear.x, tFar.x);
	if (tNear.y > tFar.y) std::swap(tNear.y, tFar.y);
	
	//collision has not occured
	if (tNear.x > tFar.y || tNear.y > tFar.x) {
		std::cout << "no collision has occured" << std::endl;
		return data;
	}
	
	float tHitNear, tHitFar;
	
	tHitNear = std::max(tNear.x, tNear.y);
	tHitFar = std::min(tFar.x, tFar.y);
	
	data.contactTime = tHitNear;
	
	//collider is behind the ray
	if(tHitFar < 0) {
		std::cout << "collider is behind the ray" << std::endl;
		return data;
	}
	
	data.contactPoint = {start.x + tHitNear * dir.x, start.y + tHitNear * dir.y};
	
	if(tNear.x > tNear.y){
		if(dir.x < 0){
			data.contactNormal = {1, 0};
		}
		else{
			data.contactNormal = {-1, 0};
		}
	}
	else{
		if(dir.y < 0){
			data.contactNormal = {0, 1};
		}
		else{
			data.contactNormal = {0, -1};
		}
	}
	
	data.contact = true;
	return data;
}