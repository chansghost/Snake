#include "snake.h"
#include <iostream>

Snake::Snake(int part_size, SDL_Renderer* renderer) : part_size(part_size), renderer(renderer){
	SDL_Surface* temp_head = SDL_LoadBMP("./head_up.bmp");
	SDL_Surface* temp_tail = SDL_LoadBMP("./tail_up.bmp");
	
	headsprite = SDL_CreateTextureFromSurface(renderer, temp_head);
	tailsprite = SDL_CreateTextureFromSurface(renderer, temp_tail);
	if (!headsprite || !tailsprite) {
		printf("Error loading sprites: snake %s\n", SDL_GetError());
		exit(1);
	}
	
	SDL_FreeSurface(temp_head);
	SDL_FreeSurface(temp_tail);
	
	head = new SnakePart(SCREEN_WIDTH/2, SCREEN_HEIGHT / 2,0,0, headsprite);
	head->setNumber(0);
	
	tail = new SnakePart(SCREEN_WIDTH / 2, part_size+(SCREEN_HEIGHT / 2), 0,0, tailsprite);
	
	head->setNext(tail);
	tail->setPrev(head);
	tail->setNumber(1);

	for (int i = 0; i < DEFAULT_BODY_LENGTH; i++) {
		addPart();
	}
}

SDL_Texture* Snake::getHeadSprite() {
	return headsprite;
}

SDL_Texture* Snake::getTailSprite() {
	return tailsprite;
}


void Snake::addPart() {
	SnakePart* temp = tail->getPrev();
	SDL_Surface* temp_bodypart;
	SDL_Texture* bodypart;
	bool size;
	if (temp->getSize() == SMALL) {
		temp_bodypart = SDL_LoadBMP("./big_body_part.bmp");
		size = BIG;
	}
	else {
		temp_bodypart = SDL_LoadBMP("./small_body_part.bmp");
		size = SMALL;
	}
	bodypart = SDL_CreateTextureFromSurface(renderer, temp_bodypart);
	SDL_FreeSurface(temp_bodypart);

	SnakePart* new_body_part = new SnakePart(tail->getx(), tail->gety(),
		tail->getDirection(), tail->getAngle(), bodypart);

	
	new_body_part->setNumber(temp->getNumber() + 1);

	tail->setNumber(new_body_part->getNumber() + 1);
	new_body_part->setSize(size);
	//updating the list

	temp->setNext(new_body_part);

	

	new_body_part->setPrev(temp);
	new_body_part->setNext(tail);
	tail->setPrev(new_body_part);

	update_tail();
}

void Snake::removePart() {
	
	SnakePart* temp = tail->getPrev();
	if (temp->getNumber() > DEFAULT_BODY_LENGTH) {

		update_remaining(REMOVE);
		SnakePart* new_behind = temp->getPrev();

		new_behind->setNext(tail);
		tail->setPrev(new_behind);
		tail->setX(temp->getx());
		tail->setY(temp->gety());

		tail->setDirection(temp->getDirection());
		tail->setAngle(temp->getAngle());
		tail->setNumber(tail->getNumber() - 1);

		delete temp;
		
	}
}

void Snake::update_tail() {
	int dir = tail->getDirection();
	double x = tail->getx();
	double y = tail->gety();
	if (dir == UP) y += part_size;
	else if (dir == DOWN) y -= part_size;
	else if (dir == RIGHT) x -= part_size;
	else if (dir == LEFT) x += part_size;
	tail->setX(x);
	tail->setY(y);
}

void Snake::move_part(SnakePart* part) {
	int dir = part->getDirection();
	if (dir == UP)
	{
		part->setY(part->gety() - speed);
		part->setAngle(0);
	}
	else if (dir == DOWN) {
		part->setY(part->gety() + speed);
		part->setAngle(180);
	}
	else if (dir == RIGHT) {
		part->setX(part->getx() + speed);
		part->setAngle(90);
	}
	else if (dir == LEFT) {
		part->setX(part->getx() - speed);
		part->setAngle(270);
	}
	
}

void Snake::move_snake(Turn_coords** new_coords_head,bool tp) {
	SnakePart* current = head->getnext();
	Turn_coords* turn = *new_coords_head;
	int done = 0;
	
	while (current != nullptr) {
		turn = *new_coords_head;
		if (turn != nullptr) {
			done = tail->getNumber() - turn->remaining;
			while (current->getNumber() < done) {
				turn = turn->next;
				if (turn == nullptr) break;
				done = tail->getNumber() - turn->remaining;
			}
		}

		if (turn != nullptr){
			if(abs(current->getx() - turn->x) <= speed) {
				if (abs(current->gety() - turn->y) <= speed) {
					if( current->getDirection() != turn->direction) {
						if (*new_coords_head!=head_node_tp) {
							current->setX(turn->x);
							current->setY(turn->y);
						}
						else {
							current->setX(turn->tp_x);
							current->setY(turn->tp_y);
						}

						current->setDirection(turn->direction);
						turn->remaining -= 1;
						//printf("Made Turn. Segment: %d\n", current->getNumber());
						if (turn != nullptr && turn->remaining == 0) {
							*new_coords_head = turn->next;
							
							delete turn;
						}
					}
				}
			}
			if(!tp) even(); //even out the position of the head, so that
			//the body doesn't move onto it from the bigger tolerance of detecting turning point
			
		}
		if (*new_coords_head == nullptr && pending_speed > 0) {
			speed = pending_speed;
			pending_speed = 0;
		}
		if(!tp) move_part(current);
		

		current = current->getnext();
	}
	
}

