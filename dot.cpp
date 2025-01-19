#include "dot.h"


Dot::Dot(bool blue, SDL_Renderer* renderer):blue(blue),renderer(renderer) {
	spawn();
	SDL_Surface* temp_dot;
	if (blue) {
		temp_dot = SDL_LoadBMP("./blue_dot.bmp");
	}
	else {
		temp_dot = SDL_LoadBMP("./red_dot.bmp");
		setTime();
	}
	
	if (!temp_dot) {
		printf("Error loading sprites dot: %s\n", SDL_GetError());
		exit(1);
	}
	dot_sprite = SDL_CreateTextureFromSurface(renderer, temp_dot);
	SDL_FreeSurface(temp_dot);

}

Dot::Dot(SDL_Renderer* renderer): renderer(renderer) {
	spawn();
}

void Dot::setTime() {
	spawn_time = (rand() % (MAX_WAIT_FOR_SPAWN - MIN_WAIT_FOR_SPAWN)) + MIN_WAIT_FOR_SPAWN;
}

const int Dot::getTime() {
	return spawn_time;
}

void Dot::render(Uint32 time){
	scale = 0.8 + 0.4 * (sin(time * pulse_speed) + 1) / 2; // 0.8-1.2
	int scaledWidth = static_cast<int>(DOT_SIZE * scale);
	int scaledHeight = static_cast<int>(DOT_SIZE * scale);

	SDL_Rect dstRect = { x - scaledWidth / 2, y - scaledHeight / 2, scaledWidth, scaledHeight };
	SDL_RenderCopy(renderer, dot_sprite, NULL, &dstRect); 
}

double Dot::getScale() {
	return scale;
}

void Dot::setScale(double newscale) {
	scale = newscale;
}

bool Dot::getSpawned() {
	return spawned;
}

void Dot::setPos(int newx, int newy) {
	x = newx;
	y = newy;
}

void Dot::setSpawned(bool newspawned) {
	spawned = newspawned;
}

void Dot::setTime(int time) {
	spawn_time = time;
}

void Dot::spawn() {
	if (!blue) setTime();
	x = (rand() % (BORDER_RIGHT - (BORDER_LEFT+DOT_SIZE))) + BORDER_LEFT+DOT_SIZE;
	y = (rand() % (BORDER_DOWN - (INFO_HEIGHT+DOT_SIZE))) + INFO_HEIGHT+DOT_SIZE;
	/*x = SCREEN_WIDTH / 2;
	x += 2;
	y = 2 + (SCREEN_HEIGHT / 2);*/
}

const int Dot::getx() {
	return x;
}

const int Dot::gety() {
	return y;
}

Dot::~Dot() {
	SDL_DestroyTexture(dot_sprite);
}
