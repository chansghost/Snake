#pragma once

struct Turn_coords {
	double x = -1, y = -1;
	int direction = -1;
	int remaining = 0;
	double speed=0.5;
	Turn_coords* next = nullptr;
	Turn_coords(double x, double y, int direction, int remaining,double speed) {
		this->x = x;
		this->y = y;
		this->direction = direction;
		this->remaining = remaining;
		this->speed = speed;

	};
	Turn_coords() {
	};

};

