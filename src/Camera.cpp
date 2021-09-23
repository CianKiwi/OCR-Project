#include "Camera.h"

Camera::Camera(double i_x, double i_y, int width, int height) 
	: x{i_x}, y{i_y}{
	view.w = width;
	view.h = height;
	view.x = (int)x - width/2;
	view.y = (int)y - height/2;
}

void Camera::set_pos(double i_x, double i_y){
	x = i_x;
	y = i_y;
	view.x = (int)x - view.w/2;
	view.y = (int)y - view.h/2;
}

void Camera::move(double i_x, double i_y){
	x += i_x;
	y += i_y;
	view.x = (int)x - view.w/2;
	view.y = (int)y - view.h/2;
}