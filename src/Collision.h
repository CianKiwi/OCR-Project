#pragma once
#include <iostream>

struct Vec2{
	double x;
	double y;
};

//NOTE: while SDL does have SDL_FRect, a pure c++ version is more portable to other frameworks
class Collider{
public:
	Vec2 pos;
	Vec2 dim;
	Vec2 vel;
	Collider(Vec2 position, Vec2 dimensions);
	Collider();
};

bool check_collision(Collider* a, Collider* b);