void Snake::even() {
	SnakePart* temp = head->getnext();
	if (head->getDirection() == temp->getDirection()) {
		int direction = head->getDirection();
		if (direction == LEFT) {
			head->setX(temp->getx() - (part_size));
		}
		else if (direction == RIGHT) {
			head->setX(temp->getx() + (part_size));
		}
		else if (direction == UP) {
			head->setY(temp->gety() - (part_size));
		}
		else if (direction == DOWN) {
			head->setY(temp->gety() +(part_size));
		}
	}
}
	
void Snake::update_remaining(bool add_remove) {
	Turn_coords* temp = head_node_turn;
	if (add_remove == ADD) {
		while (temp != nullptr) {
			temp->remaining += 1;
			temp = temp->next;
		}
	}
	else {
		while (temp != nullptr) {
			temp->remaining -= 1;
			temp = temp->next;
		}
	}
}


void Snake::update() {
	
	move_part(head);
	move_snake(&head_node_turn,false);
	move_snake(&head_node_tp,true);
	
	detect_borders();
}

void Snake::update_coords_list(Turn_coords** new_coords_head, int direction, double x, double y) {
	Turn_coords* new_node= new Turn_coords(head->getx(), head->gety(), direction, tail->getNumber(), speed);;
	if (x != -1 && y != -1) {
		new_node->setTP(x, y); //setting the coords we need to teleport to
	}
	
	Turn_coords* temp = *new_coords_head;
	if (*new_coords_head == nullptr) {
		*new_coords_head = new_node;
	}

	else {
		while (temp->next != nullptr) {
			temp = temp->next;
		}
		temp->next = new_node;
	}
	head->setDirection(direction);
}

void Snake::handle_direction(int dir) {
	if (dir != head->getDirection()) {
		if (abs(dir - head->getDirection()) != 1) {
			//if the directions are opposites (check defines.h)
			//saving coords of new turn
			update_coords_list(&head_node_turn, dir);

			
			
		}
	}
}
int Snake::detect_Xborders() {
	if (head->getx() >= BORDER_RIGHT-25) return RIGHT;
	else if (head->getx() <= BORDER_LEFT) return LEFT;
	return -1;
}

int Snake::detect_Yborders() {
	if (head->gety() >= BORDER_DOWN) return DOWN;
	else if (head->gety() <= BORDER_UP) return UP;
	return -1;
}

void Snake::detect_borders() {
	int Yborder = detect_Yborders(); 
	int Xborder = detect_Xborders(); 
	int current_direction = head->getDirection(); 
	int new_direction = current_direction;  

	
	if (Yborder == DOWN && current_direction == DOWN) { 
		new_direction = LEFT;
	}
	else if (Yborder == UP && current_direction == UP) {
		new_direction = (head->getx() <= BORDER_RIGHT / 2) ? RIGHT : LEFT;

	}
	if (Xborder == LEFT && current_direction == LEFT) {  
		new_direction = UP;
	}
	else if (Xborder == RIGHT && current_direction == RIGHT) {  
		new_direction = (head->gety() <= BORDER_DOWN / 2) ? DOWN : UP;  
	}

	
	if (new_direction != current_direction) {
		handle_direction(new_direction);  
	}
}

int Snake::detect_self_collision() {
	SnakePart* current = head->getnext();
	current = current->getnext();//segments usually cover each other a little bit, 
	//so we skip checking the segment that is right after the head
	double headX = head->getx();
	double headY = head->gety();//ceil is used for the same reason  as in the move snake function
	double x_segment, y_segment;
	double fieldX, fieldY;
	int segment_direction;

	while (current != nullptr) {
		segment_direction = current->getDirection();
		x_segment = current->getx();
		y_segment = current->gety();
		fieldX = x_segment - part_size / 2;//we need to get the position of the beggining of the segment since the x and y
		//point to the middle of it
		fieldY = y_segment - part_size / 2;
		if (headX > fieldX && headX<(fieldX + part_size) && headY>fieldY && headY < (fieldY + part_size)) {
			return LOST;
		}
		current = current->getnext();
	}
	return PLAY;
}

void Snake::render() {

	SnakePart* current = head;
	while (current != nullptr) {
		SDL_Rect dest_rect = {
			static_cast<int>(current->getx()),
			static_cast<int>(current->gety()),
			part_size,
			part_size
		};

		SDL_RenderCopyEx(
			renderer,
			current->getSprite(),
			NULL,            
			&dest_rect,     
			current->getAngle(),
			NULL,            
			SDL_FLIP_NONE    
		);
		current = current->getnext();
	}
}

