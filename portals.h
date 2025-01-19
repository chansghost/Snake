#pragma once
#include "dot.h"

class Portal : public Dot {
public:
	Portal(int number, SDL_Renderer* renderer, int portal_number);
	~Portal();

	int get_portal_nr();
	void set_portal_nr(int nr);

private:
	int portal_number = 0;
};