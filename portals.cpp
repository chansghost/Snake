#include "portals.h"

Portal::Portal(int number, SDL_Renderer* renderer, int portal_number) : portal_number(portal_number), Dot(renderer) {
	SDL_Surface* temp_dot;
	if (number % 2 == 0) {//even number = 2nd image
		temp_dot = SDL_LoadBMP("./tp_2.bmp");
	}
	else {
		temp_dot = SDL_LoadBMP("./tp_1.bmp");
	}
	if (!temp_dot) {
		printf("Error loading sprites dot: %s\n", SDL_GetError());
		exit(1);
	}
	dot_sprite = SDL_CreateTextureFromSurface(renderer, temp_dot);
	SDL_FreeSurface(temp_dot);
}

Portal::~Portal() {

}

int Portal::get_portal_nr() {
	return portal_number;
}

void Portal::set_portal_nr(int nr) {
	portal_number = nr;
}

