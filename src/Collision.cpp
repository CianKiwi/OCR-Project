#include "Collision.h"

Collider::Collider(double i_x, double i_y, double i_width, double i_height)
	: x{i_x}, y{i_y}, width{i_width}, height{i_height}{
}

Collider::Collider(){
	x = y = width = height = 0;
}

void Collider::set_pos(double i_x, double i_y){
	x = i_x;
	y = i_y;
}

void Collider::move(double i_x, double i_y){
	x += i_x;
	y += i_y;
}

void Collider::print_data(){
	std::cout << "Collider collider{" << "\n";
	std::cout << "\t x: " << x << "\n";
	std::cout << "\t y: " << y << "\n";
	std::cout << "\t width: " << width << "\n";
	std::cout << "\t height: " << height << "\n";
	std::cout << "}" << std::endl;
}

CollisionData check_collision(Collider* a, Collider* b){
	CollisionData data;
	data.a = a;
	data.b = b;
	
	/*IMPLEMENT CHECK*/
	//1: has a collision occured
	//2: how much does a overlap b
	//3: which direction is B from A
	
	//1
	if (a->x < b->x + b->width &&
	a->x + a->width > b->x &&
	a->y < b->y + b->height &&
	a->y + a->height > b->y){
		data.collision = true;
		
		//2
		if(a->x > b->x){
			data.overlap_x = b->x + b->width - a->x;
		}
		else{
			data.overlap_x = a->x - b->x;
		}
		if(a->y > b->y){
			data.overlap_y = b->y + b->height - a->y;
		}
		else{
			data.overlap_y = a->y - b->y;
		}
		
		//3
		if(abs(data.overlap_x) < abs(data.overlap_y)){
			if(data.overlap_x > 0) data.direction.left = true;
			else data.direction.right = true;
		}
		else{
			if(data.overlap_y > 0) data.direction.up = true;
			else data.direction.down = true;
		}
	}
	else{ //there is no collision
		data.collision = false;
		data.direction = {false, false, false, false};
		data.overlap_x = data.overlap_y = 0;
	}
	
	return data;
}

void print_collisionData(CollisionData data){
	std::cout << "CollisionData data{" << "\n";
		std::cout << "\tcollision: " << (data.collision ? "true" : "false") << "\n";
		std::cout << "\ta: ";
		data.a->print_data();
		std::cout << "\tb: ";
		data.b->print_data();
		std::cout << "\tdirection: " << data.direction.up << data.direction.down << data.direction.left << data.direction.right << "\n";
		std::cout << "\toverlap_x: " << data.overlap_x << "\n";
		std::cout << "\toverlap_y: " << data.overlap_y << "\n";
		std::cout << "}" << std::endl;
}