bool Snake::check_dot(int x, int y) {
	double snakeX = head->getx();
	double snakeY = head->gety();

	double tolerance = 5.0; //error margin

	if (abs(snakeX - x) <= (part_size / 2.0 + DOT_SIZE / 2.0 + tolerance) &&
		abs(snakeY - y) <= (part_size / 2.0 + DOT_SIZE / 2.0 + tolerance)) {
		
		return true;
	}

	return false;
}

int Snake::getPoints() {
	return points;
}

void Snake::speedup() {
	if (head_node_turn != nullptr) {
		pending_speed = speed*SPEED_UP;
	}
	else {
		speed = speed*SPEED_UP;
	}
	printf("Snake has sped up!\n");
}

void Snake::slowdown() {
	if (head_node_turn != nullptr) {
		pending_speed = speed * SLOW_DOWN;
	}
	else {
		speed = speed * SLOW_DOWN;
	}
	printf("Snake has slowed down!\n");
}

void Snake::teleport_head() {
	int x = head_node_tp->tp_x;
	int y = head_node_tp->tp_y;
	//moving head to the location of the corresponding portal
	//int dir = head->getDirection();
	head->setX(x);
	head->setY(y);
}

void Snake::check_for_dots(Dot* blue, Dot* red,Portal** portals) {
	int x = blue->getx();
	int y = blue->gety();
	int random;
	if (check_dot(x, y)) {
		blue->spawn();
		addPart();
		update_remaining(ADD);
		printf("New segment added!\n");
		points += BLUE_POINTS;
		return;
	}
	if (red->getSpawned()) {
		x = red->getx();
		y = red->gety();
		if (check_dot(x, y)) {
			red->setSpawned(false);
			red->setTime();
			random = rand() % 2;
			if (random == REMOVE) {
				printf("%d segments removed.\n", REMOVE_AMOUNT);
				for (int i = 0; i < REMOVE_AMOUNT; i++) {
					removePart();
				}

			}
			else {

				slowdown();
			}
			points += RED_POINTS;
		}
	}
	for (int i = 0; i < TP_NUMBER*2; i++) {
		x = portals[i]->getx();
		y = portals[i]->gety();
		Portal* second_port=nullptr;
		int dir = -1;
		int number = portals[i]->get_portal_nr();
		if (check_dot(x, y)) {
			
			if ((i != 0 && portals[i - 1]->get_portal_nr() == number)) {
				//get x,y
				second_port = portals[i - 1];
				
				
			}
			else if((i + 1) != TP_NUMBER * 2 && portals[i + 1]->get_portal_nr() == number)
			{
				second_port = portals[i + 1];
			}
			x = second_port->getx();
			y = second_port->gety();

			
			if (head->getDirection() == LEFT || head->getDirection() == DOWN) {
				dir = (head->getDirection() - 1); //opposite direction, check defines.h for explanation
			}
			else { dir = head->getDirection() + 1; }

			update_coords_list(&head_node_tp, dir,x,y);
			teleport_head();
			portals[i]->spawn();
			second_port->spawn();

			
		}
	}
}

SnakePart* Snake::getHead() {
	return head;
}

SnakePart* Snake::getTail() {
	return tail;
}

void Snake::setTail(SnakePart* part) {
	tail = part;
}

Turn_coords* Snake::getTurn() {
	return head_node_turn;
}

void Snake::setTurn(Turn_coords* turn) {
	head_node_turn = turn;
}

double Snake::getSpeed() {
	return speed;
}

void Snake::setPoints(int point) {
	points = point;
}
void Snake::setSpeed(double newspeed) {
	speed = newspeed;
}

void Snake::setHead(SnakePart* newhead) {
	SDL_Surface* temp_head = SDL_LoadBMP("./head_up.bmp");
	headsprite = SDL_CreateTextureFromSurface(renderer, temp_head);
	SDL_Surface* temp_tail = SDL_LoadBMP("./tail_up.bmp");
	tailsprite = SDL_CreateTextureFromSurface(renderer, temp_tail);
	SDL_FreeSurface(temp_head);
	SDL_FreeSurface(temp_tail);
	head = newhead;
}


int Snake::getLength() {
	return (tail->getNumber() + 1);
}
Snake::~Snake() {
	SnakePart* temp = head;
	
	while (temp->getnext() != nullptr) {
		SnakePart*  temp2 = temp->getnext();
		delete temp;
		temp = temp2;
	}
	delete temp;
	
	if (head_node_turn != nullptr) {
		Turn_coords* turntemp = head_node_turn;
		while (turntemp->next != nullptr) {
			Turn_coords* turntemp2 = turntemp->next;
			delete turntemp;
			turntemp = turntemp2;
		}
	}
	SDL_DestroyTexture(headsprite);
	SDL_DestroyTexture(tailsprite);
};
