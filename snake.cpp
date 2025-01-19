#include "snake.h"
#include <iostream>

Snake::Snake(int part_size, SDL_Renderer* renderer) : part_size(part_size), renderer(renderer){
	SDL_Surface* temp_head = SDL_LoadBMP("./head_up.bmp");
	SDL_Surface* temp_tail = SDL_LoadBMP("./tail_up.bmp");
	//SDL_Surface* temp_small_body_part = SDL_LoadBMP("./small_body_part.bmp");
	
	headsprite = SDL_CreateTextureFromSurface(renderer, temp_head);
	tailsprite = SDL_CreateTextureFromSurface(renderer, temp_tail);
	if (!headsprite || !tailsprite) {
		printf("Error loading sprites: snake %s\n", SDL_GetError());
		exit(1);
	}
	//SDL_Texture* small_body_part = SDL_CreateTextureFromSurface(renderer, temp_small_body_part);
	
	SDL_FreeSurface(temp_head);
	SDL_FreeSurface(temp_tail);
	//SDL_FreeSurface(temp_small_body_part);
	
	head = new SnakePart(SCREEN_WIDTH/2, SCREEN_HEIGHT / 2,0,0, headsprite);
	head->setNumber(0);
	
	head->setPositionChange(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2);
	tail = new SnakePart(SCREEN_WIDTH / 2, part_size+(SCREEN_HEIGHT / 2), 0,0, tailsprite);
	tail->setPositionChange(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2);
	//SnakePart* body1 = new SnakePart(200, 400, 0,0, small_body_part);
	head->setNext(tail);
	tail->setPrev(head);
	tail->setNumber(1);

	

	head->setNextTurn(head_node);
	tail->setNextTurn(head_node);
	head->setTurn(head_node);
	tail->setTurn(head_node);
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

	new_body_part->setPositionChange(temp->getPosCX(), temp->getPosCY()); //each item of the
	//linked list has to have position of the head
	new_body_part->setNumber(temp->getNumber() + 1);

	tail->setNumber(new_body_part->getNumber() + 1);
	new_body_part->setSize(size);
	//updating the list

	temp->setNext(new_body_part);

	new_body_part->setTurn(temp->getTurn());
	new_body_part->setNextTurn(temp->getNextTurn());

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
		tail->setPositionChange(temp->getPosCX(), temp->getPosCY());
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

void Snake::move_snake() {
	SnakePart* current = head->getnext();
	Turn_coords* turn=head_node;
	int done = 0;
	
	while (current != nullptr) {
		turn = head_node;
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

						current->setX(turn->x);
						current->setY(turn->y);

						current->setDirection(turn->direction);
						current->setPositionChange(turn->x, turn->y);
						turn->remaining -= 1;
						//printf("Made Turn. Segment: %d\n", current->getNumber());
						if (turn != nullptr && turn->remaining == 0) {
							head_node = turn->next;
							delete turn;
						}
					}
				}
			}
			even();
		}
		if (head_node == nullptr && pending_speed > 0) {
			speed = pending_speed;
			pending_speed = 0;
		}
		move_part(current);

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
	Turn_coords* temp = head_node;
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
	move_snake();
	
	detect_borders();
}

void Snake::handle_direction(int dir) {
	if (dir != head->getDirection()) {
		if (abs(dir - head->getDirection()) != 1) {
			//if the directions are opposites (check defines.h)
			//saving coords of new turn
			Turn_coords* new_node = new Turn_coords(head->getx(), head->gety(), dir, tail->getNumber(), speed);
			Turn_coords* temp = head_node;
			if (head_node == nullptr) {
				head_node = new_node;
			}
			
			else {
				while (temp->next != nullptr) {
					temp = temp->next;
				}
				temp->next = new_node;
			}

			head->setPositionChange(head->getx(), head->gety());
			head->setDirection(dir);
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
//wrocic tu kiedys
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

bool Snake::detect_self_collision() {
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
			return true;
		}
		current = current->getnext();
	}
	return false;
}

void Snake::render() {

	//ui->DrawSurface(screen, head->getSprite(), head->getx(), head->gety());
	//SDL_UpdateWindowSurface(window);
	//std::cout << "Head position: x=" << head->getx() << ", y=" << head->gety() << std::endl;
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
			NULL,            // Wyciêcie z tekstury (NULL = ca³oœæ)
			&dest_rect,      // Prostok¹t docelowy
			current->getAngle(),// K¹t obrotu
			NULL,            // Punkt obrotu (NULL = œrodek)
			SDL_FLIP_NONE    // Brak odwrócenia
		);
		current = current->getnext();
		//SDL_RenderPresent(rendererr);
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
	if (head_node != nullptr) {
		pending_speed = speed*SPEED_UP;
	}
	else {
		speed = speed*SPEED_UP;
	}
	printf("Snake has sped up!\n");
}

void Snake::slowdown() {
	if (head_node != nullptr) {
		pending_speed = speed * SLOW_DOWN;
	}
	else {
		speed = speed * SLOW_DOWN;
	}
	printf("Snake has slowed down!\n");
}

void Snake::check_for_dots(Dot* blue, Dot* red) {
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
			if (random==REMOVE) {
				printf("%d segments removed.\n" , REMOVE_AMOUNT);
				for (int i = 0; i < REMOVE_AMOUNT; i++) {
					
					removePart();
					
				}

			}
			else{
				
				slowdown();
			}
			points += RED_POINTS;
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
	return head_node;
}

void Snake::setTurn(Turn_coords* turn) {
	head_node = turn;
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
	
	if (head_node != nullptr) {
		Turn_coords* turntemp = head_node;
		while (turntemp->next != nullptr) {
			Turn_coords* turntemp2 = turntemp->next;
			delete turntemp;
			turntemp = turntemp2;
		}
	}
	SDL_DestroyTexture(headsprite);
	SDL_DestroyTexture(tailsprite);
};
