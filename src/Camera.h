#pragma once
#include "SDL.h"

class Camera{
public:
	double x;
	double y;
	SDL_Rect view;
	Camera(double i_x, double i_y, int width, int height);
	void set_pos(double i_x, double i_y);
	void move(double i_x, double i_y);
};