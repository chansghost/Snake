#pragma once
#include "includes.h"
//#include "ui.h"


class Dot {
public:
	Dot(bool blue, SDL_Renderer* renderer);
	~Dot();
	void render(Uint32 time);
	Dot(SDL_Renderer* renderer);

	const int getx();
	const int gety();

	void spawn();
	void setTime();
	void setTime(int spawn_time);

	const int getTime();

	double getScale();

	void setScale(double newscale);

	void setPos(int newx, int newy);

	

	bool getSpawned();
	void setSpawned(bool newspawned);
	
protected:
	SDL_Renderer* renderer=nullptr;
	int x, y;
	double pulse_speed=0.005;
	bool portal = false;
	SDL_Texture* dot_sprite=nullptr;
	bool blue = true;
	int spawn_time = 0;
	double scale=1;
	bool spawned = false;


};