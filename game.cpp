#include "game.h"
#include <windows.h>

Game::Game() {
	
}

void Game::start_game() {
	
	ui = new UI();
	blue_dot = new Dot(BLUE, ui->getRenderer());
	red_dot = new Dot(RED, ui->getRenderer());
	snake = new Snake(SNAKE_SIZE, ui->getRenderer());
	
	
}

void Game::gameplay() {
	int t2;
	double delta;
	int game_state = 0;
	int t1 = SDL_GetTicks();
	while (game_state == PLAY) {
		t2 = SDL_GetTicks();
		delta = (t2 - t1) * 0.001;
		t1 = t2;
		last_speed_up += delta;
		last_spawn += delta;
		worldTime += delta;

		ui->draw_UI(worldTime, snake->getPoints(), last_spawn, red_dot->getTime(), red_dot->getSpawned());

		blue_dot->render(SDL_GetTicks());

		red_dot_management();

		snake->render();
		SDL_RenderPresent(ui->getRenderer());


		snake->check_for_dots(blue_dot, red_dot);

		snake->update();
		game_state = key_management();
		if (game_state == PLAY)
		{
			game_state = snake->detect_self_collision();
		}
		if (last_speed_up > SPEED_UP_TIME) {
			snake->speedup();
			last_speed_up = 0;
		}
	};
	updateRanking(snake->getPoints());
	if (game_state == NEW_GAME) restart();
	if (game_state == LOST) game_lost();
}

int Game::key_management() {
	int quit, new_game;
	while (SDL_PollEvent(&event)) {
		switch (event.type) {
		case SDL_KEYDOWN:
			if (event.key.keysym.sym == SDLK_ESCAPE) {
				return QUIT;
			}
			else if (event.key.keysym.sym == SDLK_UP) {
				snake->handle_direction(UP);
			}
			else if (event.key.keysym.sym == SDLK_DOWN) {
				snake->handle_direction(DOWN);
			}
			else if (event.key.keysym.sym == SDLK_LEFT) {
				snake->handle_direction(LEFT);
			}
			else if (event.key.keysym.sym == SDLK_RIGHT) {
				snake->handle_direction(RIGHT);
			}
			if (event.key.keysym.sym == SDLK_n) {
				return NEW_GAME;
			}
			else if (event.key.keysym.sym == SDLK_s) {
				save();
			}
			else if (event.key.keysym.sym == SDLK_l) {
				load();
			}
			break;

		case SDL_QUIT:
			return QUIT;
		}
	}
	return PLAY;
}


void Game::red_dot_management() {
	if (!red_dot->getSpawned() && last_spawn >= red_dot->getTime()) {

		red_dot->spawn();
		red_dot->setSpawned(true);
		last_spawn = 0;
	}


	if (red_dot->getSpawned()) {
		red_dot->render(SDL_GetTicks());


		if (last_spawn >= RED_LIFETIME) {

			red_dot->setSpawned(false);
			last_spawn = 0;
			red_dot->setTime();
		}
	}
}

void Game::game_lost() {
	bool restart_game = false;
	bool quit = false;

	while (!quit && !restart_game) {
		
		ui->game_lost_screen();
		
		while (SDL_PollEvent(&event)) {
			switch (event.type) {
			case SDL_KEYDOWN:
				if (event.key.keysym.sym == SDLK_n) {
					restart_game = true;
				}
				else if (event.key.keysym.sym == SDLK_ESCAPE) {
					quit = true;
				}
				break;

			case SDL_QUIT:
				quit = true;
				break;
			}
		}
	}

	if (restart_game) {
		restart(); 
	}
}
void Game::restart() {
	delete snake;
	delete ui;
	worldTime = 0;
	start_game();
	gameplay();
}

void Game::save() {
	FILE* file = fopen("snake.dat", "wb");
	if (!file) {
		perror("Failed to open file");
		return;
	}
	fwrite(&worldTime, sizeof(worldTime), 1, file);
	fwrite(&last_spawn, sizeof(last_spawn), 1, file);
	fwrite(&last_speed_up, sizeof(last_speed_up), 1, file);
	save_snake(file);
	save_turns(file);
	save_dot(file, BLUE);
	save_dot(file, RED);
	fclose(file);
	printf("Game saved successfully.\n");

}

