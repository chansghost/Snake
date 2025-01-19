#include "snakeparts.h"

SnakePart::SnakePart(double x, double y, int direction, int angle, SDL_Texture* bodysprite) : 
	x(x), y(y), direction(direction), angle(angle),bodysprite(bodysprite) {
	
}

SnakePart::SnakePart(SDL_Renderer* renderer,double x, double y, int direction, int angle, bool size, int segment_number) :
	x(x), y(y), direction(direction), angle(angle), size(size), segment_number(segment_number) {
	
	SDL_Surface* temp_bodypart;
	if (size == SMALL) {
		temp_bodypart = SDL_LoadBMP("./small_body_part.bmp");
	}
	else {
		temp_bodypart = SDL_LoadBMP("./big_body_part.bmp");
	}
	bodysprite= SDL_CreateTextureFromSurface(renderer, temp_bodypart);
	SDL_FreeSurface(temp_bodypart);
}

void SnakePart::setSprite(SDL_Texture* sprite) {
	bodysprite = sprite;
}

void SnakePart::setDirection(int newdir) {
	direction = newdir;
}

void SnakePart::setX(double newx) {
	x = newx;
}

void SnakePart::setY(double newy) {
	y = newy;
}

void SnakePart::setAngle(int new_angle) {
	angle = new_angle;
}

const int SnakePart::getAngle() {
	return angle;
}

const double SnakePart::getx() {
	return x;
}

const double SnakePart::gety() {
	return y;
}

const int SnakePart::getDirection() {
	return direction;
}
SDL_Texture* SnakePart::getSprite() {
	return bodysprite; 
}

void SnakePart::setNext(SnakePart* body) {
	next = body;
}

SnakePart* SnakePart::getnext() {
	return next;
}

void SnakePart::setPrev(SnakePart* body) {
	prev = body;
}

SnakePart* SnakePart::getPrev() {
	return prev;
}

const bool SnakePart::getSize() {
	return size;
}

void SnakePart::setPositionChange(double x,double y) {
	position_changeX = x;
	position_changeY = y;
}

const double SnakePart::getPosCX(){
	return position_changeX;
}

const double SnakePart::getPosCY() {
	return position_changeY;
}

void SnakePart::setSize(bool newsize) {
	size = newsize;
}

void SnakePart::setNumber(int number) {
	segment_number = number;
}

int SnakePart::getNumber() {
	return segment_number;
}

Turn_coords* SnakePart::getTurn() {
	return made_turn;
}

void SnakePart::setTurn(Turn_coords*turn) {
	made_turn = turn;
}

Turn_coords* SnakePart::getNextTurn() {
	return next_turn;
}

void SnakePart::setNextTurn(Turn_coords* turn) {
	next_turn = turn;
}

SnakePart::~SnakePart() {
	SDL_DestroyTexture(bodysprite);
};
