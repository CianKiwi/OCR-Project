#pragma once
#include <iostream>
#include <cmath>

struct Vec2{
	double x;
	double y;
};

struct RaycastData{
	bool contact;
	Vec2 contactPoint;
	Vec2 contactNormal;
	double contactTime;
};

struct SortingData{
	int index;
	double contactTime;
};

//NOTE: while SDL does have SDL_FRect, a pure c++ version is more portable to other frameworks and programs
class Collider{
public:
	Vec2 pos;
	Vec2 dim;
	Vec2 vel;
	bool isDynamic; //is able to move
	bool isTrigger; //passed through by other dynamic colliders
	bool isActive;
	Collider(Vec2 position, Vec2 dimensions, bool active, bool dynamic, bool trigger);
	Collider();
};

bool check_overlap(Collider* a, Collider* b);

RaycastData check_ray(Vec2 start, Vec2 dir, Collider* c);

RaycastData check_dynamic_collision(Collider* a, Collider* b);