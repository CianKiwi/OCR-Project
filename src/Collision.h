#pragma once
#include <iostream>

struct directionBool{
	bool up;
	bool down;
	bool left;
	bool right;
};

class Collider{
public:
	double x;
	double y;
	double width;
	double height;
	Collider(double i_x, double i_y, double i_width, double i_height);
	Collider();
	void set_pos(double i_x, double i_y);
	void move(double i_x, double i_y);
	void print_data();
};

struct CollisionData{
	bool collision;
	Collider* a;
	Collider* b;
	//direction from a's overlapping side to b's nearest side
	directionBool direction;
	double overlap_x;
	double overlap_y;
};

CollisionData check_collision(Collider* a, Collider* b);

void print_collisionData(CollisionData data);