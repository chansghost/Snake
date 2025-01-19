#pragma once
#include "includes.h"
#include "ui.h"


class Dot {
public:
	Dot(bool blue, SDL_Renderer* renderer);
	~Dot();
	
	void pulsate_dot();

	const int getx();
	const int gety();

	void spawn();
	void setTime();
	void setTime(int spawn_time);

	const int getTime();

	double getScale();

	void setScale(double newscale);

	void setPos(int newx, int newy);

	void render(Uint32 time);

	bool getSpawned();
	void setSpawned(bool newspawned);
	
protected:
	SDL_Renderer* renderer;
	int x, y;
	double pulse_speed=0.005;
	SDL_Texture* dot_sprite=nullptr;
	bool blue = true;
	int spawn_time = 0;
	double scale=1;
	bool spawned = false;

};