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

//NOTE: while SDL does have SDL_FRect, a pure c++ version is more portable to other frameworks
class Collider{
public:
	Vec2 pos;
	Vec2 dim;
	Collider(Vec2 position, Vec2 dimensions);
	Collider();
};

bool check_collision(Collider* a, Collider* b);

RaycastData check_ray(Vec2 start, Vec2 dir, Collider* c);