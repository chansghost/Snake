#pragma once
#include "includes.h"
#include "portals.h"
#include "score_entry.h"

class UI {
public:
	UI();
	~UI();
	

	void draw_Borders(SDL_Surface* screen, Uint32 fillColor);
	void draw_UI(double time, int Points, double last_spawn, double red_spawn,bool spawned);

	void game_lost_screen();

	void congrats_screen();

	
	void ranking(ScoreEntry ranking[],int count);

	SDL_Renderer* getRenderer();
	SDL_Surface* getScreen();

	SDL_Window* getWindow();

	void Draw_portal_numbers(Portal** portals);
	

private:
	int black, pink,blue;
	char text[128];
	int rc;
	bool cleared = false;
	
	SDL_Surface* screen;
	SDL_Surface* charset;
	SDL_Texture* scrtex;
	SDL_Window* window;
	SDL_Renderer* renderer;
	void DrawString(SDL_Surface* screen, int x, int y, const char* text,
		SDL_Surface* charset);
	void DrawSurface(SDL_Surface* screen, SDL_Surface* sprite, int x, int y);
	void DrawPixel(SDL_Surface* surface, int x, int y, Uint32 color);
	void DrawLine(SDL_Surface* screen, int x, int y, int l, int dx, int dy, Uint32 color);
	void DrawRectangle(SDL_Surface* screen, int x, int y, int l, int k,
		Uint32 outlineColor, Uint32 fillColor);


};