void Game::save_snake(FILE* file) {
	SnakePart* current = snake->getHead();
	int count = snake->getLength();
	fwrite(&count, sizeof(count), 1, file);
	while (current != nullptr) {
		
		double x = current->getx();
		double y = current->gety();
		int direction = current->getDirection();
		int angle = current->getAngle();
		bool size = current->getSize();
		int number = current->getNumber();

		
		fwrite(&x, sizeof(x), 1, file);
		fwrite(&y, sizeof(y), 1, file);
		fwrite(&direction, sizeof(direction), 1, file);
		fwrite(&angle, sizeof(angle), 1, file);
		fwrite(&size, sizeof(size), 1, file);
		fwrite(&number, sizeof(number), 1, file);

		current = current->getnext();
	}
	double speed = snake->getSpeed();
	int points = snake->getPoints();
	fwrite(&speed, sizeof(speed), 1, file);
	fwrite(&points, sizeof(points), 1, file);
}

void Game::save_turns(FILE* file) {
	Turn_coords* current = snake->getTurn();
	int count = 0;
	Turn_coords* temp = current;
	while (temp) {
		count++;
		temp = temp->next;
	}

	
	fwrite(&count, sizeof(count), 1, file);
	while (current != nullptr) {
		fwrite(&current->x, sizeof(current->x), 1, file);
		fwrite(&current->y, sizeof(current->y), 1, file);
		fwrite(&current->direction, sizeof(current->direction), 1, file);
		fwrite(&current->remaining, sizeof(current->remaining), 1, file);
		fwrite(&current->speed, sizeof(current->speed), 1, file);

		current = current->next;
	}
}

void Game::save_dot(FILE* file, bool blue) {
	Dot* dot = blue ? blue_dot : red_dot;
	int x = dot->getx();
	int y = dot->gety();
	double scale = dot->getScale();
	int spawn_time = dot->getTime();
	bool spawned = dot->getSpawned();

	
	fwrite(&x, sizeof(x), 1, file);
	fwrite(&y, sizeof(y), 1, file);
	fwrite(&scale, sizeof(scale), 1, file);

	fwrite(&spawn_time, sizeof(spawn_time), 1, file);
	fwrite(&spawned, sizeof(spawned), 1, file);

	printf("Saving Dot: x=%d, y=%d, scale=%f\n", x, y, scale);
}



void Game::load_snake(FILE* file) {
	double x, y, speed;
	int direction, angle, number, points;
	bool size;
	int count = 0;
	fread(&count, sizeof(count), 1, file);

	snake = new Snake(SNAKE_SIZE, ui->getRenderer());

	SnakePart* current = nullptr;
	SnakePart* prev = nullptr;
	for (int i = 0; i < count; i++) {
		fread(&x, sizeof(x), 1, file);
		fread(&y, sizeof(y), 1, file);
		fread(&direction, sizeof(direction), 1, file);
		fread(&angle, sizeof(angle), 1, file);
		fread(&size, sizeof(size), 1, file);
		fread(&number, sizeof(number), 1, file);

		current = new SnakePart(ui->getRenderer(), x, y, direction, angle, size, number);
		if (number == 0) {

			snake->setHead(current);  
			current->setSprite(snake->getHeadSprite());
			prev = current;
		}
		else {
			current->setPrev(prev);
			prev->setNext(current);
			prev = current;
		}
		if (i == count - 1) {
			snake->setTail(current);
			current->setSprite(snake->getTailSprite());
		}
	}

	fread(&speed, sizeof(speed), 1, file);
	fread(&points, sizeof(points), 1, file);
	snake->setSpeed(speed);
	snake->setPoints(points); 
}


void Game::load_turns(FILE* file) {
	int count = 0;

	fread(&count, sizeof(count), 1, file);

	Turn_coords* prev = nullptr;
	for (int i = 0; i < count; i++) {
		double x, y, speed;
		int direction, remaining;

		fread(&x, sizeof(x), 1, file);
		fread(&y, sizeof(y), 1, file);
		fread(&direction, sizeof(direction), 1, file);
		fread(&remaining, sizeof(remaining), 1, file);
		fread(&speed, sizeof(speed), 1, file);

		Turn_coords* turn = new Turn_coords(x, y, direction, remaining, speed);
		if (prev) {
			prev->next = turn;
		}
		else {
			snake->setTurn(turn);
		}
		prev = turn;
	}
}

