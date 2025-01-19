#pragma once
#include "includes.h"
#include "turn_coords.h"

class SnakePart {
public:
	SnakePart(double x, double y, int direction, int angle, SDL_Texture*bodysprite);
	SnakePart(SDL_Renderer* renderer,double x, double y, int direction, int angle, bool size, int segment_number);
	~SnakePart();

	void setDirection(int newdir);
	void setX(double newx);
	void setY(double newy);

	void setAngle(int new_angle);

	const int getAngle();
	const double getx();
	const double gety();
	const int getDirection();
	SDL_Texture* getSprite();
	void setSprite(SDL_Texture* sprite);

	void setNext(SnakePart* body);
	SnakePart* getnext();
	void setPrev(SnakePart* body);
	SnakePart* getPrev();

	const bool getSize();
	void setSize(bool newsize);

	void setPositionChange(double x, double y);


	const double getPosCX();
	const double getPosCY();

	void setNumber(int number);

	int getNumber();

	Turn_coords* getTurn();

	void setTurn(Turn_coords*turn);

	void setNextTurn(Turn_coords* turn);

	Turn_coords* getNextTurn();

private:
	double x, y;
	int direction;
	int angle;//variable for easier image flipping management
	SDL_Texture* bodysprite;
	bool size = BIG;
	SnakePart* next=nullptr;
	SnakePart* prev = nullptr;
	int segment_number = 0;
	double position_changeX, position_changeY;  //variables needed for
	//making the snake turn
	Turn_coords* made_turn=nullptr;//do we really need pointers
	Turn_coords* next_turn=nullptr;
};