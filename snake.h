#pragma once
#include "snakeparts.h"
#include "includes.h"
#include "UI.h"
#include "portals.h"
#include "dot.h"
#include "turn_coords.h"


class Snake {
public:
	Snake(int part_size, SDL_Renderer* renderer);
	~Snake();
	void render();

	int detect_self_collision();

	void check_for_dots(Dot*blue,Dot*red, Portal** portals);

	void handle_direction(int dir);

	void speedup();

	void slowdown();

	SDL_Texture* getHeadSprite();
	SDL_Texture* getTailSprite();

	int getPoints();
	void setPoints(int point);
	void update();

	void update_coords_list(Turn_coords** new_coords_head, int direction,double x=-1, double y=-1);

	int getLength();

	double getSpeed();
	void setSpeed(double newspeed);

	SnakePart* getHead();

	SnakePart* getTail();
	void setTail(SnakePart*part);
	Turn_coords* getTurn();

	void setTurn(Turn_coords* turn);

	void setHead(SnakePart* newhead);
	void teleport_head();
	


private:
	SnakePart* head=nullptr;
	SnakePart* tail=nullptr; //items in snake body list

	SDL_Texture* headsprite=nullptr; //obrazy
	SDL_Texture* tailsprite=nullptr;
	int part_size=0;
	double speed = 0.4;
	double pending_speed=0;
	SDL_Renderer* renderer=nullptr;
	int points = 0;
	
	Turn_coords* head_node_turn=nullptr;
	Turn_coords* head_node_tp=nullptr;


	void update_remaining(bool add_remove);
	bool check_dot(int x, int y);

	void removePart();

	void even();
	void move_part(SnakePart* part);
	void move_snake(Turn_coords ** new_coords_head,bool tp);

	
	
	void addPart();
	void update_tail();

	int detect_Xborders();
	int detect_Yborders();

	void detect_borders();
};
