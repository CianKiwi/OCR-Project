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

	RaycastData();
};

struct SortingData{
	int index;
	double contactTime;
};

//NOTE: while SDL does have SDL_FRect, a pure c++ version is more portable to other frameworks and programs
class Collider{
public:
	int world_ID; //set when colliders are placed into the world
	Vec2 pos;
	Vec2 dim;
	Vec2 vel;
	bool isDynamic; //is able to move
	bool isTrigger; //doesnt cause physical interactions
	bool isActive;
	Collider(Vec2 position, Vec2 dimensions, bool active, bool dynamic, bool trigger);
	Collider(Vec2 position, Vec2 dimensions);
	Collider();
};

bool check_overlap(Collider* a, Collider* b);

RaycastData check_ray(Vec2 start, Vec2 dir, Collider* c);

RaycastData check_dynamic_collision(Collider* a, Collider* b);

double calc_distance(Vec2 start, Vec2 end);