void Game::load_dot(FILE* file, bool blue) {
	Dot* dot;
	dot = new Dot(blue, ui->getRenderer());
	int spawn_time;
	bool spawned;
	int x, y;
	double scale;

	if (!blue) {
		
		red_dot = dot;
	}
	else {
		blue_dot = dot;
	}
	fread(&x, sizeof(x), 1, file);  
	fread(&y, sizeof(y), 1, file);
	fread(&scale, sizeof(scale), 1, file);  
	fread(&spawn_time, sizeof(spawn_time), 1, file);
	fread(&spawned, sizeof(spawned), 1, file);
	
	
	
	dot->setTime(spawn_time);
	dot->setSpawned(spawned);
	dot->setPos(x, y);  
	dot->setScale(scale);  
	printf("Loading Dot: x=%d, y=%d, scale=%f\n", x, y, scale);

}



void Game::loadRanking(const char* filename, ScoreEntry* ranking, int& count) {
	FILE* file = fopen(filename, "r");
	if (file) {
		count = 0;
		//check how many players
		while (fscanf(file, "%49s %d", ranking[count].name, &ranking[count].score) == 2 && count < 3) {
			count++;
		}
		fclose(file);
	}
}

void Game::saveRanking(const char* filename, ScoreEntry* ranking, int count) {
	FILE* file = fopen(filename, "w");
	if (file) {
		for (int i = 0; i < count; i++) {
			fprintf(file, "%s %d\n", ranking[i].name, ranking[i].score);
		}
		fclose(file);
	}
}

void Game::updateRanking(int playerScore) {
	const char* filename = "ranking.txt";
	ScoreEntry ranking[3];
	count = 0;

	loadRanking(filename, ranking, count);

	
	int minIndex = 0;
	for (int i = 1; i < count; i++) {
		if (ranking[i].score < ranking[minIndex].score) {//lowest score
			minIndex = i;
		}
	}

	if (count < 3 || playerScore > ranking[minIndex].score) {
		char playerName[50];
		ui->congrats_screen();
		std::cout << "Please enter your name: ";
		std::cin >> playerName;

		if (count < 3) {
			//add anyway if there's less than 3 players
			strcpy(ranking[count].name, playerName);
			ranking[count].score = playerScore;
			count++;
		}
		else {
			strcpy(ranking[minIndex].name, playerName);
			ranking[minIndex].score = playerScore;
		}

		
		for (int i = 0; i < count - 1; i++) {//sort points
			for (int j = 0; j < count - i - 1; j++) {
				if (ranking[j].score < ranking[j + 1].score) {
					ScoreEntry temp = ranking[j];
					ranking[j] = ranking[j + 1];
					ranking[j + 1] = temp;
				}
			}
		}

		
		saveRanking(filename, ranking, count);
	}
	else {
		std::cout << "Your score didn't make it to the top 3. Better luck next time!\n";
	}
	int quit = false;
	while (!quit) {
		ui->ranking(ranking, count);
		while (SDL_PollEvent(&event)) {
			switch (event.type) {
			case SDL_KEYDOWN:
				if (event.key.keysym.sym == SDLK_ESCAPE) {
					quit = true;
				}
				break;
			case SDL_QUIT:
				quit = true;
				break;
			}
		}
	}
}

void Game::load() {
	delete ui;
	delete snake;
	delete blue_dot;
	delete red_dot;
	ui = new UI();
	FILE* file = fopen("snake.dat", "rb");
	if (!file) {
		perror("Failed to open file");
		return;
	}

	
	fread(&worldTime, sizeof(worldTime), 1, file);
	fread(&last_spawn, sizeof(last_spawn), 1, file);
	fread(&last_speed_up, sizeof(last_speed_up), 1, file);

	load_snake(file);
	load_turns(file);
	load_dot(file, BLUE);  
	load_dot(file, RED); 

	fclose(file);
	printf("Game loaded successfully.\n");
}

Game::~Game() {
	delete ui;
	delete snake;
	
	SDL_Quit();
}