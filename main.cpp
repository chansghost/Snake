//#define _USE_MATH_DEFINES

#include "includes.h"
#include "UI.h"
#include "snake.h"
#include "defines.h"
#include "game.h"




#ifdef __cplusplus
extern "C"
#endif
int main(int argc, char **argv) {
	srand(time(NULL));
	Game *game = new Game();
	game->start_game();
	game->gameplay();
	delete game;
	return 0;
	};
