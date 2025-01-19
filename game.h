#pragma once
#include "includes.h"
#include "classes_include.h"
#include "score_entry.h"

class Game {
public:
	Game();
	~Game();


	void start_game();
	void restart();
	void game_lost();

	void save();
	void load();
	void gameplay();

	
	
	

private:
	UI* ui;
	Snake* snake;
	const Uint32 cooldown = 300;
	SDL_Event event;
	double worldTime=0;
	Dot* blue_dot=nullptr;
	Dot* red_dot=nullptr;
	double last_spawn = 0;
	double last_speed_up = 0;
	ScoreEntry* ranking;
	int count = 0;
	void loadRanking(const char* filename, ScoreEntry* ranking, int& count);

	void save_snake(FILE* file);
	void load_snake(FILE* file);

	void save_turns(FILE* file);
	void load_turns(FILE* file);

	void save_dot(FILE* file, bool blue);
	void load_dot(FILE* file, bool blue);
	void saveRanking(const char* filename, ScoreEntry* ranking, int count);
	void updateRanking(int playerScore);

	int key_management();

	void red_dot_management();
};
